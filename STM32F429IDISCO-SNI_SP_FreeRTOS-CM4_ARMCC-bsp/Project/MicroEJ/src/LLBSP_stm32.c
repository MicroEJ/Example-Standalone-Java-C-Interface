/*
 * Copyright 2014 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

/* Includes ------------------------------------------------------------------*/

#include "LLBSP_impl.h"
#include "microej.h"
#include <stdio.h>

/* Defines -------------------------------------------------------------------*/

/**
 * Memory settings on STM32
 */
#define MEM_CODE_START (0)
#define MEM_CODE_END (0x20000000)

/* Public functions ----------------------------------------------------------*/

// Checks if the given pointer is in a read only memory or not.
uint8_t LLBSP_IMPL_isInReadOnlyMemory(void* ptr)
{
	if(
#if MEM_CODE_START != 0
			// avoid warning #186-D: pointless comparison of unsigned integer with zero
			(uint32_t)ptr >= MEM_CODE_START &&
#endif
			(uint32_t)ptr < MEM_CODE_END
	)
		return MICROEJ_TRUE;
	else
		return MICROEJ_FALSE;
}

/**
 * Writes the character <code>c</code>, cast to an unsigned char, to stdout stream.
 * This function is used by the default implementation of the Java <code>System.out</code>.
 */
void LLBSP_IMPL_putchar(int32_t c)
{
	putchar(c);
}
