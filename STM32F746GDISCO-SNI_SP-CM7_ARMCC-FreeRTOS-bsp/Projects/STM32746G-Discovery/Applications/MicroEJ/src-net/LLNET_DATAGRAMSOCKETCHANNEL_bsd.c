/*
 * C
 *
 * Copyright 2014-2016 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */
#include <LLNET_DATAGRAMSOCKETCHANNEL_impl.h>

#include <stdio.h>
#include <string.h>
#include <lwip/err.h>
#ifndef LWIP_PROVIDE_ERRNO
#include <errno.h>
#endif
#include <sys/socket.h>
#include "LLNET_CONSTANTS.h"
#include "LLDISPATCH_CONSTANTS.h"
#include "LLDISPATCH_EVENT.h"
#include "LLNET_CHANNEL_impl.h"
#include "LLNET_ERRORS.h"

#ifdef __cplusplus
	extern "C" {
#endif

extern int32_t map_to_java_exception(int32_t err);
		
int64_t DatagramSocketChannel_recvfrom(int32_t fd, int8_t* dst, int32_t dstOffset, int32_t dstLength, int8_t* hostPort, int32_t hostPortOffset, int32_t hostPortLength, uint8_t retry){
	LLNET_DEBUG_TRACE("%s(fd=%d, ..., retry=%d)\n", __func__, fd, retry);
	struct sockaddr	 sockAddr;
	int32_t addrLen = sizeof(sockAddr);
	int32_t flags = MSG_WAITALL;
	memset(&sockAddr, 0, sizeof(sockAddr));
	int32_t ret = recvfrom(fd, dst+dstOffset, dstLength, flags, &sockAddr, (socklen_t *)&addrLen);

	if (ret == -1) {
		return map_to_java_exception(errno);
	}

	if (sockAddr.sa_family == AF_INET) {
		// push host address in result bufffer
		struct sockaddr_in* sockAddrIn = (struct sockaddr_in*)&sockAddr;
		*((int32_t*)hostPort+hostPortOffset) = (sockAddrIn->sin_addr.s_addr);
		// push host address in result buffer
		*((int32_t*)hostPort+hostPortOffset+1) = ntohs(sockAddrIn->sin_port);
        // add data receive length in return value
        int64_t retValue = (((int64_t)ret) << 32l);
        // add host length in return value
        retValue |= 4l;
		return retValue;
	}else if(ret == 0){
		return 0;
	}
	//unsupported address family
	return J_EAFNOSUPPORT;
}

int32_t DatagramSocketChannel_sendto(int32_t fd, int8_t* src, int32_t srcoffset, int32_t srclength, int8_t* addr, int32_t addroffset, int32_t addrlength, int32_t port, uint8_t retry){
	LLNET_DEBUG_TRACE("%s(fd=%d, ..., retry=%d)\n", __func__, fd, retry);
	struct sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(struct sockaddr_in));
	sockAddr.sin_family = AF_INET;
	*((int32_t*)(&sockAddr.sin_addr.s_addr)) = *((int32_t*)addr+addroffset);
	sockAddr.sin_port = htons(port);
	int32_t flags = 0;
	int32_t ret = sendto(fd, src+srcoffset, srclength, flags,(struct sockaddr*)&sockAddr, sizeof(struct sockaddr_in));
	if(ret == -1){
		if(errno == EAGAIN || errno == EWOULDBLOCK){
			return 0;
		}
		return map_to_java_exception(errno);
	}
	return ret;
}

