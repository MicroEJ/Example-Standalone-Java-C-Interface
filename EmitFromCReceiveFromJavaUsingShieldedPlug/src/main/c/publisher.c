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

void PUBLISHER_init(const PUBLISHER_t* pPublisher)
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
		const PUBLISHER_t* pPublisher = (const PUBLISHER_t*) arg;	
		const portTickType xDelay = pPublisher->publicationPeriodInMS / portTICK_RATE_MS;

		if ( NULL != pPublisher->publicationFunction )
		{
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

