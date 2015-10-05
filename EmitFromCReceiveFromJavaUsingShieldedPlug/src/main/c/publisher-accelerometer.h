/*
 * C
 *
 * Copyright 2015 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY. Use is subject to license terms.
 */
#ifndef PUBLISHER_ACCELEROMETER_H
#define PUBLISHER_ACCELEROMETER_H

#include <stdint.h>
#include "publisher.h"


typedef struct PUBLISHER_accelerometer_t{
	int8_t x;
	int8_t y;
	int8_t z;
} PUBLISHER_accelerometer_t;


void PUBLISHER_accelerometer_init(void);

void PUBLISHER_accelerometer_publish(const PUBLISHER_t* pPublisher);

#endif // PUBLISHER_ACCELEROMETER_H
