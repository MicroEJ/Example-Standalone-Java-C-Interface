/*
 * C
 *
 * Copyright 2014-2016 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */
#include <LLNET_CHANNEL_impl.h>

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
#include "LLNET_ERRORS.h"

#ifdef __cplusplus
	extern "C" {
#endif

///* From http://www.iana.org/assignments/port-numbers:
//   "The Dynamic and/or Private Ports are those from 49152 through 65535" */
//#define LOCAL_PORT_RANGE_START        0xc000
//#define LOCAL_PORT_RANGE_END          0xffff
//#define ENSURE_LOCAL_PORT_RANGE(port) (((port) & ~LOCAL_PORT_RANGE_START) + LOCAL_PORT_RANGE_START)

extern void __iceTea__componentInit_dispatch_7events(void);

//int32_t firstPort = -1;
//int32_t nextPort;
//
//int32_t LLNET_CHANNEL_IMPL_nextPort(){
//	if(firstPort == -1){
//		// Use random only for the first call of this function.
//		uint32_t random = LLNET_getRandomNumber();
//		if(random == 0){
//			// If random function is not implemented, use default stack random
//			firstPort = 0;
//		}
//		else {
//			// Computed port  must be different from the port reserved by the stack.
//			firstPort = ENSURE_LOCAL_PORT_RANGE(random);
//		}
//
//		nextPort = firstPort;
//	}
//	else if(nextPort != 0){
//		// This is not the first call to this function and random function returns a valid value (not 0)
//		++nextPort;
//		if(nextPort >= LOCAL_PORT_RANGE_END){
//			nextPort = firstPort;
//		}
//	}
//	return nextPort;
//
//}

extern int32_t map_to_java_exception(int32_t err);

int32_t LLNET_CHANNEL_IMPL_close(int32_t fd, uint8_t retry)
{
	LLNET_DEBUG_TRACE("%s[thread %d](fd=%d)\n", __func__, SNI_getCurrentJavaThreadID(), fd);
	if(close(fd) == -1){
		return map_to_java_exception(errno);
	}
	return 0;
}
int32_t LLNET_CHANNEL_IMPL_initialize(void)
{
	LLNET_DEBUG_TRACE("%s\n", __func__);
	__iceTea__componentInit_dispatch_7events(); // init LLDISPATCH IceTea component
	return 0;
}
int32_t LLNET_CHANNEL_IMPL_setBlocking(int32_t fd, uint8_t blocking, uint8_t retry)
{
	LLNET_DEBUG_TRACE("%s[thread %d](fd=%d, blocking=%d)\n", __func__, SNI_getCurrentJavaThreadID(), fd, blocking);
	int32_t opts = fcntl(fd, F_GETFL, 0);
	if(opts >= 0){
		if(fcntl(fd, F_SETFL, blocking? (opts &= ~O_NONBLOCK) : (opts |= O_NONBLOCK)) == 0){
			return 0;
		}
	}
	return map_to_java_exception(errno);
}
int32_t LLNET_CHANNEL_IMPL_shutdown(int32_t fd, uint8_t retry)
{
	LLNET_DEBUG_TRACE("%s[thread %d](fd=%d)\n", __func__, SNI_getCurrentJavaThreadID(), fd);
	int32_t ret = shutdown(fd, SHUT_RD); //shutdown input stream
	LLNET_DEBUG_TRACE("%s[thread %d](fd=%d) ret=%d errno=%d\n", __func__, SNI_getCurrentJavaThreadID(), fd, ret, errno);
	return 0;
}
int32_t LLNET_CHANNEL_IMPL_bind(int32_t fd, int8_t* addr, int32_t offset, int32_t length, int32_t port, uint8_t retry)
{
	LLNET_DEBUG_TRACE("%s[thread %d](fd=%d, port=%d,...)\n", __func__, SNI_getCurrentJavaThreadID(), fd, port);
	struct sockaddr_in sockaddr;

//	//If port is not specified, get a random port
//	if(port == 0){
//		port = LLNET_CHANNEL_IMPL_nextPort();
//	}

	memset(&sockaddr, 0, sizeof(sockaddr));
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(port);
	*((int32_t*)(&sockaddr.sin_addr.s_addr)) = *((int32_t*)addr);
	int32_t ret = bind(fd, (struct sockaddr*)&sockaddr, sizeof(struct sockaddr_in));
	LLNET_DEBUG_TRACE("%s[thread %d](fd=%d, errno=%d, ...)\n", __func__, SNI_getCurrentJavaThreadID(), fd, errno);
	if(ret == -1){
		return map_to_java_exception(errno);
	}
	return 0;
}
int32_t LLNET_CHANNEL_IMPL_getOption(int32_t fd, int32_t option, uint8_t retry)
{
	LLNET_DEBUG_TRACE("%s[thread %d](fd=%d, option=%d)\n", __func__, SNI_getCurrentJavaThreadID(), fd, option);
	int32_t optname = -1;
	int32_t level = SOL_SOCKET;
	int32_t optlen = sizeof(int32_t);
	int32_t optVal;
	struct timeval sock_timeout;
	void* p_optVal = &optVal;

	switch (option) {
		case CPNET_SO_KEEPALIVE:
			optname = SO_KEEPALIVE;
			break;
		case CPNET_SO_BROADCAST:
			optname = SO_BROADCAST;
			break;
		case CPNET_SO_TIMEOUT :
			p_optVal = &sock_timeout;
		  optlen = sizeof(sock_timeout);
			optname = SO_RCVTIMEO;
			break;
		case CPNET_SO_SNDBUF :
			optname = SO_SNDBUF;
			break;
		case CPNET_SO_RCVBUF :
			optname = SO_RCVBUF;
			break;
		case CPNET_TCP_NODELAY:
			level = IPPROTO_TCP;
			optname = TCP_NODELAY;
			break;
		case CPNET_IP_TOS:
			level = IPPROTO_IP;
			optname = IP_TOS;
			break;
		case CPNET_SO_REUSEADDR :
			optname = SO_REUSEADDR;
			break;
		case CPNET_SO_OOBINLINE:	// no urgent data option for this implementation
		default:
			//option not available
			return J_ENOPROTOOPT;
	}

	int32_t res = getsockopt(fd, level, optname, p_optVal, (socklen_t *)&optlen);
	LLNET_DEBUG_TRACE("%s[thread %d](fd=%d, option=%d) res=%d errno=%d\n", __func__, SNI_getCurrentJavaThreadID(), fd, option, res, errno);
	if(res == -1){
		return map_to_java_exception(errno);
	}

	if(option == CPNET_SO_TIMEOUT){
		return (sock_timeout.tv_usec / 1000) + (sock_timeout.tv_sec * 1000);
	}
	return optVal;
}
int32_t LLNET_CHANNEL_IMPL_setOption(int32_t fd, int32_t option, int32_t value, uint8_t retry)
{
	LLNET_DEBUG_TRACE("%s[thread %d](fd=%d, option=%d, value=%d)\n", __func__, SNI_getCurrentJavaThreadID(), fd, option, value);
	int32_t optname = -1;
	int32_t level = SOL_SOCKET;
  struct timeval sock_timeout;

	int optlen = sizeof(int32_t);

	void* p_optval = &value;

	switch (option) {
		case CPNET_SO_KEEPALIVE:
			optname = SO_KEEPALIVE;
			break;
		case CPNET_SO_BROADCAST:
			optname = SO_BROADCAST;
			break;
		case CPNET_SO_TIMEOUT:
			if(value > 1000){
				sock_timeout.tv_sec = value / 1000;
				sock_timeout.tv_usec = (value % 1000) * 1000;
			}else{
				sock_timeout.tv_sec = 0;
				sock_timeout.tv_usec = value * 1000;
			}
			p_optval = &sock_timeout;
			optlen = sizeof(sock_timeout);
			optname = SO_RCVTIMEO;
			break;
		case CPNET_SO_SNDBUF:
			optname = SO_SNDBUF;
			break;
		case CPNET_SO_RCVBUF:
			optname = SO_RCVBUF;
			break;
		case CPNET_TCP_NODELAY:
			level = IPPROTO_TCP;
			optname = TCP_NODELAY;
			break;
		case CPNET_IP_TOS:
			level = IPPROTO_IP;
			optname = IP_TOS;
			break;
		case CPNET_SO_REUSEADDR :	// no reuse option for this implementation
			optname = SO_REUSEADDR;
			break;
		case CPNET_SO_OOBINLINE:	// no urgent data option for this implementation
		default:
			/* option not available */
			return J_ENOPROTOOPT;
	}
	int32_t res = setsockopt (fd, level, optname, p_optval, optlen);
	LLNET_DEBUG_TRACE("%s[thread %d](fd=%d, option=%d, value=%d), res=%d, errno=%d\n", __func__, SNI_getCurrentJavaThreadID(), fd, option, value, res, errno);
	if (res == -1) {
		return map_to_java_exception(errno);
	}
	return 0;
}
int32_t LLNET_CHANNEL_IMPL_listen(int32_t fd, int32_t backlog, uint8_t retry)
{
	LLNET_DEBUG_TRACE("%s[thread %d](fd=%d, backlog=%d)\n", __func__, SNI_getCurrentJavaThreadID(), fd, backlog);
	int32_t res = listen(fd, backlog);
	if(res == -1){
		return map_to_java_exception(errno);
	}
	return 0;
}

//-----------------------------
//	Internal API
//-----------------------------
int32_t NonBlocking_Select(int32_t fd, int32_t operation){
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
	int32_t selectRes = -1;
	if(operation == LLDISPATCH_CONSTANTS_REQUEST_READ){
		selectRes = select(fd+1, &io_fds, &empty_fds, &err_fds, &zero_timeout);
	}else if(operation == LLDISPATCH_CONSTANTS_REQUEST_WRITE){
		selectRes = select(fd+1, &empty_fds, &io_fds, &err_fds, &zero_timeout);
	}else{
		//unsupported IO/Operation
		LLNET_DEBUG_ASSERT("%s: wrong operation == %d!\n", __func__, operation);
	}

	return selectRes;
}

#ifdef __cplusplus
	}
#endif
