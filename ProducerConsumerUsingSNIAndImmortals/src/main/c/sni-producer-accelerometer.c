/*
 * C
 *
 * Copyright 2016 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY. Use is subject to license terms.
 *
 */

/* Includes ------------------------------------------------------------------*/

#include "sni-producer-accelerometer.h"
#include "accelerometer-data.h"
#include "LLNativeQueueService.h"
#include "queue.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


#define PRODUCER_ACCELEROMETER_QUEUE_MAX_ITEMS 	20
#define PRODUCER_ACCELEROMETER_QUEUE_ITEM_SIZE 	4


xQueueHandle accelerometerQueueHandle;
queue_service_descriptor_t accelerometerQueue = {0};

//== service initialization
jboolean SNI_PRODUCER_accelerometer_queue_init(void)
{
	printf("%s\n",__PRETTY_FUNCTION__);

	jboolean result = JFALSE;

	if ( 0 == accelerometerQueueHandle )
	{
		accelerometerQueueHandle = xQueueCreate(PRODUCER_ACCELEROMETER_QUEUE_MAX_ITEMS, PRODUCER_ACCELEROMETER_QUEUE_ITEM_SIZE);
		if( 0 == accelerometerQueueHandle )
		{
			// Queue was not created and must not be used.
		}
		else
		{
			result = LLQueue_init(&accelerometerQueue, accelerometerQueueHandle, PRODUCER_ACCELEROMETER_QUEUE_ITEM_SIZE, PRODUCER_ACCELEROMETER_QUEUE_MAX_ITEMS);
		}
	}
	else
	{
		//assume the queue has already been initialized by another producer ?
		result = JTRUE;
	}
	return result;
}

//== constructor
void SNI_PRODUCER_accelerometer_init(SNI_PRODUCER_accelerometer_t* pAccelerometer)
{
	if ( NULL != pAccelerometer )
	{
		SNI_PRODUCER_init(&pAccelerometer->super);
		printf("%s sensor_ID : %d\n",__PRETTY_FUNCTION__,pAccelerometer->sensor_ID);
	}
}

//== production function adapter
void SNI_PRODUCER_accelerometer_produce_adapter(SNI_PRODUCER_t* pProducer)
{
	if ( NULL != pProducer )
	{
		SNI_PRODUCER_accelerometer_produce((SNI_PRODUCER_accelerometer_t*) pProducer);
	}
}

//== actual production function
void SNI_PRODUCER_accelerometer_produce(SNI_PRODUCER_accelerometer_t* pProducer)
{
	Accelerometer_data_t data = Accelerometer_data_generate_sample(pProducer->sensor_ID);

	if ( QUEUE_SERVICE_OK == LLQueue_write(&accelerometerQueue,(jbyte*)(&data)) )
	{
		char dataAsString[ACCELEROMETER_DATA_MAX_STRING_LENGTH];
		Accelerometer_data_toString(&data,dataAsString);
		printf("+%s\n",dataAsString);
	}
	else
	{
		printf("%s error writing data \n",__PRETTY_FUNCTION__);
	}
}

//== SNI function
jint Java_com_microej_examples_java2c_AccelerometerData_getQueuePtr(void)
{
	return (jint)&accelerometerQueue;
}
