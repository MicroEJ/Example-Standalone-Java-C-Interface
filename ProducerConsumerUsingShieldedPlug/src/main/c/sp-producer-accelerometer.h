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

typedef struct SP_PRODUCER_accelerometer_t{
	const signed char name[32];								//name of the producing task (useful for debugging purposes)
	int16_t productionPeriodInMS;							//period (in Milliseconds) for producing data

	int8_t sensor_ID;

	//== "private" data
	int8_t shieldedPlugDatabaseId;			//shielded plug database ID
	int8_t shieldedPlugDatabaseFieldId;		//shielded plug database field ID
	ShieldedPlug pDatabase;					//pointer to shielded plug database - set by SP_PRODUCER_accelerometer_init(...)

}SP_PRODUCER_accelerometer_t;

//== constructor
void SP_PRODUCER_accelerometer_init(SP_PRODUCER_accelerometer_t* accelerometer);

#endif // SP_PRODUCER_ACCELEROMETER_H
