/*
 * C
 *
 * Copyright 2014-2015 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

#ifndef LLNET_UCTCPIP_H
#define LLNET_UCTCPIP_H

#include <stdint.h>
#include <IF/net_if.h>

// network interface name max char number
#define IF_NAME_MAX_CHAR 20

// network interface structure
struct networkInterface{
	NET_IF_NBR ifNbr;
	uint8_t name[IF_NAME_MAX_CHAR];
	uint8_t isLoopback;
	uint8_t isPointToPoint;
	uint8_t multicastSupport;

};
typedef struct networkInterface LLNET_networkInterface_t;

/**
 * Notify Net Embedded mechanism that a network interface is down. This
 * function will unblock all socket operations on this network interface.
 */
void LLNET_uCTCPIP_notifyIFDown(void);

/**
 * Return the current network interface number.
 * @return the current network interface number
 */
uint8_t LLNET_uCTCPIP_getCurrentIFNbr(void);

/**
 * Set the current network interface number.
 * @param the current network interface number
 */
void LLNET_uCTCPIP_setCurrentIFNbr(uint8_t if_nbr);

/**
 * Return the number of network interface.
 * @return number of network interface
 */
int32_t LLNET_uCTCPIP_getIFNbr(void);

/**
 * Register a new network interface.
 * @param llnet_if the new network interface
 * @return 1 if successful, false otherwise
 */
uint8_t LLNET_uCTCPIP_registerNewIF(LLNET_networkInterface_t llnet_if);

/**
 * Return the network interface associated to the logical id, NULL otherwise.
 * @param llnet_if_id network interface id
 * @return the network interface associated to the logical id, NULL if not found
 */
LLNET_networkInterface_t* LLNET_uCTCPIP_getIF(int32_t llnet_if_id);

#endif // LLNET_UCTCPIP_H
