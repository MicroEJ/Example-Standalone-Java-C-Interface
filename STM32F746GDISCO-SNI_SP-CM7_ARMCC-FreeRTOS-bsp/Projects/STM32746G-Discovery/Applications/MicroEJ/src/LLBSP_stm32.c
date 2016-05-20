/*
 * Copyright 2014-2015 IS2T. All rights reserved.
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
#define MEM_SRAM_START (0x20000000)
#define MEM_SRAM_END   (0x2004BFFF)
#define MEM_EXTRAM_START (0xC0000000)
#define MEM_EXTRAM_END   (0xC0800000)

/* Public functions ----------------------------------------------------------*/

// Checks if the given pointer is in a read only memory or not.
uint8_t LLBSP_IMPL_isInReadOnlyMemory(void* ptr)
{
	if(
	 ((uint32_t)ptr >= MEM_SRAM_START && (uint32_t)ptr < MEM_SRAM_END) ||
         ((uint32_t)ptr >= MEM_EXTRAM_START && (uint32_t)ptr < MEM_EXTRAM_END)
	)
		return MICROEJ_FALSE;
	else
		return MICROEJ_TRUE;
}

/**
 * Writes the character <code>c</code>, cast to an unsigned char, to stdout stream.
 * This function is used by the default implementation of the Java <code>System.out</code>.
 */
void LLBSP_IMPL_putchar(int32_t c)
{
	putchar(c);
}