int64_t DatagramSocketChannel_asyncOperation(int32_t fd, int32_t operation, int8_t* data, int32_t dataOffset, int32_t dataLength, int8_t* addr, int32_t addroffset, int32_t addrlength, int32_t port, int8_t* hostPort, int32_t hostPortOffset, int32_t hostPortLength, uint8_t retry)
{
	struct timeval zero_timeout;
	zero_timeout.tv_sec = 0;
	zero_timeout.tv_usec = 0;
	fd_set io_fds;
	fd_set err_fds;
	fd_set empty_fds;
	FD_ZERO(&io_fds);
	FD_ZERO(&err_fds);
	FD_ZERO(&empty_fds);
	FD_SET(fd, &io_fds);
	// first, a select with zero timeout is done to check if the socket is readable or writable.
	// The select call is non blocking when the given timeout is equals to zero.
	// On success, the socket is ready to read or write data. A blocking call to recv function
	// can be done. It will not block the socket.
	int32_t selectRes = -1;
	if(operation == LLDISPATCH_CONSTANTS_REQUEST_READ){
		selectRes = select(fd+1, &io_fds, &empty_fds, &err_fds, &zero_timeout);
	}else if(operation == LLDISPATCH_CONSTANTS_REQUEST_WRITE){
		selectRes = select(fd+1, &empty_fds, &io_fds, &err_fds, &zero_timeout);
	}else{
		//unsupported IO/Operation
		LLNET_DEBUG_ASSERT("%s: wrong operation == %d!\n", __func__, operation);
	}
	if(selectRes == 0){
		// the socket is not readable or writable.
		// A blocking request is added in the dispatch event queue.
		int32_t javaThreadID = SNI_getCurrentJavaThreadID();
		int32_t timeout = LLNET_CHANNEL_IMPL_getOption(fd, CPNET_SO_TIMEOUT, 0);
		//check the timeout
		if(timeout < 0){
			//error code
			return timeout;
		}
		if((timeout != 0) && retry){
			//The socket has been configured with a timeout
			//AND there is no available data
			//AND this is the second read (retry==true)
			//THEN timeout has been reached during the first read
			return J_ETIMEDOUT;
		}
		uint8_t added = LLDISPATCH_EVENT_addBlockingRequest(fd, javaThreadID, operation, timeout);
		if(added){
			// request added in the queue
			return J_NET_NATIVE_CODE_BLOCKED_WITHOUT_RESULT;
		}
		// requests queue limit reached
		return J_ASYNC_BLOCKING_REQUEST_QUEUE_LIMIT_REACHED;
	}else{
		if(operation == LLDISPATCH_CONSTANTS_REQUEST_READ){
			return DatagramSocketChannel_recvfrom(fd, data, dataOffset, dataLength, hostPort, hostPortOffset, hostPortLength, retry);
		}else if(operation == LLDISPATCH_CONSTANTS_REQUEST_WRITE){
			return DatagramSocketChannel_sendto(fd, data, dataOffset, dataLength, addr, addroffset, addrlength, port, retry);
		}else{
			LLNET_DEBUG_ASSERT("%s: wrong operation == %d!\n", __func__, operation);
		}
		return J_EINVAL;
	}
}

int64_t LLNET_DATAGRAMSOCKETCHANNEL_IMPL_receive(int32_t fd, int8_t* dst, int32_t dstOffset, int32_t dstLength, int8_t* hostPort, int32_t hostPortOffset, int32_t hostPortLength, uint8_t retry)
{
	LLNET_DEBUG_TRACE("%s(fd=%d, ..., retry=%d)\n", __func__, fd, retry);
	return DatagramSocketChannel_asyncOperation(fd, LLDISPATCH_CONSTANTS_REQUEST_READ, dst, dstOffset, dstLength, 0, 0, 0, 0, hostPort, hostPortOffset, hostPortLength,retry);
}
int32_t LLNET_DATAGRAMSOCKETCHANNEL_IMPL_send(int32_t fd, int8_t* src, int32_t srcoffset, int32_t srclength, int8_t* addr, int32_t addroffset, int32_t addrlength, int32_t port, uint8_t retry)
{
	LLNET_DEBUG_TRACE("%s(fd=%d, ..., retry=%d)\n", __func__, fd, retry);
	return (int32_t)DatagramSocketChannel_asyncOperation(fd, LLDISPATCH_CONSTANTS_REQUEST_WRITE, src, srcoffset, srclength, addr, addroffset, addrlength, port, NULL, 0, 0, retry);
}
int32_t LLNET_DATAGRAMSOCKETCHANNEL_IMPL_disconnect(int32_t fd, uint8_t retry)
{
	LLNET_DEBUG_TRACE("%s(fd=%d)\n ", __func__, fd);
	struct sockaddr sockAddr;
	sockAddr.sa_family = AF_UNSPEC;
	if(connect(fd, &sockAddr, sizeof(struct sockaddr)) == -1) {
		return map_to_java_exception(errno);
	}
	return 0; //success
}
#ifdef __cplusplus
	}
#endif
