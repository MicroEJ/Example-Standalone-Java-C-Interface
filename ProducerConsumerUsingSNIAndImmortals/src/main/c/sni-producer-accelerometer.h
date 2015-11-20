/*
 * C
 *
 * Copyright 2015 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY. Use is subject to license terms.
 */
#ifndef SP_PRODUCER_ACCELEROMETER_H
#define SP_PRODUCER_ACCELEROMETER_H

#include <stdint.h>
#include "sni-producer.h"

typedef struct SNI_PRODUCER_accelerometer_t{
	SNI_PRODUCER_t super;						//a SNI_PRODUCER_accelerometer_t "Is-a" SNI_PRODUCER_t

	int8_t sensor_ID;


}SNI_PRODUCER_accelerometer_t;

//== constructor
void SNI_PRODUCER_accelerometer_init(SNI_PRODUCER_accelerometer_t* accelerometer);

//== functions adapters
jboolean SNI_PRODUCER_accelerometer_configure_adapter(SNI_PRODUCER_t* pProducer);
void SNI_PRODUCER_accelerometer_produce_adapter(SNI_PRODUCER_t* pProducer);

//== functions implementations
jboolean SNI_PRODUCER_accelerometer_configure(SNI_PRODUCER_accelerometer_t* pProducer);
void SNI_PRODUCER_accelerometer_produce(SNI_PRODUCER_accelerometer_t* pProducer);

#endif // SP_PRODUCER_ACCELEROMETER_H
