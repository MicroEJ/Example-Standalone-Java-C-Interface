
/*
 *	H
 *	
 *	Copyright 2016 IS2T. All rights reserved.
 *	IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

#ifndef _ADC_MANAGER
#define _ADC_MANAGER

/* Includes ------------------------------------------------------------------*/

#include "hal_stm32.h"

/* API -----------------------------------------------------------------------*/

/*
 * Initialize all ADCs
 */
int32_t adc_manager_initialize(void);

/*
 * Initialize an ADC rank. Must be called by ADC manager implementation during the call of
 * "adc_manager_impl_configure_ranks()"
 */
void adc_manager_configure_rank(ADC_HandleTypeDef* AdcHandle, uint32_t rank, uint32_t channel);

/*
 * Return the ADC value for given GPIO
 */
int32_t adc_manager_get_value(hal_stm32_gpio_data* gpio);

/* EOF -----------------------------------------------------------------------*/

#endif // _ADC_MANAGER
