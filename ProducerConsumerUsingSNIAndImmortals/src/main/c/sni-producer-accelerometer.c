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


#define PRODUCER_ACCELEROMETER_QUEUE_ID					7 //this queue ID should not be reused for a message queue holding anything else than accelerometer data
#define PRODUCER_ACCELEROMETER_QUEUE_MAX_ITEMS 	20
#define PRODUCER_ACCELEROMETER_QUEUE_ITEM_SIZE 	4


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

	xQueueHandle producerQueue = xQueueCreate(PRODUCER_ACCELEROMETER_QUEUE_MAX_ITEMS,PRODUCER_ACCELEROMETER_QUEUE_ITEM_SIZE);
	LLQueue_registerQueue(PRODUCER_ACCELEROMETER_QUEUE_ID,producerQueue,PRODUCER_ACCELEROMETER_QUEUE_ITEM_SIZE,PRODUCER_ACCELEROMETER_QUEUE_MAX_ITEMS);

}

void SNI_PRODUCER_accelerometer_produce(SNI_PRODUCER_accelerometer_t* pProducer)
{
	Accelerometer_data_t data = Accelerometer_data_generate_sample(pProducer->sensor_ID);

	if ( QUEUE_WRITE_OK == LLQueue_write(PRODUCER_ACCELEROMETER_QUEUE_ID,(jbyte*)(&data)) )
	{
		char dataAsString[ACCELEROMETER_DATA_MAX_STRING_LENGTH];
		Accelerometer_data_toString(&data,dataAsString);
		printf("+%s\n",dataAsString);
		fflush(stdout);
	}
	else
	{
		printf("%s error writing data \n",__PRETTY_FUNCTION__);
	}
}
