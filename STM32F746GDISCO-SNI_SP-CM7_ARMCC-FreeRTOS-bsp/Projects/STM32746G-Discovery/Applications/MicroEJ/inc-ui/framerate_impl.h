/*
 * C
 *
 * Copyright 2014-2015 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

/*
 * Implement the *_impl* functions according the CPU/OS/Compiler
 */

#ifndef _FRAMERATE_IMPL
#define _FRAMERATE_IMPL

/* Includes ------------------------------------------------------------------*/

#include "framerate.h"

/* API -----------------------------------------------------------------------*/

#ifdef FRAMERATE_ENABLED

/*
 * Framerate task have just to call this function
 */
void framerate_task_work(void);

/*
 * Create and start a framerate task
 */
int32_t framerate_impl_start_task(void);

/*
 * Sleep the framerate task
 */
void framerate_impl_sleep(uint32_t ms);

#endif	// FRAMERATE_ENABLED

#endif	// _FRAMERATE_IMPL
