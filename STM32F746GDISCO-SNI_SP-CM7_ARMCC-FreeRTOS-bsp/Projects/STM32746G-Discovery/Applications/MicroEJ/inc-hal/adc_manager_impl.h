
/*
 *	H
 *	
 *	Copyright 2016 IS2T. All rights reserved.
 *	IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

#ifndef _ADC_MANAGER_IMPL
#define _ADC_MANAGER_IMPL

/* Includes ------------------------------------------------------------------*/

#include "hal_stm32.h"

/* API -----------------------------------------------------------------------*/

/*
 * Fill the structures addresses for the given ADC
 */
int32_t adc_manager_impl_get_data(int32_t adc_ID, ADC_HandleTypeDef** AdcHandle, DMA_HandleTypeDef** hdma_adc);

/*
 * Configures all ranks for given ADC. Implementation has to call "adc_manager_configure_rank()".
 */
void adc_manager_impl_configure_ranks(int32_t adc_ID, ADC_HandleTypeDef* AdcHandle);

/* EOF -----------------------------------------------------------------------*/

#endif // _ADC_MANAGER_IMPL
