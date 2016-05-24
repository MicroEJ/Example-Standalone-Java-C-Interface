/*
 * C
 *
 * Copyright 2016 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

#include "LLNET_Util.h"
#include <string.h>

#define MAC_ADDR_SEPARATOR	':'
#define MAC_ADDR_STR_SIZE   17

static int32_t convertHexStringToInt(uint8_t* str, int32_t offset, int32_t length);

uint32_t computeCRC24(const char* buffer, int32_t length) {
	uint32_t result = 0;
    uint32_t polynom = 0x005A6000; // BTLE polynom
	uint32_t i,j;
    for(i = 0; i < length; ++i) {
        char b = buffer[i];
        for (j = 0; j < 8; ++j) {
            int bitSet = (result ^ b) & 1;
            result = result >> 1;
            b = b >> 1;
            if (bitSet) {
                result |= 1 << 23;
                result = result ^ polynom;
            }
        }
    }
    return result;
}

/**
 * Parse a MAC address in string format and return the associated byte array.
 * Return an array of 0 if an error occurred.
 * @param str MAC address in string format
 * @param dst destination MAC address array pointer
 * @param dstOffset destination MAC address array pointer offset
 * @param dstLength destination MAC address array length
 */
void parseMacAddr(uint8_t* str, uint8_t* dst, int32_t dstOffset, int32_t dstLength){
	uint32_t count = 0;
	int32_t dstIndex = 0;
	int32_t strLen = strlen((const char*)str);
	if(strLen != MAC_ADDR_STR_SIZE){
		return;
	}
	for(int32_t i = -1; ++i < strLen;){
		if(str[i] == MAC_ADDR_SEPARATOR){
			if((count == 2) && (dstIndex < (dstLength-1))){
				dst[(dstIndex++) + dstOffset] = (uint8_t)convertHexStringToInt(&str[i-2], 0, 2);
				count = 0;
			}else{
				// parse error
				for(int32_t j = -1; ++j < dstLength;){
					dst[dstOffset + j] = 0;
				}
				return;
			}
		}else{
			++count;
		}
	}
	// last byte
	dst[dstIndex + dstOffset] = (uint8_t)convertHexStringToInt(&str[strLen-2], 0, 2);
}

/**
 * Convert an hexadecimal string (without '0x' prefix) to an integer
 * @param str a hexadecimal string
 * @param offset hexadecimal string offset
 * @param length hexadecimal string length
 * @return integer conversion of the hexadecimal string
 */
static int32_t convertHexStringToInt(uint8_t* str, int32_t offset, int32_t length){
	int32_t res = 0;
	for(int32_t i = -1; ++i < length;){
		int32_t tmp = str[offset + i] - '0';
		if(tmp > 10){
			tmp = str[offset + i] - 'A';
			if(tmp > 10){
				tmp = str[offset + i] - 'a';
			}
			tmp += 10;
		}
		res += tmp  << (4 * (length - 1 - i));
	}
	return res;
}
