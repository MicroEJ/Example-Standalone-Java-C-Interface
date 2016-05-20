/*
 * C
 *
 * Copyright 2015-2016 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */
#ifndef LLNET_SOCKETCHANNEL_LWIP_H
#define LLNET_SOCKETCHANNEL_LWIP_H

#ifdef __cplusplus
	extern "C" {
#endif

int32_t SocketChanel_Address(int32_t fd, int8_t* name, uint8_t localAddress);

int32_t SocketChanel_Port(int32_t fd, uint8_t localPort);

#ifdef __cplusplus
	}
#endif

#endif	// LLNET_SOCKETCHANNEL_LWIP_H
