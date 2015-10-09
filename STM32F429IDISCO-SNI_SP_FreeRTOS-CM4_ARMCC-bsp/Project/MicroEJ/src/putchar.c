/*
 * C
 *
 * Copyright 2014 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

/* Includes ------------------------------------------------------------------*/

#include "cpu.h"
#include "stm32f4xx.h"
#include "stm32f429i_discovery.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_rcc.h"

#include <stdio.h>

/* Defines -------------------------------------------------------------------*/

#define PUTCHAR_USART	 			USART2
#define PUTCHAR_RCC_Periph_GPIO		RCC_AHB1Periph_GPIOD
#define PUTCHAR_RCC_Periph_USART	RCC_APB1Periph_USART2
#define PUTCHAR_GPIO				GPIOD
#define PUTCHAR_GPIO_PinSource		GPIO_PinSource5
#define PUTCHAR_GPIO_AF				GPIO_AF_USART2
#define PUTCHAR_GPIO_Pin			GPIO_Pin_5

/* Globals -------------------------------------------------------------------*/

static int putchar_initialized = 0;

/* Private functions ---------------------------------------------------------*/

static void uart_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* Enable GPIO clock */
	RCC_AHB1PeriphClockCmd(PUTCHAR_RCC_Periph_GPIO, ENABLE);

	/* Enable UART clock */
	RCC_APB1PeriphClockCmd(PUTCHAR_RCC_Periph_USART, ENABLE);

	/* Connect PXx to USARTx_Tx*/
	GPIO_PinAFConfig(PUTCHAR_GPIO, PUTCHAR_GPIO_PinSource, PUTCHAR_GPIO_AF);

	/* Configure USART Tx as alternate function  */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = PUTCHAR_GPIO_Pin;
	GPIO_Init(PUTCHAR_GPIO, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx;

	/* USART configuration */
	USART_Init(PUTCHAR_USART, &USART_InitStructure);

	/* Enable USART */
	USART_Cmd(PUTCHAR_USART, ENABLE);
}

/* Public functions ----------------------------------------------------------*/

extern int getkey(void)
{
	return 0;
}

int fputc(int ch, FILE *f)
{
	if(!putchar_initialized){
		uart_init();
		putchar_initialized = 1;
		while (USART_GetFlagStatus(PUTCHAR_USART, USART_FLAG_TC) == RESET);
	}

	USART_SendData(PUTCHAR_USART, (uint8_t) ch);

	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(PUTCHAR_USART, USART_FLAG_TC) == RESET);

	return ch;
}

