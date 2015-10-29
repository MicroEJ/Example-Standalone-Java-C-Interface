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
#include "sp-producer.h"

typedef struct PRODUCER_accelerometer_t{
	PRODUCER_t super;						//a PRODUCER_accelerometer_t "Is-a" PRODUCER_t

	int8_t sensor_ID;

	//== "private" data
	int8_t shieldedPlugDatabaseId;			//shielded plug database ID
	ShieldedPlug pDatabase;					//pointer to shielded plug database - set by PRODUCER_accelerometer_init(...)

}PRODUCER_accelerometer_t;

void PRODUCER_accelerometer_init(PRODUCER_accelerometer_t* accelerometer);

//== functions adapters
void PRODUCER_accelerometer_configure_adapter(PRODUCER_t* pProducer);
void PRODUCER_accelerometer_produce_adapter(PRODUCER_t* pProducer);

//== functions implementations
void PRODUCER_accelerometer_configure(PRODUCER_accelerometer_t* pProducer);
void PRODUCER_accelerometer_produce(PRODUCER_accelerometer_t* pProducer);

//== "static" factory function
void PRODUCER_accelerometer_init_factory(void);

#endif // SP_PRODUCER_ACCELEROMETER_H
