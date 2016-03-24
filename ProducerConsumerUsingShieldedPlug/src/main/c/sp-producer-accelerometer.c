/*
 * C
 *
 * Copyright 2016 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY. Use is subject to license terms.
 *
 */

/* Includes ------------------------------------------------------------------*/

#include "sp-producer-accelerometer.h"
#include "accelerometer-data.h"

#include "FreeRTOS.h"
#include "task.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Defines -------------------------------------------------------------------*/

#define SP_PRODUCER_STACK_SIZE 2048
#define SP_PRODUCER_TASK_PRIORITY      ( 3 ) /** Should be > tskIDLE_PRIORITY & < configTIMER_TASK_PRIORITY */
#define SP_PRODUCER_TASK_STACK_SIZE     SP_PRODUCER_STACK_SIZE/4


//==== "private" method(s) declaration
void _SP_PRODUCER_taskBody(void* arg);


//== constructor
void SP_PRODUCER_accelerometer_init(SP_PRODUCER_accelerometer_t* pAccelerometer)
{
	if ( NULL != pAccelerometer )
	{
		printf("%s name : %s\n",__PRETTY_FUNCTION__,pAccelerometer->name);
		printf("%s productionPeriodInMS : %d\n",__PRETTY_FUNCTION__,pAccelerometer->productionPeriodInMS);
		printf("%s sensor_ID : %d\n",__PRETTY_FUNCTION__,pAccelerometer->sensor_ID);

		xTaskHandle xHandle;
		portBASE_TYPE xReturn;
		xReturn = xTaskCreate(_SP_PRODUCER_taskBody, NULL, SP_PRODUCER_TASK_STACK_SIZE, (void*) pAccelerometer, SP_PRODUCER_TASK_PRIORITY, xHandle);
		if( xReturn != pdPASS )
		{
			printf("%s error : unable to create task for %s\n",__PRETTY_FUNCTION__, pAccelerometer->name);
		}
	}
	else
	{
		printf("%s error : pointer to pAccelerometer is NULL !\n",__PRETTY_FUNCTION__);
	}
}

//==== "private" method(s) definition
void _SP_PRODUCER_taskBody(void* arg)
{
	if ( NULL != arg )
	{
		SP_PRODUCER_accelerometer_t* pProducer = (SP_PRODUCER_accelerometer_t*) arg;

		const portTickType xDelay = pProducer->productionPeriodInMS / portTICK_RATE_MS;

		SP_PRODUCER_accelerometer_configure(pProducer);

		portTickType xLastWakeTime = xTaskGetTickCount ();
		for(;;)
		{
			vTaskDelayUntil(&xLastWakeTime,xDelay);
			SP_PRODUCER_accelerometer_produce(pProducer);
		}
	}
	else
	{
		printf("%s error : pointer to producer is NULL !\n",__PRETTY_FUNCTION__);
	}
}

void SP_PRODUCER_accelerometer_configure(SP_PRODUCER_accelerometer_t* pProducer)
{
	//ShieldedPlug is a typedef to void*, so the database will outlive the scope of the function
	ShieldedPlug database = SP_getDatabase(pProducer->shieldedPlugDatabaseId);
	pProducer->pDatabase = database;

	int32_t SP_number_of_blocks = SP_getSize(pProducer->pDatabase );
	printf("%s SP_number_of_blocks %d\n",__PRETTY_FUNCTION__,SP_number_of_blocks);
	int32_t SP_block_length_accelerometer = SP_getLength(pProducer->pDatabase,pProducer->shieldedPlugDatabaseFieldId);
	printf("%s SP_block_length_accelerometer %d\n",__PRETTY_FUNCTION__,SP_block_length_accelerometer);
}

void SP_PRODUCER_accelerometer_produce(SP_PRODUCER_accelerometer_t* pProducer)
{
	ShieldedPlug* pDatabase = pProducer->pDatabase;
	if ( NULL != pDatabase )
	{
		Accelerometer_data_t data = Accelerometer_data_generate_sample(pProducer->sensor_ID);
		
		int32_t writeStatus = SP_write(pDatabase,pProducer->shieldedPlugDatabaseFieldId,&data);
		if (SP_SUCCESS == writeStatus )
		{
			char dataAsString[ACCELEROMETER_DATA_MAX_STRING_LENGTH];
			Accelerometer_data_toString(&data,dataAsString);
			printf("+%s\n",dataAsString);
			fflush(stdout);
		}
		else
		{
			printf("%s could not write data. writeStatus : %d\n",__PRETTY_FUNCTION__,writeStatus);
		}
	}
	else
	{
		printf("%s pDatabase is NULL !\n",__PRETTY_FUNCTION__);
	}
}
