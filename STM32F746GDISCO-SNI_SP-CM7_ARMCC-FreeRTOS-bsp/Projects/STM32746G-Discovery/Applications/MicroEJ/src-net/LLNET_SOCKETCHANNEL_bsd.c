/*
 * C
 *
 * Copyright 2014-2016 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */
#include <LLNET_SOCKETCHANNEL_impl.h>

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
#include "LLNET_NETWORKADDRESS_impl.h"
#include "LLNET_SOCKETCHANNEL_lwip.h"
#include "LLNET_ERRORS.h"

#ifdef __cplusplus
	extern "C" {
#endif

// external function used to retrieve currentTime (same as MicroJvm)
extern int64_t LLMJVM_IMPL_getCurrentTime__Z(uint8_t system);
		
extern int32_t NonBlocking_Select(int32_t fd, int32_t operation);

extern int32_t map_to_java_exception(int32_t err);
		
// connection timeout table used to retrieve SocketTimeoutException during a connection
static int64_t connectTimeoutTable[MEMP_NUM_UDP_PCB+MEMP_NUM_TCP_PCB];

int32_t LLNET_SOCKETCHANNEL_IMPL_connect(int32_t fd, int8_t* addr, int32_t offset, int32_t length, int32_t port, int32_t timeout, uint8_t retry)
{
	LLNET_DEBUG_TRACE("%s[thread %d](fd=%d, port=%d, timeout=%d)\n", __func__, SNI_getCurrentJavaThreadID(),fd, port, timeout);
	struct sockaddr_in addr_remote;
	int32_t origflags;
	int32_t flags;

	if(retry){
		//check if the socket is connected
		struct sockaddr_in sockaddr;
		struct sockaddr * p_sockaddr = (struct sockaddr *)&sockaddr;
		u32_t addrlen = sizeof(struct sockaddr_in);
		int32_t selectRes = NonBlocking_Select(fd, LLDISPATCH_CONSTANTS_REQUEST_WRITE);
		int32_t peerRes = getpeername(fd, p_sockaddr, &addrlen);
		if(selectRes > 0 && (peerRes == 0 && sockaddr.sin_port > 0)){
			connectTimeoutTable[fd] = 0; // reset the connection timeout of the socket
			//the socket is connected to a remote host
			return 1; //success
		}
		if(selectRes == 0 || (peerRes < 0 && errno == ENOTCONN)){
			//the socket is not connected and
			//the timeout expires during the first connect
			if(timeout > 0){
				// a timeout is set for this request, look at timeout expiration
				if(LLMJVM_IMPL_getCurrentTime__Z(1) < connectTimeoutTable[fd]){
					// error: timeout not reached and socket not connected to the host
					connectTimeoutTable[fd] = 0; // reset connect timeout table entry
					return J_ENOTCONN;
				}

				// error: timeout expired, return a timeout exception
				connectTimeoutTable[fd] = 0; // reset connect timeout table entry
				return J_ETIMEDOUT;
			}
		}
		//error
		connectTimeoutTable[fd] = 0; // reset the connection timeout of the socket
		return map_to_java_exception(errno);
	}

	addr_remote.sin_family = AF_INET;
	addr_remote.sin_port = htons(port);
	*((int32_t*)(&addr_remote.sin_addr.s_addr)) = *((int32_t*)addr);
	origflags = fcntl(fd, F_GETFL, 0);
	if (origflags == -1) {
		return map_to_java_exception(errno);
	}
	/* Set nonblocking mode, if not already set. */
	if (!((origflags & O_NONBLOCK) != 0)) {
		flags = origflags | O_NONBLOCK;
		if (fcntl(fd, F_SETFL, flags) == -1) {
			return map_to_java_exception(errno);
		}
	}

	connectTimeoutTable[fd] = LLMJVM_IMPL_getCurrentTime__Z(1) + timeout; // set connect timeout
	int32_t res = connect(fd, (struct sockaddr*)&addr_remote, sizeof(struct sockaddr_in));
	int32_t connectErrno = errno;
	
	/* Reset the non-blocking flag, if needed. */
	if (!((origflags & O_NONBLOCK) != 0)) {
		if (fcntl(fd, F_SETFL, origflags) == -1) {
			return map_to_java_exception(errno);
		}
	}
	if(res == -1){
		LLNET_DEBUG_TRACE("[ERROR] Connection failed (retry=%d, errno=%d)\n", retry, errno);
		if(connectErrno == EINPROGRESS) { //The connection can not be completed immediately.
								   // A blocking request on write operation is added in the dispatch event queue for completion.
			int32_t javaThreadID = SNI_getCurrentJavaThreadID();
			uint8_t added = LLDISPATCH_EVENT_addBlockingRequest(fd, javaThreadID, LLDISPATCH_CONSTANTS_REQUEST_WRITE, timeout);
			if(added){
				// request added in the queue
				LLNET_DEBUG_TRACE("Native Code Has Blocked Without Result\n");
				return J_NET_NATIVE_CODE_BLOCKED_WITHOUT_RESULT;
			}
			// requests queue limit reached
			LLNET_DEBUG_TRACE("Blocking Request Queue Limit Reached\n");
			return J_ASYNC_BLOCKING_REQUEST_QUEUE_LIMIT_REACHED;
		}
		return map_to_java_exception(connectErrno);
	}
	connectTimeoutTable[fd] = 0; // reset connect timeout table entry
	return 1;// connected
}

int32_t LLNET_SOCKETCHANNEL_IMPL_getLocalPort(int32_t fd, uint8_t retry)
{
	LLNET_DEBUG_TRACE("%s[thread %d](fd=%d)\n", __func__, SNI_getCurrentJavaThreadID(), fd);
	return SocketChanel_Port(fd, JTRUE);
}

int32_t LLNET_SOCKETCHANNEL_IMPL_getPeerAddress(int32_t fd, int8_t* name, int32_t offset, int32_t length, uint8_t retry)
{
	LLNET_DEBUG_TRACE("%s[thread %d](fd=%d)\n", __func__, SNI_getCurrentJavaThreadID(), fd);
	return SocketChanel_Address(fd, name, JFALSE);
}

int32_t LLNET_SOCKETCHANNEL_IMPL_getPeerPort(int32_t fd, uint8_t retry)
{
	LLNET_DEBUG_TRACE("%s[thread %d](fd=%d)\n", __func__, SNI_getCurrentJavaThreadID(), fd);
	return SocketChanel_Port(fd, JFALSE);
}

int32_t LLNET_SOCKETCHANNEL_IMPL_getLocalAddress(int32_t fd, int8_t* name, int32_t offset, int32_t length, uint8_t retry)
{
	LLNET_DEBUG_TRACE("%s[thread %d](fd=%d)\n", __func__, SNI_getCurrentJavaThreadID(), fd);
	return SocketChanel_Address(fd, name, JTRUE);
}

int32_t SocketChanel_Port(int32_t fd, uint8_t localPort){
	struct sockaddr sockaddr;
	u32_t addrlen = sizeof(struct sockaddr_in);
	int32_t ret = -1;
	if(localPort){
		ret = getsockname(fd, &sockaddr, &addrlen);
	}else{
		ret = getpeername(fd, &sockaddr, &addrlen);
	}
	if (ret == 0 && sockaddr.sa_family == AF_INET) {
		return ntohs(((struct sockaddr_in *)&sockaddr)->sin_port);
	}

	return map_to_java_exception(errno);
}

int32_t SocketChanel_Address(int32_t fd, int8_t* name, uint8_t localAddress){
	struct sockaddr_in sockaddr;
	struct sockaddr * p_sockaddr = (struct sockaddr *)&sockaddr;
	u32_t addrlen = sizeof(struct sockaddr_in);
	int32_t ret = -1;
	if(localAddress){
		ret = getsockname(fd, p_sockaddr, &addrlen);
		LLNET_DEBUG_TRACE("getsockname(fd=%d) ret=%d errno=%d\n", fd,ret,errno);
	}else{
		ret = getpeername(fd, p_sockaddr, &addrlen);
		LLNET_DEBUG_TRACE("getpeername(fd=%d) ret=%d errno=%d\n", fd,ret,errno);
	}
	if (ret == 0 && p_sockaddr->sa_family == AF_INET) {
		if(!localAddress && (sockaddr.sin_addr.s_addr == 0 || sockaddr.sin_port == 0)){
			//not connected
			return J_ENOTCONN;
		}
		ret = sizeof(struct in_addr);
		memcpy(name, (void *)&(sockaddr.sin_addr), ret);
		return ret;
	}
	return map_to_java_exception(errno);
}

int32_t LLNET_SOCKETCHANNEL_IMPL_socket(uint8_t stream, uint8_t retry)
{
	LLNET_DEBUG_TRACE("%s[thread %d](stream=%d) ", __func__, SNI_getCurrentJavaThreadID(), stream);
	int32_t ret;
	int32_t sock_protocol = 0; //default value
	ret = socket(AF_INET, stream ? SOCK_STREAM : SOCK_DGRAM, sock_protocol);
	if (ret == -1) {
		int32_t err = map_to_java_exception(errno);
		LLNET_DEBUG_TRACE("err=%d\n", err);
		return err;
	}
	LLNET_DEBUG_TRACE("fd=%d\n", ret);
	return ret;
}

int32_t LLNET_SOCKETCHANNEL_IMPL_serverSocket(uint8_t retry){
	return LLNET_SOCKETCHANNEL_IMPL_socket(JTRUE, retry);
}

#ifdef __cplusplus
	}
#endif
