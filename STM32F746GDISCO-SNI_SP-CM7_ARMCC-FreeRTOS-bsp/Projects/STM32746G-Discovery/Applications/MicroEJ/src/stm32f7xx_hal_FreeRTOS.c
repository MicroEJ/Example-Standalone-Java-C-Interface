/*
 * C
 *
 * Copyright 2015 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

#include "stm32f7xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stm32f7xx_hal_FreeRTOS_conf.h"

__STATIC_INLINE void HAL_Delay_hard(__IO uint32_t Delay){
	uint32_t tickstart = 0;
	tickstart = HAL_GetTick();
	while((HAL_GetTick() - tickstart) < Delay)
	{
	}
}

void HAL_Delay(__IO uint32_t Delay)
{
	if(Delay > HAL_DELAY_OS_THRESHOLD_MS){
		// threshold reached, use default OS delay implementation if available
#if (INCLUDE_xTaskGetSchedulerState  == 1 )
  if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
  {
#else
		#error define 'INCLUDE_xTaskGetSchedulerState' required but not set.
#endif  /* INCLUDE_xTaskGetSchedulerState */
		TickType_t ticks = Delay / portTICK_PERIOD_MS;
		vTaskDelay(ticks ? ticks : 1);          /* Minimum delay = 1 tick */
		return;
#if (INCLUDE_xTaskGetSchedulerState  == 1 )
  }
#endif  /* INCLUDE_xTaskGetSchedulerState */
	}
	// threshold not reached or OS delay not available. Use default HAL implementation
	HAL_Delay_hard(Delay);
}
