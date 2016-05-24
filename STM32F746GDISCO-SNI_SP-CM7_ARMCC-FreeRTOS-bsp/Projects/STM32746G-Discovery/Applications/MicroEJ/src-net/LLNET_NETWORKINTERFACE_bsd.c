/*
 * C
 *
 * Copyright 2014-2016 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */
#include <LLNET_NETWORKINTERFACE_impl.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <lwip/opt.h>
#include <lwip/netif.h>
#include <lwip/sockets.h>
#include "LLNET_CONSTANTS.h"
#include "LLNET_ERRORS.h"

#ifdef __cplusplus
	extern "C" {
#endif

static struct netif * getNetworkInterface(int8_t* name, int32_t offset, int32_t length);

int32_t LLNET_NETWORKINTERFACE_IMPL_getVMInterface(int32_t id, int8_t* nameReturned, int32_t offset, int32_t length, uint8_t retry)
{
	LLNET_DEBUG_TRACE("%s(id=%d)\n", __func__, id);
	// retrieve network interface list global variable
	struct netif *pnetif_list = netif_list;
	int32_t ifCnt = -1;
	// look at each network interface to find the one matching with 'idIf'
	while(pnetif_list != NULL){
		if(++ifCnt == id){
			if(pnetif_list->name != NULL){
				size_t tmpLen = strlen(pnetif_list->name);
				size_t pnetif_list_name_len = tmpLen+1 > length ? length-1 : tmpLen;
				// interface found, return the configured name
				memcpy(nameReturned+offset, pnetif_list->name, pnetif_list_name_len);
				// add a '\0' at the end of the name returned
				*(nameReturned+offset+pnetif_list_name_len) = 0;
				return pnetif_list_name_len;
			}else{
				// add a '\0' at the end of the name returned
				*(nameReturned+offset) = 0;
				return 0;
			}
		}
		// not found, look at the next interface
		pnetif_list = pnetif_list->next;
	}
	// error: no address found at the 'idAddr' index
	return 0;
}

int32_t LLNET_NETWORKINTERFACE_IMPL_getVMInterfaceAddress(int32_t idIf, int32_t idAddr, int8_t* nameReturned, int32_t offset, int32_t length, uint8_t retry)
{
	LLNET_DEBUG_TRACE("%s\n", __func__);
	// retrieve network interface list global variable
	struct netif *pnetif_list = netif_list;
	// look at each network interface to find the one matching with 'idIf'
	while(pnetif_list != NULL){
		if(pnetif_list->num == idIf){
			// interface found, return the configured ip address
			// 'idAddr' not take into account here because only one ip address
			// is configured per network interface
			memcpy(nameReturned, &pnetif_list->ip_addr, sizeof(ip_addr_t));
			return sizeof(ip_addr_t);
		}
		// not found, look at the next interface
		pnetif_list = pnetif_list->next;
	}
	// error: no address found at the 'idAddr' index
	return 0;
}

int32_t LLNET_NETWORKINTERFACE_IMPL_getVMInterfaceAddressesCount(int32_t id, uint8_t retry)
{
	LLNET_DEBUG_TRACE("%s\n", __func__);
	return 1; // only one address configuration per network interface
}

int32_t LLNET_NETWORKINTERFACE_IMPL_getVMInterfacesCount(uint8_t retry)
{
	LLNET_DEBUG_TRACE("%s\n", __func__);
	struct netif *pnetif_list = netif_list;
	int32_t ifCnt = 0;
	while(pnetif_list != NULL){
		++ifCnt;
		// get next interface, and verify it exists or not
		pnetif_list = pnetif_list->next;
	}
	return ifCnt;
}

int32_t LLNET_NETWORKINTERFACE_IMPL_isLoopback(int8_t* name, int32_t offset, int32_t length, uint8_t retry)
{
	LLNET_DEBUG_TRACE("%s\n", __func__);
	#ifdef LWIP_NETIF_LOOPBACK
		return 0; // 0 means true
	#else
		return 1; // 1 means false
	#endif
}

int32_t LLNET_NETWORKINTERFACE_IMPL_isPointToPoint(int8_t* name, int32_t offset, int32_t length, uint8_t retry)
{
	LLNET_DEBUG_TRACE("%s\n", __func__);
	return 1; // 0 means true
}

int32_t LLNET_NETWORKINTERFACE_IMPL_isUp(int8_t* name, int32_t offset, int32_t length, uint8_t retry)
{
	LLNET_DEBUG_TRACE("%s\n", __func__);
	struct netif *pnetif = getNetworkInterface(name, offset, length);
	if(pnetif != NULL){
		return netif_is_up(pnetif) ? 0 : 1; // 0 means true
	}
	return J_EUNKNOWN;
}

int32_t LLNET_NETWORKINTERFACE_IMPL_supportsMulticast(int8_t* name, int32_t offset, int32_t length, uint8_t retry)
{
	LLNET_DEBUG_TRACE("%s\n", __func__);
	struct netif *pnetif = getNetworkInterface(name, offset, length);
	if(pnetif != NULL){
		return (pnetif->flags & NETIF_FLAG_BROADCAST) != 0 ? 0 : 1; // 0 means true
	}
	return J_EUNKNOWN;
}

int32_t LLNET_NETWORKINTERFACE_IMPL_getHardwareAddress(int8_t* name, int32_t offset, int32_t length, int8_t* hwAddr, int32_t hwAddrOffset, int32_t hwAddrMaxLength, uint8_t retry)
{
	LLNET_DEBUG_TRACE("%s\n", __func__);
	struct netif *pnetif = getNetworkInterface(name, offset, length);
	if(pnetif != NULL){
		memcpy(hwAddr+hwAddrOffset, pnetif->hwaddr, pnetif->hwaddr_len);
		return pnetif->hwaddr_len;
	}
	// hardware address not found
	return 0;
}

static struct netif * getNetworkInterface(int8_t* name, int32_t offset, int32_t length){
	struct netif *pnetif_list = netif_list;
	while(pnetif_list != NULL){
		if(strncmp((char const*)name, pnetif_list->name, 2) == 0){
			return pnetif_list;
		}
		// not the right interface, get next
		pnetif_list = pnetif_list->next;
	}
	return NULL;
}

#ifdef __cplusplus
	}
#endif
