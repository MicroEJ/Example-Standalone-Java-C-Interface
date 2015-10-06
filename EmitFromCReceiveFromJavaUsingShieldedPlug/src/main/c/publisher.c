/*
 * C
 *
 * Copyright 2015 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY. Use is subject to license terms.
 */

/* Includes ------------------------------------------------------------------*/

#include "publisher.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Defines -------------------------------------------------------------------*/

#define PUBLISHER_STACK_SIZE 4096
#define PUBLISHER_TASK_PRIORITY      ( 3 ) /** Should be > tskIDLE_PRIORITY & < configTIMER_TASK_PRIORITY */
#define PUBLISHER_TASK_STACK_SIZE     PUBLISHER_STACK_SIZE/4


/* API -----------------------------------------------------------------------*/

void PUBLISHER_init(PUBLISHER_t* pPublisher)
{
	if ( NULL != pPublisher )
	{
		// create the PUBLISHER task
		xTaskCreate(PUBLISHER_taskBody, NULL, PUBLISHER_TASK_STACK_SIZE, (void*) pPublisher, PUBLISHER_TASK_PRIORITY, NULL);
	}
	else
	{
		printf("%s error : pointer to publicationFunction is NULL !\n",__PRETTY_FUNCTION__);
	}
}

void PUBLISHER_taskBody(void* arg)
{
	if ( NULL != arg )
	{
		PUBLISHER_t* pPublisher = (PUBLISHER_t*) arg;	
		const portTickType xDelay = pPublisher->publicationPeriodInMS / portTICK_RATE_MS;

		if ( NULL != pPublisher->publicationFunction )
		{
			ShieldedPlug database = SP_getDatabase(pPublisher->shieldedPlugDatabaseId);

			pPublisher->pDatabase = &database;
			
			int32_t SPS_size = SP_getSize(database);

			printf("%s SPS_size %d\n",__PRETTY_FUNCTION__,SPS_size);

			int32_t SPS_length_block_0 = SP_getLength(database,0);

			printf("%s SPS_length_block_0 %d\n",__PRETTY_FUNCTION__,SPS_length_block_0);
			vTaskDelay(xDelay);
			
			for(;;)
			{
				pPublisher->publicationFunction(pPublisher);
				vTaskDelay(xDelay);			
			}
		}
		else
		{
			printf("%s error : pointer to publicationFunction is NULL !\n",__PRETTY_FUNCTION__);
		}
	}
	else
	{
		printf("%s error : pointer to publisher is NULL !\n",__PRETTY_FUNCTION__);
	}
}

