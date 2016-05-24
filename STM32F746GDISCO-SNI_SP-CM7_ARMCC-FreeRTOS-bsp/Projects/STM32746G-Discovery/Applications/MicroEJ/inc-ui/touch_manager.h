/*
 * C
 *
 * Copyright 2015 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

#ifndef _TOUCH_MANAGER
#define _TOUCH_MANAGER

/* Includes ------------------------------------------------------------------*/

#include <stdint.h>

/* API -----------------------------------------------------------------------*/

void TOUCH_MANAGER_initialize(void);
uint8_t TOUCH_MANAGER_interrupt(void);
uint8_t TOUCH_MANAGER_work(void );

#endif
