/*
 * C
 *
 * Copyright 2016 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY. Use is subject to license terms.
 */
#ifndef MESSENGER_DATA_H
#define MESSENGER_DATA_H

#define MESSENGER_DATA_MAX_STRING_LENGTH 253 // = 255 - length of (sender_ID + content_Length) )

#include <stdint.h>
#include <sni.h>

typedef struct Messenger_data_t{
	int8_t sender_ID;
	int8_t content_Length;
	jbyte content[MESSENGER_DATA_MAX_STRING_LENGTH];
} Messenger_data_t;

Messenger_data_t Messenger_data_get_next_line(int8_t senderID, int32_t* fileIndex, int32_t* lineIndex);
void Messenger_data_toString(const Messenger_data_t* data, char * output);

#endif // MESSENGER_DATA_H
