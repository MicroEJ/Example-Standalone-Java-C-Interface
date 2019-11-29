/*
 * C
 *
 * Copyright 2016-2019 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 *
 */
#ifndef SP_PRODUCER_ACCELEROMETER_H
#define SP_PRODUCER_ACCELEROMETER_H

#include <stdint.h>
#include <sp.h>

typedef struct SP_PRODUCER_accelerometer_t{
	const signed char name[32];								//name of the producing task (useful for debugging purposes)
	int16_t productionPeriodInMS;							//period (in Milliseconds) for producing data

	int8_t sensor_ID;

	//== "private" data
	int8_t shieldedPlugDatabaseId;			//shielded plug database ID
	ShieldedPlug pDatabase;					//pointer to shielded plug database - set by SP_PRODUCER_accelerometer_init(...)

}SP_PRODUCER_accelerometer_t;

//== constructor
void SP_PRODUCER_accelerometer_init(SP_PRODUCER_accelerometer_t* accelerometer);

#endif // SP_PRODUCER_ACCELEROMETER_H
