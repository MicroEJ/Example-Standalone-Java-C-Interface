/*
 * C
 *
 * Copyright 2016 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

#ifndef  LLNET_UTIL_H
#define  LLNET_UTIL_H

#include <stdint.h>

uint32_t computeCRC24(const char* buffer, int32_t length);
void parseMacAddr(uint8_t* str, uint8_t* dst, int32_t dstOffset, int32_t dstLength);

#endif // LLNET_UTIL_H
