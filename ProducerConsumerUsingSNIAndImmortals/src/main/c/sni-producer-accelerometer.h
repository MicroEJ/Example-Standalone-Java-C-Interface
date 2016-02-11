/*
 * C
 *
 * Copyright 2016 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY. Use is subject to license terms.
 *
 */
#ifndef SP_PRODUCER_ACCELEROMETER_H
#define SP_PRODUCER_ACCELEROMETER_H

#include <stdint.h>
#include "sni-producer.h"

typedef struct SNI_PRODUCER_accelerometer_t{
	SNI_PRODUCER_t super;						//a SNI_PRODUCER_accelerometer_t "Is-a" SNI_PRODUCER_t
	int8_t sensor_ID;
}SNI_PRODUCER_accelerometer_t;

//== static service initialization method, must be called once before production begins
jboolean SNI_PRODUCER_accelerometer_queue_init(void);

//== constructor
void SNI_PRODUCER_accelerometer_init(SNI_PRODUCER_accelerometer_t* accelerometer);

//== functions adapters
void SNI_PRODUCER_accelerometer_produce_adapter(SNI_PRODUCER_t* pProducer);

//== functions implementations
void SNI_PRODUCER_accelerometer_produce(SNI_PRODUCER_accelerometer_t* pProducer);

jint Java_com_microej_examples_java2c_AccelerometerData_getQueuePtr(void);

#endif // SP_PRODUCER_ACCELEROMETER_H
