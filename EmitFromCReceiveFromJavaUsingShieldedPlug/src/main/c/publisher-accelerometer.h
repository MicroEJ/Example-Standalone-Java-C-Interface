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


typedef struct PUBLISHER_accelerometer_data_t{
	int8_t sensor_ID;
	int8_t x;
	int8_t y;
	int8_t z;
} PUBLISHER_accelerometer_data_t;

typedef struct PUBLISHER_accelerometer_t{
	PUBLISHER_t super;
	int8_t sensor_ID;
}PUBLISHER_accelerometer_t;

//struct PUBLISHER_t{


void PUBLISHER_accelerometer_init_ALL(void);
void PUBLISHER_accelerometer_init(PUBLISHER_accelerometer_t* accelerometer);

void PUBLISHER_accelerometer_publish(PUBLISHER_t* pPublisher);
void PUBLISHER_accelerometer_publish_override(PUBLISHER_accelerometer_t* pPublisher);

#endif // PUBLISHER_ACCELEROMETER_H
