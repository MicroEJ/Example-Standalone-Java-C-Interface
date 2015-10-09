/*
 * C
 *
 * Copyright 2012-2014 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

#ifndef _INTERRUPTS
#define _INTERRUPTS

#include "microej.h"

uint8_t isInInterrupt(void);
void enterInterrupt(void);
void leaveInterrupt(void);

#endif
