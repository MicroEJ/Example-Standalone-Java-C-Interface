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
#include <sp.h>
#include "sp-producer.h"

typedef struct SP_PRODUCER_accelerometer_t{
	SP_PRODUCER_t super;						//a SP_PRODUCER_accelerometer_t "Is-a" SP_PRODUCER_t

	int8_t sensor_ID;

	//== "private" data
	int8_t shieldedPlugDatabaseId;			//shielded plug database ID
	ShieldedPlug pDatabase;					//pointer to shielded plug database - set by SP_PRODUCER_accelerometer_init(...)

}SP_PRODUCER_accelerometer_t;

//== constructor
void SP_PRODUCER_accelerometer_init(SP_PRODUCER_accelerometer_t* accelerometer);

//== functions adapters
void SP_PRODUCER_accelerometer_configure_adapter(SP_PRODUCER_t* pProducer);
void SP_PRODUCER_accelerometer_produce_adapter(SP_PRODUCER_t* pProducer);

//== functions implementations
void SP_PRODUCER_accelerometer_configure(SP_PRODUCER_accelerometer_t* pProducer);
void SP_PRODUCER_accelerometer_produce(SP_PRODUCER_accelerometer_t* pProducer);

#endif // SP_PRODUCER_ACCELEROMETER_H
