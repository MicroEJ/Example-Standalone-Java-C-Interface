/*
 * C
 *
 * Copyright 2014-2015 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */
#ifndef  LLBSP_NET_IMPL_H
#define  LLBSP_NET_IMPL_H

#include <stdint.h>

/**
 * Network initialization. Start network interfaces and configure it.
 * @return 0 if no error, error code otherwise.
 */
int32_t  LLBSP_NET_init(void);

/**
 * Uncomment this define to put LLBSP_NET in debug mode
 */
#define LLBSP_NET_DEBUG

/**
 * Network debug trace modes.
 */
#ifdef LLBSP_NET_DEBUG
#define LLBSP_NET_TRACE 	printf
#define LLBSP_NET_ASSERT(...) 	printf(__VA_ARGS__); while(1);
#else
#define LLBSP_NET_TRACE(...) ((void) 0)
#define LLBSP_NET_ASSERT(...) ((void) 0)
#endif

#endif // LLBSP_NET_IMPL_H
