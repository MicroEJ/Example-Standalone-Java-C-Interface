/*
 * Copyright 2014 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

/* Includes ------------------------------------------------------------------*/

#include "time_hardware_timer.h"
#include "microej.h"
#include "cpu.h"
#include "stm32f4xx.h"
#include "interrupts.h"

/* Defines -------------------------------------------------------------------*/

// TIMER definition
#define TIMER TIM2
#define TIMER_RCC RCC_APB1Periph_TIM2
#define TIMER_IRQ TIM2_IRQn
#define TIMER_IRQHandler TIM2_IRQHandler
#define TIMER_RCC_METHOD RCC_APB1PeriphClockCmd

#define MAX_TIMER_VALUE 0xFFFFFFFF
#define TC_CLOCK 1000000	// clock frequency in Hertz

/* Globals -------------------------------------------------------------------*/

// this value is updated at each interrupt
static int64_t software_counter ;

/* Private functions ---------------------------------------------------------*/

int64_t timer_get_counter_value(void)
{
	int64_t tc = TIM_GetCounter(TIMER);
	return tc & 0xffffffff ;
}

void timer_set_interrupt_value(int64_t itVal)
{
	TIM_SetCompare1(TIMER, (uint32_t)itVal);
	TIM_ITConfig(TIMER, TIM_IT_CC1, ENABLE);
}

int64_t timer_get_max_counter_value(void)
{
	return MAX_TIMER_VALUE ;
}

int64_t timer_get_max_value(void)
{
	// get the max value which can be used for an interrupt
	// return the max value of the timer divided by 2
	// if the interrupt is initialize to the max value of timer, an overflow will occurs
	return timer_get_max_counter_value() >> 1; // / 2
}

void timer_update_software_counter(void)
{
	// increment the soft timer value
	int64_t currentTimerValue = timer_get_counter_value();
	software_counter += currentTimerValue ;
}

/**
 * Reconfigure interrupt value
 */
void timer_configure_next_interrupt(void)
{
	timer_set_interrupt_value(timer_get_max_value());
}

void timer_set_counter_value(int64_t timeVal)
{
	TIM_SetCounter(TIMER, (uint32_t) timeVal);
}

/* Interrupt functions -------------------------------------------------------*/

void TIMER_IRQHandler(void)
{
	TIM_ClearITPendingBit(TIMER, TIM_IT_CC1);

	// increment the soft timer value and reset the hardware timer to 0
	timer_update_software_counter();
	timer_set_counter_value(0) ;

	timer_configure_next_interrupt();
}

/* Public functions ----------------------------------------------------------*/

void time_hardware_timer_initialize(void)
{
	// initialize fields
	software_counter = 0;

	// initialize hardware timer
	{
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		TIM_OCInitTypeDef  TIM_OCInitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
		uint16_t PrescalerValue ;

		/* TIMER clock enable */
		TIMER_RCC_METHOD(TIMER_RCC, ENABLE);
		TIM_DeInit(TIMER);

		/* Enable the TIMER global Interrupt */
		NVIC_InitStructure.NVIC_IRQChannel = TIMER_IRQ;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);

		/* Time base configuration (1MHz) */
		PrescalerValue = ((SystemCoreClock / 2) / TC_CLOCK) - 1 ;
		TIM_TimeBaseStructure.TIM_Period = MAX_TIMER_VALUE ;
		TIM_TimeBaseStructure.TIM_Prescaler = 0;
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseInit(TIMER, &TIM_TimeBaseStructure);
		TIM_PrescalerConfig(TIMER, PrescalerValue, TIM_PSCReloadMode_Immediate);

		/* Output Compare Timing Mode configuration: Channel1 */
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
		TIM_OCInitStructure.TIM_Pulse = MAX_TIMER_VALUE;
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

		TIM_OC1Init(TIMER, &TIM_OCInitStructure);

		TIM_OC1PreloadConfig(TIMER, TIM_OCPreload_Disable);
	}

	// initialize the timer value to 0
	timer_set_counter_value(0);
	// initialize the interrupt
	timer_configure_next_interrupt();
	// hardware start
	TIM_Cmd(TIMER, ENABLE);
}

int64_t time_hardware_timer_getCurrentTime(void)
{
	return (software_counter + timer_get_counter_value())/1000 ;
}

int64_t time_hardware_timer_getTimeNanos(void)
{
	return (software_counter + timer_get_counter_value())*1000 ;
}
