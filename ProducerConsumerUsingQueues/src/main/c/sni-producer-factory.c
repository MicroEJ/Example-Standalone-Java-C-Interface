/*
 * C
 *
 * Copyright 2016 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY. Use is subject to license terms.
 */

/* Includes ------------------------------------------------------------------*/

#include "sni-producer-factory.h"
#include "sni-producer-accelerometer.h"

//== "static" factory functions	
void SNI_PRODUCER_init_factory(void)
{
	SNI_PRODUCER_accelerometer_queue_init();

	//since producers are passed as FreeRTOS task parameters, they must live for the lifetime of the task (i.e. forever)
	//we therefore declare them as static
	static SNI_PRODUCER_accelerometer_t accelerometerProducer1 = {
			"PRODUCER_Accelerometer_Task_1",
			1300,
			1
	};

	static SNI_PRODUCER_accelerometer_t accelerometerProducer2 = {
			"PRODUCER_Accelerometer_Task_2",
			1400,
			2
	};


	SNI_PRODUCER_accelerometer_init(&accelerometerProducer1);
	SNI_PRODUCER_accelerometer_init(&accelerometerProducer2);
}
