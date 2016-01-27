/*
 * C
 *
 * Copyright 2016 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY. Use is subject to license terms.
 */

/* Includes ------------------------------------------------------------------*/

#include "sni-producer-accelerometer.h"
#include "sni-producer-messenger.h"

//== "static" factory function
void SNI_PRODUCER_init_factory(void)
{
	//since producers are passed as FreeRTOS task parameters, they must live for the lifetime of the task (i.e. forever)
	//we therefore declare them as static
	static SNI_PRODUCER_accelerometer_t accelerometerProducer1 = {
			{
					"PRODUCER_Accelerometer_Task_1",
					1600,
					&SNI_PRODUCER_accelerometer_configure_adapter,
					&SNI_PRODUCER_accelerometer_produce_adapter
			},
			1
	};

	static SNI_PRODUCER_accelerometer_t accelerometerProducer2 = {
			{
					"PRODUCER_Accelerometer_Task_2",
					1500,
					&SNI_PRODUCER_accelerometer_configure_adapter,
					&SNI_PRODUCER_accelerometer_produce_adapter
			},
			2
	};

	static SNI_PRODUCER_messenger_t messenger1 = {
			{
					"PRODUCER_Messenger_Task_1",
					1100,
					&SNI_PRODUCER_messenger_configure_adapter,
					&SNI_PRODUCER_messenger_produce_adapter
			},
			11,
			0,
			0
	};

		static SNI_PRODUCER_messenger_t messenger2 = {
			{
					"PRODUCER_Messenger_Task_2",
					1800,
					&SNI_PRODUCER_messenger_configure_adapter,
					&SNI_PRODUCER_messenger_produce_adapter
			},
			22,
			1,
			0
	};

	SNI_PRODUCER_messenger_init(&messenger1);
	SNI_PRODUCER_accelerometer_init(&accelerometerProducer1);
	SNI_PRODUCER_accelerometer_init(&accelerometerProducer2);
	SNI_PRODUCER_messenger_init(&messenger2);

}
