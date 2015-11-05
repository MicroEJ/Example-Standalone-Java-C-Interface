/*
 * C
 *
 * Copyright 2015 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY. Use is subject to license terms.
 */
#ifndef MESSENGER_DATA_H
#define MESSENGER_DATA_H

#define MESSENGER_DATA_MAX_STRING_LENGTH 255

#include <stdint.h>

char* Messenger_data_get_next_line(int32_t* fileIndex, int32_t* lineIndex);

#endif // MESSENGER_DATA_H
