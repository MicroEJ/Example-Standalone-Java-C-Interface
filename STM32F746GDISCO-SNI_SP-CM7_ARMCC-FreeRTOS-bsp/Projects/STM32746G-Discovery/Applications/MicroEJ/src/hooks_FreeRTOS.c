/*
 * C
 *
 * Copyright 2013-2016 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

/*
 * This file implements the application hooks functions needed by FreeRTOS.
 * These functions trace problems that occurs during execution.
 * Their names are defined by FreeRTOS
 * and must be implemented (here) or desactivated (in ConfigFreeRTOS.h).
 *
 * See "Hook Functions [More Advanced]" on FreeRTOS Website.
 * http://www.freertos.org/a00016.hmtl
 */

/* Includes ------------------------------------------------------------------*/

#include <stdio.h>
#include <stdint.h>
#include "FreeRTOSConfig.h"

/* Public functions ----------------------------------------------------------*/

void vApplicationStackOverflowHook(void)
{
    while(1)
    {} // Trap when a stack overflow occurs
}

void vApplicationMallocFailedHook(void)
{
    while(1)
    {} // Trap when a call to malloc fails
}

#ifdef DEBUG_MALLOC
static int32_t mallocSpaceUsed = 0;

void traceMallocDebug(void * pvAddress, size_t uiSize)
{
	mallocSpaceUsed += uiSize;
	printf("%d bytes used of %d heap size\n", mallocSpaceUsed, configTOTAL_HEAP_SIZE);
}

void traceFreeDebug(void * pvAddress, size_t uiSize)
{
	mallocSpaceUsed -= uiSize;
	printf("%d bytes used of %d heap size\n", mallocSpaceUsed, configTOTAL_HEAP_SIZE);
}
#endif
