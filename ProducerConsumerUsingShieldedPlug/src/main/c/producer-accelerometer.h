/*
 * C
 *
 * Copyright 2015 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY. Use is subject to license terms.
 */
#ifndef PRODUCER_ACCELEROMETER_H
#define PRODUCER_ACCELEROMETER_H

#include <stdint.h>
#include "producer.h"

typedef struct PRODUCER_accelerometer_t{
	PRODUCER_t super;
	int8_t sensor_ID;
}PRODUCER_accelerometer_t;

//struct PRODUCER_t{


void PRODUCER_accelerometer_init_ALL(void);
void PRODUCER_accelerometer_init(PRODUCER_accelerometer_t* accelerometer);

void PRODUCER_accelerometer_produce_adapter(PRODUCER_t* pProducer);
void PRODUCER_accelerometer_produce(PRODUCER_accelerometer_t* pProducer);

#endif // PRODUCER_ACCELEROMETER_H
