/*
 * C
 *
 * Copyright 2014-2016 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */
#include <LLNET_NETWORKADDRESS_impl.h>

#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include "LLNET_CONSTANTS.h"
#include "LLNET_ERRORS.h"

#ifdef __cplusplus
	extern "C" {
#endif

int32_t LLNET_NETWORKADDRESS_IMPL_aton(int8_t* inOut, int32_t offset, int32_t length, uint8_t retry)
{
	LLNET_DEBUG_TRACE("%s\n", __func__);
	//return address in network order (not host order)
	uint32_t inetAddr = inet_addr((char*)(inOut+offset));
	//invalid host
	if(((int32_t)inetAddr) == -1){
		return inetAddr;
	}
	*(int32_t*)(inOut+offset) = inetAddr;
	return 4;
}

int32_t LLNET_NETWORKADDRESS_IMPL_getLocalHostnameNative(int8_t* result, int32_t offset, int32_t length, uint8_t retry)
{
	LLNET_DEBUG_TRACE("%s\n", __func__);
	// Localhost IP addresses are any host address in the '127.<host>' subnet
	// so we took an arbitrary localhost address
	result[0] = (int8_t)127;
	result[1] = (int8_t)0;
	result[2] = (int8_t)0;
	result[3] = (int8_t)1;
	return 0;
}

int32_t LLNET_NETWORKADDRESS_IMPL_lookupInaddrAny(int8_t* result, int32_t offset, int32_t length, uint8_t retry)
{
	LLNET_DEBUG_TRACE("%s\n", __func__);
	result[0] = (int8_t)0;
	result[1] = (int8_t)0;
	result[2] = (int8_t)0;
	result[3] = (int8_t)0;
	return 4;
}

int32_t LLNET_NETWORKADDRESS_IMPL_loopbackAddress(int8_t* result, int32_t offset, int32_t length, uint8_t retry){
	LLNET_DEBUG_TRACE("%s\n", __func__);
	result[0] = (int8_t)127;
	result[1] = (int8_t)0;
	result[2] = (int8_t)0;
	result[3] = (int8_t)1;
	return 4;
}

#ifdef __cplusplus
	}
#endif
