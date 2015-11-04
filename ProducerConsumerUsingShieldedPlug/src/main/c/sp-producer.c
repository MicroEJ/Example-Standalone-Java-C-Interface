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

#define SP_PRODUCER_STACK_SIZE 4096
#define SP_PRODUCER_TASK_PRIORITY      ( 3 ) /** Should be > tskIDLE_PRIORITY & < configTIMER_TASK_PRIORITY */
#define SP_PRODUCER_TASK_STACK_SIZE     SP_PRODUCER_STACK_SIZE/4

//==== "public" method(s)
//== constructor
void SP_PRODUCER_init(SP_PRODUCER_t* pProducer)
{
	if ( NULL != pProducer )
	{
		printf("%s name : %s\n",__PRETTY_FUNCTION__,pProducer->name);
		printf("%s productionPeriodInMS : %d\n",__PRETTY_FUNCTION__,pProducer->productionPeriodInMS);

		xTaskCreate(_SP_PRODUCER_taskBody, NULL, SP_PRODUCER_TASK_STACK_SIZE, (void*) pProducer, SP_PRODUCER_TASK_PRIORITY, NULL);
	}
	else
	{
		printf("%s error : pointer to productionFunction is NULL !\n",__PRETTY_FUNCTION__);
	}
}

//==== "private" method(s)
void _SP_PRODUCER_taskBody(void* arg)
{
	if ( NULL != arg )
	{
		SP_PRODUCER_t* pProducer = (SP_PRODUCER_t*) arg;
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

