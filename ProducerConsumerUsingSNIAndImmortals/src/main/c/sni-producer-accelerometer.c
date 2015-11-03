/*
 * C
 *
 * Copyright 2015 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY. Use is subject to license terms.
 */

/* Includes ------------------------------------------------------------------*/

#include "sni-producer-accelerometer.h"
#include "accelerometer-data.h"
#include "LLNativeQueueService.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


#define PRODUCER_QUEUE_ID 07
#define PRODUCER_QUEUE_MAX_ITEMS 5
#define PRODUCER_QUEUE_ITEM_SIZE 4


//== constructor
void SNI_PRODUCER_accelerometer_init(SNI_PRODUCER_accelerometer_t* pAccelerometer)
{
	if ( NULL != pAccelerometer )
	{
		SNI_PRODUCER_init(&pAccelerometer->super);
		printf("%s sensor_ID : %d\n",__PRETTY_FUNCTION__,pAccelerometer->sensor_ID);
	}
}

//== functions adapters
void SNI_PRODUCER_accelerometer_configure_adapter(SNI_PRODUCER_t* pProducer)
{
	if ( NULL != pProducer )
	{
		SNI_PRODUCER_accelerometer_configure((SNI_PRODUCER_accelerometer_t*) pProducer);
	}
}

void SNI_PRODUCER_accelerometer_produce_adapter(SNI_PRODUCER_t* pProducer)
{
	if ( NULL != pProducer )
	{
		SNI_PRODUCER_accelerometer_produce((SNI_PRODUCER_accelerometer_t*) pProducer);
	}
}

//== functions implementations
void SNI_PRODUCER_accelerometer_configure(SNI_PRODUCER_accelerometer_t* pProducer)
{
	printf("%s\n",__PRETTY_FUNCTION__);

	xQueueHandle producerQueue = xQueueCreate(PRODUCER_QUEUE_MAX_ITEMS,PRODUCER_QUEUE_ITEM_SIZE);
	LLQueue_registerQueue(PRODUCER_QUEUE_ID,producerQueue,PRODUCER_QUEUE_ITEM_SIZE,PRODUCER_QUEUE_MAX_ITEMS);

}

void SNI_PRODUCER_accelerometer_produce(SNI_PRODUCER_accelerometer_t* pProducer)
{
	//printf("%s\n",__PRETTY_FUNCTION__);

	Accelerometer_data_t data = Accelerometer_data_generate_sample(pProducer->sensor_ID);

	if ( QUEUE_WRITE_OK == LLQueue_write(PRODUCER_QUEUE_ID,(jbyte*)(&data)) ) //TODO error handling
	{
		char dataAsString[ACCELEROMETER_DATA_MAX_STRING_LENGTH];
		Accelerometer_data_toString(&data,dataAsString);
		//printf("+%s\n",dataAsString);
	}
	else
	{
		printf("%s error writing data \n",__PRETTY_FUNCTION__);
	}
}


//== "static" factory function
void SNI_PRODUCER_accelerometer_init_factory(void)
{
	//since producers are passed as FreeRTOS task parameters, they must live for the lifetime of the task (i.e. forever)
	//we therefore declare them as static
	static SNI_PRODUCER_accelerometer_t accelerometerProducer1 = {
			{
					"PRODUCER_Accelerometer_Task_1",
					1000,
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

	SNI_PRODUCER_accelerometer_init(&accelerometerProducer1);
	SNI_PRODUCER_accelerometer_init(&accelerometerProducer2);
}
