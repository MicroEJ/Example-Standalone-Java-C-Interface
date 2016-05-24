
/*
 *	H
 *	
 *	Copyright 2016 IS2T. All rights reserved.
 *	IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

#ifndef _HAL_STM32_CONFIGURATION
#define _HAL_STM32_CONFIGURATION

/* Includes ------------------------------------------------------------------*/

#include "stm32f7xx_hal.h"

/* Defines -------------------------------------------------------------------*/

/*
 * DAC timers input clock (use default value)
 */
// #define DAC_TIMER_IN_FREQUENCY	20000000	// 20 MHz

/*
 * DAC timers output clock (use default value)
 */
// #define DAC_TIMER_OUT_FREQUENCY	1000		// 1 KHz

/*
 * Available number of channels
 */
#define ADC_NB_CHANNELS 6	// 6 (ADC3)

/* EOF -----------------------------------------------------------------------*/

#endif // _HAL_STM32_CONFIGURATION
