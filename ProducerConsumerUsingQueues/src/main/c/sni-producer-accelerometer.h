/*
 * C
 *
 * Copyright 2016-2019 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 *
 */
#ifndef SP_PRODUCER_ACCELEROMETER_H
#define SP_PRODUCER_ACCELEROMETER_H

#include <sni.h>
#include <stdint.h>

typedef struct SNI_PRODUCER_accelerometer_t{
	const signed char name[32];							//name of the producing task (useful for debugging purposes)
	int16_t productionPeriodInMS;							//period (in Milliseconds) for producing data
	int8_t sensor_ID;
}SNI_PRODUCER_accelerometer_t;

//== static service initialization method, must be called once before production begins
jboolean SNI_PRODUCER_accelerometer_queue_init(void);

//== constructor
void SNI_PRODUCER_accelerometer_init(SNI_PRODUCER_accelerometer_t* accelerometer);

//== SNI function
jint Java_com_microej_example_java2c_ProducerConsumerExample_getAccelerometerQueuePtr(void);

#endif // SP_PRODUCER_ACCELEROMETER_H
