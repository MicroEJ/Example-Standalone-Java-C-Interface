/*
 * C
 *
 * Copyright 2014-2016 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */
#include <LLNET_STREAMSOCKETCHANNEL_impl.h>

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
#include "LLNET_Cfg.h"
#include <LLNET_CHANNEL_impl.h>
#include "LLNET_ERRORS.h"

#ifdef __cplusplus
	extern "C" {
#endif

extern int32_t map_to_java_exception(int32_t err);
		
int32_t StreamSocketChannel_accept(int32_t fd)
{
	LLNET_DEBUG_TRACE("%s[thread %d](fd = %d)\n", __func__, SNI_getCurrentJavaThreadID(), fd);
	int32_t ret;
	ret = accept(fd, NULL, NULL);
	if(ret == -1){
		LLNET_DEBUG_TRACE("Error on accept (fd = %d)\n", fd);
		return map_to_java_exception(errno);
	}

	LLNET_DEBUG_TRACE("Successful accept OK fd = %d\n", ret);
	return ret;
}

int32_t StreamSocketChannel_writeByteBufferNative(int32_t fd, int8_t* src, int32_t offset, int32_t length)
{
	LLNET_DEBUG_TRACE("%s[thread %d]\n", __func__, SNI_getCurrentJavaThreadID());
	int32_t ret;
	ret = send(fd, src+offset, length, 0);
	LLNET_DEBUG_TRACE("sent bytes size = %d (length=%d)\n", ret, length);
	if(ret == -1){
		return map_to_java_exception(errno);
	}
	return ret;
}

int32_t StreamSocketChannel_readByteBufferNative(int32_t fd, int8_t* dst, int32_t offset, int32_t length)
{
	LLNET_DEBUG_TRACE("%s[thread %d]\n", __func__, SNI_getCurrentJavaThreadID());
	int32_t ret;
	ret = recv(fd, dst+offset, length, 0);
	LLNET_DEBUG_TRACE("nb received data : %d errno=%d\n", ret, errno);
	if(ret == -1){
		return map_to_java_exception(errno);
	}

	if (0 == ret) {
		return -1; //EOF
	}
	return ret;
}

int32_t StreamSocketChannel_asyncOperation(int32_t fd, int32_t operation, uint8_t isAccept, int8_t* data, int32_t offset, int32_t length, uint8_t retry)
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
			LLNET_DEBUG_TRACE("Native Code Has Blocked Without Result\n");
			return J_NET_NATIVE_CODE_BLOCKED_WITHOUT_RESULT;
		}
		// requests queue limit reached
		LLNET_DEBUG_TRACE("Blocking Request Queue Limit Reached\n");
		return J_ASYNC_BLOCKING_REQUEST_QUEUE_LIMIT_REACHED;
	}else{
		if(operation == LLDISPATCH_CONSTANTS_REQUEST_READ){
			if(isAccept){
				return StreamSocketChannel_accept(fd);
			}else{
				return StreamSocketChannel_readByteBufferNative(fd, data, offset, length);
			}
		}else if(operation == LLDISPATCH_CONSTANTS_REQUEST_WRITE){
			return StreamSocketChannel_writeByteBufferNative(fd, data, offset, length);
		}else{
			LLNET_DEBUG_ASSERT("%s: wrong operation == %d!\n", __func__, operation);
		}
		return J_EINVAL;
	}
}

int32_t LLNET_STREAMSOCKETCHANNEL_IMPL_readByteBufferNative(int32_t fd, int32_t kind, int8_t* dst, int32_t offset, int32_t length, uint8_t retry)
{
	LLNET_DEBUG_TRACE("%s[thread %d](fd=%d, kind=%d, retry=%d)\n", __func__, SNI_getCurrentJavaThreadID(), fd, kind, retry);
	return StreamSocketChannel_asyncOperation(fd, LLDISPATCH_CONSTANTS_REQUEST_READ, JFALSE, dst, offset, length, retry);
}

int32_t LLNET_STREAMSOCKETCHANNEL_IMPL_writeByteBufferNative(int32_t fd, int32_t kind, int8_t* src, int32_t offset, int32_t length, uint8_t retry)
{
	LLNET_DEBUG_TRACE("%s[thread %d]\n", __func__, SNI_getCurrentJavaThreadID());
	return StreamSocketChannel_asyncOperation(fd, LLDISPATCH_CONSTANTS_REQUEST_WRITE, JFALSE, src, offset, length, retry);
}

int32_t LLNET_STREAMSOCKETCHANNEL_IMPL_available(int32_t fd, uint8_t retry)
{
	LLNET_DEBUG_TRACE("%s[thread %d](fd=%d)\n", __func__, SNI_getCurrentJavaThreadID(), fd);
	int32_t size = recv(fd, NET_EMBEDDED_AVAILABLE_BUFFER, NET_EMBEDDED_AVAILABLE_BUFFER_SIZE, MSG_PEEK | MSG_DONTWAIT);
	LLNET_DEBUG_TRACE("%s[thread %d](fd=%d) size=%d errno=%d\n", __func__, SNI_getCurrentJavaThreadID(), fd, size, errno);
	if(size >= 0){
		return size;
	}
	if(errno == EAGAIN){
		return 0;
	}
	return map_to_java_exception(errno);
}

int32_t LLNET_STREAMSOCKETCHANNEL_IMPL_accept(int32_t fd, uint8_t retry)
{
	LLNET_DEBUG_TRACE("%s[thread %d](fd=%d, retry=%d)\n", __func__, SNI_getCurrentJavaThreadID(), fd, retry);
	return StreamSocketChannel_asyncOperation(fd, LLDISPATCH_CONSTANTS_REQUEST_READ, JTRUE, NULL, 0, 0, retry);
}

#ifdef __cplusplus
	}
#endif
