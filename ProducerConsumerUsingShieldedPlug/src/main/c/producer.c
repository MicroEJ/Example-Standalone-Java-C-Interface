/*
 * C
 *
 * Copyright 2015 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY. Use is subject to license terms.
 */

/* Includes ------------------------------------------------------------------*/

#include "producer.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Defines -------------------------------------------------------------------*/

#define PRODUCER_STACK_SIZE 4096
#define PRODUCER_TASK_PRIORITY      ( 3 ) /** Should be > tskIDLE_PRIORITY & < configTIMER_TASK_PRIORITY */
#define PRODUCER_TASK_STACK_SIZE     PRODUCER_STACK_SIZE/4


/* API -----------------------------------------------------------------------*/

void PRODUCER_init(PRODUCER_t* pProducer)
{
	if ( NULL != pProducer )
	{
		// create the PRODUCER task
		xTaskCreate(PRODUCER_taskBody, NULL, PRODUCER_TASK_STACK_SIZE, (void*) pProducer, PRODUCER_TASK_PRIORITY, NULL);
	}
	else
	{
		printf("%s error : pointer to productionFunction is NULL !\n",__PRETTY_FUNCTION__);
	}
}

void PRODUCER_taskBody(void* arg)
{
	if ( NULL != arg )
	{
		PRODUCER_t* pProducer = (PRODUCER_t*) arg;	
		const portTickType xDelay = pProducer->productionPeriodInMS / portTICK_RATE_MS;

		if ( NULL != pProducer->productionFunction )
		{
			ShieldedPlug database = SP_getDatabase(pProducer->shieldedPlugDatabaseId);

			pProducer->pDatabase = &database;
			
			int32_t SPS_size = SP_getSize(database);

			printf("%s SPS_size %d\n",__PRETTY_FUNCTION__,SPS_size);

			int32_t SPS_length_block_0 = SP_getLength(database,0);

			printf("%s SPS_length_block_0 %d\n",__PRETTY_FUNCTION__,SPS_length_block_0);
			vTaskDelay(xDelay);
			
			for(;;)
			{
				pProducer->productionFunction(pProducer);
				vTaskDelay(xDelay);			
			}
		}
		else
		{
			printf("%s error : pointer to productionFunction is NULL !\n",__PRETTY_FUNCTION__);
		}
	}
	else
	{
		printf("%s error : pointer to publisher is NULL !\n",__PRETTY_FUNCTION__);
	}
}

