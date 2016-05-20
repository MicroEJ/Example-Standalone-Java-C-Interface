/*
 * C
 *
 * Copyright 2014-2015 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

#include <LLNET_Cfg.h>
#include <stdint.h>
#include <stdlib.h>

static uint8_t* ptr_device_ip_addr = NULL;
static uint8_t* ptr_gateway_ip_addr = NULL;
static uint8_t* ptr_netmask = NULL;
static uint8_t* ptr_dns_ip_addr = NULL;

/**
 * Set device IP address
 */
void LLNET_setDeviceIPAddress(uint8_t* deviceIPAddr){
      ptr_device_ip_addr = deviceIPAddr;
}


/**
 * Device IP address
 */
uint8_t* LLNET_getDeviceIPAddress(){
      if(ptr_device_ip_addr == NULL && (((int32_t)&LLNET_DHCP_CONF) == LLNET_FALSE)){
        // if DHCP configuration is disabled and device IP not initialized,
        // device IP address is provided by 'LLNET_DEVICE_IP_ADDRESS' symbol
        LLNET_setDeviceIPAddress((uint8_t*)&LLNET_DEVICE_IP_ADDRESS);
      }
      return ptr_device_ip_addr;
}

/**
 * Set device netmask
 */
void LLNET_setDeviceNetmask(uint8_t* deviceNetmask){
	ptr_netmask = deviceNetmask;
}

/**
 * Device netmask
 */
uint8_t* LLNET_getDeviceNetmask(){
    if((ptr_netmask == NULL) && (((int32_t)&LLNET_DHCP_CONF) == LLNET_FALSE)){
      // if DHCP configuration is disabled and netmask not initialized,
      // netmask is provided by 'LLNET_NETMASK' symbol
    	LLNET_setDeviceNetmask((uint8_t*)&LLNET_NETMASK);
    }
    return ptr_netmask;
}

/**
 * Set device IP Gateway address
 */
void LLNET_setDeviceGatewayIPAddr(uint8_t* gatewayIPAddr){
	ptr_gateway_ip_addr = gatewayIPAddr;
}

/**
 * Device IP Gateway address
 */
uint8_t* LLNET_getDeviceGatewayIPAddr(){
    if(ptr_gateway_ip_addr == NULL && (((int32_t)&LLNET_DHCP_CONF) == LLNET_FALSE)){
      // if DHCP configuration is disabled and gateway IP not initialized,
      // gateway IP address is provided by 'LLNET_GATEWAY_IP_ADDRESS' symbol
    	LLNET_setDeviceGatewayIPAddr((uint8_t*)&LLNET_GATEWAY_IP_ADDRESS);
    }
    return ptr_gateway_ip_addr;
}

/**
 * Set DNS host IP address
 */
void LLNET_setDNSHostIPAddr(uint8_t* dnsIPAddr){
	ptr_dns_ip_addr = dnsIPAddr;
}

/**
 * DNS host IP address
 */
uint8_t* LLNET_getDNSHostIPAddr(){
	int32_t dhcpConfEnabled = ((int32_t)&LLNET_DHCP_CONF == LLNET_TRUE);
	int32_t dhcpdnsConfEnabled = ((int32_t)&LLNET_DHCP_DNS_CONF == LLNET_TRUE);
	
    if((ptr_dns_ip_addr == NULL) && ((dhcpConfEnabled && !dhcpdnsConfEnabled) || (!dhcpConfEnabled))){
      // if DHCP configuration, DHCP DNS configuration is disabled and DNS IP not initialized,
      // DNS IP address is provided by 'LLNET_DNS_IP_ADDRESS' symbol
    	LLNET_setDNSHostIPAddr((uint8_t*)&LLNET_DNS_IP_ADDRESS);
    }
    return ptr_dns_ip_addr;
}

/**
 * MAC address
 */
uint8_t* LLNET_getMACAddr(){
	return (uint8_t*)&LLNET_MAC_ADDRESS;
}

/**
 * Net Embedded task settings.
 */ 
int16_t 		NET_EMBEDDED_TASK_PRIORITY	= 12;

/**
 * Net Embedded available buffer settings
 */
#define 	NET_EMBEDDED_AVAILABLE_BUFFER_SIZE_		4096
int32_t 	NET_EMBEDDED_AVAILABLE_BUFFER_SIZE = NET_EMBEDDED_AVAILABLE_BUFFER_SIZE_;
uint8_t 	NET_EMBEDDED_AVAILABLE_BUFFER[NET_EMBEDDED_AVAILABLE_BUFFER_SIZE_];
