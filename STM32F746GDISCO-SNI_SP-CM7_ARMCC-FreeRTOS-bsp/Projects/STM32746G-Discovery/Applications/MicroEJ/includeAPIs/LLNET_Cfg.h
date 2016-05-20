/*
 * C
 *
 * Copyright 2014-2015 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */
#ifndef LLNET_CFG_H
#define LLNET_CFG_H

#include <intern/LLNET_Cfg.h>
#include <stdint.h>

#define LLNET_TRUE		(1)
#define LLNET_FALSE 	(-1)

/**
 * Set device IP address
 */
void LLNET_setDeviceIPAddress(uint8_t* deviceIPAddr);

/**
 * Device IP address
 */
uint8_t* LLNET_getDeviceIPAddress(void);

/**
 * Returns a 32 bit random number.
 */
uint32_t LLNET_getRandomNumber(void);

/**
 * Set device netmask
 */
void LLNET_setDeviceNetmask(uint8_t* deviceNetmask);

/**
 * Device netmask
 */
uint8_t* LLNET_getDeviceNetmask(void);

/**
 * Set device IP Gateway address
 */
void LLNET_setDeviceGatewayIPAddr(uint8_t* gatewayIPAddr);

/**
 * Device IP Gateway address
 */
uint8_t* LLNET_getDeviceGatewayIPAddr(void);

/**
 * Set DNS host IP address
 */
void LLNET_setDNSHostIPAddr(uint8_t* dnsIPAddr);

/**
 * DNS host IP address
 */
uint8_t* LLNET_getDNSHostIPAddr(void);

/**
 * MAC address
 */
uint8_t* LLNET_getMACAddr(void);

/**
 * Net Embedded task priority
 */
extern int16_t 	NET_EMBEDDED_TASK_PRIORITY;

/**
 * Net Embedded available buffer size
 */
extern int32_t NET_EMBEDDED_AVAILABLE_BUFFER_SIZE;

/**
 * Net Embedded available buffer
 */
extern uint8_t NET_EMBEDDED_AVAILABLE_BUFFER[];

#endif // LLNET_CFG_H
