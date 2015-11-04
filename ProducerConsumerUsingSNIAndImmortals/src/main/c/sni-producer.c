/*
 * C
 *
 * Copyright 2015 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY. Use is subject to license terms.
 */

/* Includes ------------------------------------------------------------------*/

#include "sni-producer.h"

#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Defines -------------------------------------------------------------------*/

#define SNI_PRODUCER_STACK_SIZE 4096
#define SNI_PRODUCER_TASK_PRIORITY      ( 3 ) /** Should be > tskIDLE_PRIORITY & < configTIMER_TASK_PRIORITY */
#define SNI_PRODUCER_TASK_STACK_SIZE     SNI_PRODUCER_STACK_SIZE/4


/* API -----------------------------------------------------------------------*/

void SNI_PRODUCER_init(SNI_PRODUCER_t* pProducer)
{
	if ( NULL != pProducer )
	{
		// create the PRODUCER task
		xTaskCreate(_SNI_PRODUCER_taskBody, NULL, SNI_PRODUCER_TASK_STACK_SIZE, (void*) pProducer, SNI_PRODUCER_TASK_PRIORITY, NULL);
	}
	else
	{
		printf("%s error : pointer to productionFunction is NULL !\n",__PRETTY_FUNCTION__);
	}
}

void _SNI_PRODUCER_taskBody(void* arg)
{
	if ( NULL != arg )
	{
		SNI_PRODUCER_t* pProducer = (SNI_PRODUCER_t*) arg;
		const portTickType xDelay = pProducer->productionPeriodInMS / portTICK_RATE_MS;

		if ( NULL != pProducer->configurationFunction )
		{
			pProducer->configurationFunction(pProducer);
		}

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
		printf("%s error : pointer to producer is NULL !\n",__PRETTY_FUNCTION__);
	}
}
