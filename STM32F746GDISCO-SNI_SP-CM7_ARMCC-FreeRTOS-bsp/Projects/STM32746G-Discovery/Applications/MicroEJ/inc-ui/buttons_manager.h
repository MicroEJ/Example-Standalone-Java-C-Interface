/*
 * C
 *
 * Copyright 2015 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

#ifndef _BUTTONS_MANAGER
#define _BUTTONS_MANAGER

/* Includes ------------------------------------------------------------------*/

#include <stdint.h>
#include "stm32746g_discovery.h"

/* API -----------------------------------------------------------------------*/

void BUTTONS_MANAGER_initialize(void);
void BUTTONS_MANAGER_interrupt(Button_TypeDef Button);
void BUTTONS_MANAGER_enable_interrupts(void);
void BUTTONS_MANAGER_disable_interrupts(void);

#endif
