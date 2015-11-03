/*
 * C
 *
 * Copyright 2015 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY. Use is subject to license terms.
 */

/* Includes ------------------------------------------------------------------*/

#include "sp-producer.h"

#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Defines -------------------------------------------------------------------*/

#define PRODUCER_STACK_SIZE 4096
#define PRODUCER_TASK_PRIORITY      ( 3 ) /** Should be > tskIDLE_PRIORITY & < configTIMER_TASK_PRIORITY */
#define PRODUCER_TASK_STACK_SIZE     PRODUCER_STACK_SIZE/4

//==== "public" method(s)
//== constructor
void PRODUCER_init(PRODUCER_t* pProducer)
{
	if ( NULL != pProducer )
	{
		printf("%s name : %s\n",__PRETTY_FUNCTION__,pProducer->name);
		printf("%s productionPeriodInMS : %d\n",__PRETTY_FUNCTION__,pProducer->productionPeriodInMS);

		xTaskCreate(_PRODUCER_taskBody, NULL, PRODUCER_TASK_STACK_SIZE, (void*) pProducer, PRODUCER_TASK_PRIORITY, NULL);
	}
	else
	{
		printf("%s error : pointer to productionFunction is NULL !\n",__PRETTY_FUNCTION__);
	}
}

//==== "private" method(s)
void _PRODUCER_taskBody(void* arg)
{
	if ( NULL != arg )
	{
		PRODUCER_t* pProducer = (PRODUCER_t*) arg;	
		const portTickType xDelay = pProducer->productionPeriodInMS / portTICK_RATE_MS;

		if ( NULL != pProducer->configurationFunction )
		{
			pProducer->configurationFunction(pProducer);

			if ( NULL != pProducer->productionFunction )
			{
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
			printf("%s error : pointer to configurationFunction is NULL !\n",__PRETTY_FUNCTION__);
		}
	}
	else
	{
		printf("%s error : pointer to producer is NULL !\n",__PRETTY_FUNCTION__);
	}
}

