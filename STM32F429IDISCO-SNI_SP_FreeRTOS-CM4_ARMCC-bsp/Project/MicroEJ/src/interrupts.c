/*
 * C
 *
 * Copyright 2014 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

/* Includes ------------------------------------------------------------------*/

#include "interrupts.h"
#include "microej.h"

/* Globals -------------------------------------------------------------------*/

static volatile uint8_t isInISR = MICROEJ_FALSE;

/* Public functions ----------------------------------------------------------*/

uint8_t isInInterrupt(void)
{
	return isInISR;
}

void enterInterrupt(void)
{
	isInISR = MICROEJ_TRUE;
}

void leaveInterrupt(void)
{
	isInISR = MICROEJ_FALSE;
}
