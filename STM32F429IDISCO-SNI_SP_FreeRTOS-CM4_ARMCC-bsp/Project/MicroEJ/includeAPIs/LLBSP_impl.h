/* 
 * Copyright 2011-2014 IS2T. All rights reserved.
 * Modification and distribution is permitted under certain conditions.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */
/* 
 * AUTOGENERATED by COMP-ICETEA-ARMCC-ARM - 1.0.0 - DO NOT EDIT IT
 */
/* 
 * Implementation header file. Shall only be included by client implementation C files.
 */
#include <stdint.h>
#include <intern/LLBSP_impl.h>
// --------------------------------------------------------------------------------
// -                      Functions that must be implemented                      -
// --------------------------------------------------------------------------------

/**
 * @return 1 if the given pointer is in a read only memory (eg. flash), 0 otherwise.
 */
uint8_t LLBSP_IMPL_isInReadOnlyMemory(void* ptr);

/**
 * <p>
 * Writes the character <code>c</code>, cast to an unsigned char, to stdout stream.
 * </p>
 * <p>
 * This function is used by the default implementation of the Java <code>System.out</code>.
 * </p>
 */
void LLBSP_IMPL_putchar(int32_t c);