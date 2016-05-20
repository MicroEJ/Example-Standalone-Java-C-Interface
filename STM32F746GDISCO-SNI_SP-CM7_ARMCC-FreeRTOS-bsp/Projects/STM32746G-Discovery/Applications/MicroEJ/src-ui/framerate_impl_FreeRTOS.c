/*
 * C
 *
 * Copyright 2015 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

/*
 * Implementation for FreeRTOS
 */

#include "framerate_conf.h"
#ifdef FRAMERATE_ENABLED

/* Includes ------------------------------------------------------------------*/
 
#include "FreeRTOS.h"
#include "task.h"
#include "framerate_impl.h"

/* Defines -------------------------------------------------------------------*/

#define FRAMERATE_STACK_SIZE ( 512 )
#define FRAMERATE_TASK_PRIORITY      ( 3 )
#define FRAMERATE_TASK_STACK_SIZE     FRAMERATE_STACK_SIZE/4

/* Private API ---------------------------------------------------------------*/

static void _framerate_task(void * pvParameters)
{
	// launch framerate job
	framerate_task_work();
	// job end, cleanup resources
	vTaskDelete(xTaskGetCurrentTaskHandle());
}

/* API -----------------------------------------------------------------------*/

int32_t framerate_impl_start_task(void)
{
	BaseType_t xReturn = xTaskCreate( _framerate_task, "FRAMERATE", FRAMERATE_TASK_STACK_SIZE, NULL, FRAMERATE_TASK_PRIORITY, NULL );
	return xReturn == pdPASS ? FRAMERATE_OK : FRAMERATE_ERROR;
}

void framerate_impl_sleep(uint32_t ms)
{
	TickType_t ticks = ms / portTICK_PERIOD_MS;
	vTaskDelay(ticks ? ticks : 1);          /* Minimum delay = 1 tick */
	return;
}

#endif

