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

#include "sni.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SNI_PRODUCER_TASK_PRIORITY      ( 3 ) /** Should be > tskIDLE_PRIORITY & < configTIMER_TASK_PRIORITY */
#define SNI_PRODUCER_TASK_STACK_SIZE    128

#define PRODUCER_ACCELEROMETER_QUEUE_MAX_ITEMS 	20
#define PRODUCER_ACCELEROMETER_QUEUE_ITEM_SIZE 	4

xQueueHandle accelerometerQueueHandle;
queue_service_descriptor_t accelerometerQueue = {0};

//==== "private" method(s) declaration
void _SNI_PRODUCER_accelerometer_taskBody(void* arg);
void _SNI_PRODUCER_accelerometer_produce(SNI_PRODUCER_accelerometer_t* pProducer);

//==== "public" method(s) definition

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
void SNI_PRODUCER_accelerometer_init(SNI_PRODUCER_accelerometer_t* pProducer)
{
	if ( NULL != pProducer )
	{
		printf("%s sensor_ID : %d\n",__PRETTY_FUNCTION__,pProducer->sensor_ID);
		// create the PRODUCER task		
		xTaskHandle xHandle = NULL;
		portBASE_TYPE xReturn;
		xReturn = xTaskCreate(_SNI_PRODUCER_accelerometer_taskBody, NULL, SNI_PRODUCER_TASK_STACK_SIZE, (void*) pProducer, SNI_PRODUCER_TASK_PRIORITY, xHandle);
		if( xReturn != pdPASS )
		{
			printf("%s error : unable to create task for %s\n",__PRETTY_FUNCTION__, pProducer->name);
		}
	}
	else
	{
		printf("%s error : pointer to productionFunction is NULL !\n",__PRETTY_FUNCTION__);
	}
}

//==== "private" method(s) definition
void _SNI_PRODUCER_accelerometer_taskBody(void* arg)
{
	if ( NULL != arg )
	{
		SNI_PRODUCER_accelerometer_t* pProducer = (SNI_PRODUCER_accelerometer_t*) arg;

		const portTickType xDelay = pProducer->productionPeriodInMS / portTICK_RATE_MS;

		//disable buffering on stdout
		setbuf(stdout,NULL);

		for(;;)
		{
			vTaskDelay(xDelay);
			_SNI_PRODUCER_accelerometer_produce(pProducer);
		}

	}
	else
	{
		printf("%s error : pointer to producer is NULL !\n",__PRETTY_FUNCTION__);
	}
}

//== production function
void _SNI_PRODUCER_accelerometer_produce(SNI_PRODUCER_accelerometer_t* pProducer)
{
	Accelerometer_data_t data = Accelerometer_data_generate_sample(pProducer->sensor_ID);

	//trace for debugging/documentation purposes
	{
		char dataAsString[ACCELEROMETER_DATA_MAX_STRING_LENGTH] = "";
		Accelerometer_data_toString(&data,dataAsString);
		printf("+%s\n",dataAsString);
	}

	if ( QUEUE_SERVICE_OK != LLQueue_write(&accelerometerQueue,(jbyte*)(&data)) )
	{
		printf("%s error writing data \n",__PRETTY_FUNCTION__);
	}
}

//== SNI function
jint Java_com_microej_examples_java2c_ProducerConsumerExample_getAccelerometerQueuePtr(void)
{
	return (jint)&accelerometerQueue;
}
