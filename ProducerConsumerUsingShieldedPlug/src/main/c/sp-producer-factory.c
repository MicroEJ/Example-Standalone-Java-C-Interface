/*
 * C
 *
 * Copyright 2015 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY. Use is subject to license terms.
 */

/* Includes ------------------------------------------------------------------*/

#include "accelerometer-data.h"
#include "sp-producer-accelerometer.h"
#include <stdio.h> //only to retrieve definition of NULL

//== "static" factory function
void SP_PRODUCER_init_factory(void)
{
	//since producers are passed as FreeRTOS task parameters, they must live for the lifetime of the task (i.e. forever)
	//we therefore declare them as static
	static SP_PRODUCER_accelerometer_t accelerometerProducer1 = {
			{
					"PRODUCER_Accelerometer_Task_1",
					1000,
					&SP_PRODUCER_accelerometer_configure_adapter,
					&SP_PRODUCER_accelerometer_produce_adapter
			},
			1,
			ACCELEROMETER_SP_DATABASE_ID,
			NULL
	};

	static SP_PRODUCER_accelerometer_t accelerometerProducer2 = {
			{
					"PRODUCER_Accelerometer_Task_2",
					1500,
					&SP_PRODUCER_accelerometer_configure_adapter,
					&SP_PRODUCER_accelerometer_produce_adapter
			},
			2,
			ACCELEROMETER_SP_DATABASE_ID,
			NULL
	};

	SP_PRODUCER_accelerometer_init(&accelerometerProducer1);
	SP_PRODUCER_accelerometer_init(&accelerometerProducer2);
}
