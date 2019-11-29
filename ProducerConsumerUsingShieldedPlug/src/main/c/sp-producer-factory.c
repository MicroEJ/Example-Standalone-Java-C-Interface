/*
 * C
 *
 * Copyright 2016-2019 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 *
 */

/* Includes ------------------------------------------------------------------*/

#include "accelerometer-data.h"
#include "sp-producer-accelerometer.h"
#include <stdio.h> //only to retrieve definition of NULL

//must match the IDs used in the shielded plug database definition file
#define ACCELEROMETER_SP_DATABASE_ID 0
#define ACCELEROMETER_SP_DATABASE_FIELD_ID_ACCELEROMETER_1 0
#define ACCELEROMETER_SP_DATABASE_FIELD_ID_ACCELEROMETER_2 1
#define ACCELEROMETER_SP_DATABASE_FIELD_ID_ACCELEROMETER_3 2

//== "static" factory function
void SP_PRODUCER_init_factory(void)
{
	//since producers are passed as FreeRTOS task parameters, they must live for the lifetime of the task (i.e. forever)
	//we therefore declare them as static
	static SP_PRODUCER_accelerometer_t accelerometerProducer1 = {
			"PRODUCER_Accelerometer_Task_1",
			1000,
			ACCELEROMETER_SP_DATABASE_FIELD_ID_ACCELEROMETER_1,
			ACCELEROMETER_SP_DATABASE_ID,
			NULL
	};

	static SP_PRODUCER_accelerometer_t accelerometerProducer2 = {
			"PRODUCER_Accelerometer_Task_2",
			1500,
			ACCELEROMETER_SP_DATABASE_FIELD_ID_ACCELEROMETER_2,
			ACCELEROMETER_SP_DATABASE_ID,
			NULL
	};

	SP_PRODUCER_accelerometer_init(&accelerometerProducer1);
	SP_PRODUCER_accelerometer_init(&accelerometerProducer2);
}
