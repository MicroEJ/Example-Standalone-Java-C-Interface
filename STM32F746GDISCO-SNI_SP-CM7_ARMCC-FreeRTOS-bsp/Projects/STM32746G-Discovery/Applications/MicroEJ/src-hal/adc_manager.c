/*
 * C
 *
 * Copyright 2016 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

/* Includes ------------------------------------------------------------------*/

#include "hal_stm32.h"
#include "adc_manager_impl.h"

/* Fields --------------------------------------------------------------------*/

/*
 * Buffer used to store ADC converted values
 */
static __IO uint32_t converted_values[ADC_NB_CHANNELS];

/* Private functions ---------------------------------------------------------*/

/*
 * Same function than HAL_ADC_Start_DMA(...) available in "stm32fxxx_hal_adc.c" but without
 * ADC and DMA interrupts configuration. In our case, the conversions are always running and
 * we don't need the interruptions calls.
 */
static HAL_StatusTypeDef _HAL_ADC_Start_DMA(ADC_HandleTypeDef* hadc, uint32_t* pData, uint32_t Length)
{
	__IO uint32_t counter = 0;

	/* Check the parameters */
	assert_param(IS_FUNCTIONAL_STATE(hadc->Init.ContinuousConvMode));
	assert_param(IS_ADC_EXT_TRIG_EDGE(hadc->Init.ExternalTrigConvEdge));

	/* Process locked */
	__HAL_LOCK(hadc);

	/* Enable ADC DMA mode */
	hadc->Instance->CR2 |= ADC_CR2_DMA;

	/* Enable the DMA Stream */
	HAL_DMA_Start(hadc->DMA_Handle, (uint32_t)&hadc->Instance->DR, (uint32_t)pData, Length);

	/* Change ADC state */
	hadc->State = HAL_ADC_STATE_BUSY_REG;

	/* Process unlocked */
	__HAL_UNLOCK(hadc);

	/* Check if ADC peripheral is disabled in order to enable it and wait during
     Tstab time the ADC's stabilization */
	if((hadc->Instance->CR2 & ADC_CR2_ADON) != ADC_CR2_ADON)
	{
		/* Enable the Peripheral */
		__HAL_ADC_ENABLE(hadc);

		/* Delay for ADC stabilization time */
		/* Compute number of CPU cycles to wait for */
		counter = (ADC_STAB_DELAY_US * (SystemCoreClock / 1000000));
		while(counter != 0)
		{
			counter--;
		}
	}

	/* if no external trigger present enable software conversion of regular channels */
	if((hadc->Instance->CR2 & ADC_CR2_EXTEN) == RESET)
	{
		/* Enable the selected ADC software conversion for regular group */
		hadc->Instance->CR2 |= ADC_CR2_SWSTART;
	}

	/* Return function status */
	return HAL_OK;
}

/* API -----------------------------------------------------------------------*/

int32_t adc_manager_initialize(void)
{
	uint8_t adc_ID = 0;
	uint32_t* array_ptr = (uint32_t*)&converted_values;

	while(1)
	{
		ADC_HandleTypeDef* AdcHandle;
		DMA_HandleTypeDef* hdma_adc;

		// get specific ADC data
		if (adc_manager_impl_get_data(adc_ID, &AdcHandle, &hdma_adc) != HAL_GPIO_OK)
		{
			// no more ADC
			return HAL_GPIO_OK;
		}

		hdma_adc->Init.Direction = DMA_PERIPH_TO_MEMORY;
		hdma_adc->Init.PeriphInc = DMA_PINC_DISABLE;
		hdma_adc->Init.MemInc = DMA_MINC_ENABLE;
		hdma_adc->Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
		hdma_adc->Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
		hdma_adc->Init.Mode = DMA_CIRCULAR;
		hdma_adc->Init.Priority = DMA_PRIORITY_HIGH;
		hdma_adc->Init.FIFOMode = DMA_FIFOMODE_DISABLE;
		hdma_adc->Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;
		hdma_adc->Init.MemBurst = DMA_MBURST_SINGLE;
		hdma_adc->Init.PeriphBurst = DMA_PBURST_SINGLE;
		HAL_DMA_Init(hdma_adc);

		// associate the initialized DMA handle to the ADC handle
		AdcHandle->DMA_Handle = hdma_adc;
		hdma_adc->Parent = AdcHandle;

		AdcHandle->Init.ClockPrescaler        = ADC_CLOCKPRESCALER_PCLK_DIV4;
		AdcHandle->Init.Resolution            = ADC_RESOLUTION_12B;
		AdcHandle->Init.ScanConvMode          = ENABLE;                       /* Sequencer disabled (ADC conversion on only 1 channel: channel set on rank 1) */
		AdcHandle->Init.ContinuousConvMode    = ENABLE;                        /* Continuous mode disabled to have only 1 conversion at each conversion trig */
		AdcHandle->Init.DiscontinuousConvMode = DISABLE;                       /* Parameter discarded because sequencer is disabled */
		AdcHandle->Init.NbrOfDiscConversion   = 0;
		AdcHandle->Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;        /* Conversion start trigged at each external event */
		AdcHandle->Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T1_CC1;
		AdcHandle->Init.DataAlign             = ADC_DATAALIGN_RIGHT;
		AdcHandle->Init.DMAContinuousRequests = ENABLE;
		AdcHandle->Init.EOCSelection          = DISABLE;

		if (HAL_ADC_Init(AdcHandle) != HAL_OK)
		{
			// ADC initialization Error
			return HAL_GPIO_NOK;
		}

		// initialize all ranks for this ADC
		adc_manager_impl_configure_ranks(adc_ID, AdcHandle);

		// start the conversion process
		if(_HAL_ADC_Start_DMA(AdcHandle, array_ptr, AdcHandle->Init.NbrOfConversion) != HAL_OK)
		{
			// start Conversation Error
			return HAL_GPIO_NOK;
		}

		++adc_ID;
		array_ptr += AdcHandle->Init.NbrOfConversion;
	}
}

void adc_manager_configure_rank(ADC_HandleTypeDef* AdcHandle, uint32_t rank, uint32_t channel)
{
	ADC_ChannelConfTypeDef sConfig;
	sConfig.Channel      = channel;
	sConfig.Rank         = rank;
	sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	sConfig.Offset       = 0;
	HAL_ADC_ConfigChannel(AdcHandle, &sConfig);
}

int32_t adc_manager_get_value(hal_stm32_gpio_data* gpio)
{
	return converted_values[gpio->adc_rank];
}
