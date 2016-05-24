/*
 * C
 *
 * Copyright 2015-2016 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

#ifndef _LLCOMM_UART
#define _LLCOMM_UART

/* Includes ------------------------------------------------------------------*/

#include "LLCOMM_BUFFERED_CONNECTION.h"
#include "LLCOMM_UART_configuration.h"
#include "stm32f7xx_hal.h"

/* Defines and Macros --------------------------------------------------------*/

/*
 * Defines the software FIFO size for transmission used by MicroEJ framework.
 * Default size is 5 bytes.
 */
#ifndef LLCOMM_UART_TX_BUFFER_SIZE
#define LLCOMM_UART_TX_BUFFER_SIZE 5
#endif

/*
 * Defines the software FIFO size for reception used by MicroEJ framework
 * Default size is 1024 bytes.
 */
#ifndef LLCOMM_UART_RX_BUFFER_SIZE
#define LLCOMM_UART_RX_BUFFER_SIZE 1*1024
#endif

/* Structures ----------------------------------------------------------------*/

typedef struct LLCOMM_UART
{
	// MicroEJ LLCOMM connection header
	struct LLCOMM_BUFFERED_CONNECTION header;

	// HAL UART instance
	UART_HandleTypeDef hal_uart_handle;

	// HAL UART ID
	uint8_t platformId;

	// Transmit interrupt status
	uint8_t txInterruptEnabled;

	// Software FIFO size for transmission used by MicroEJ framework
	uint8_t txBuffer[LLCOMM_UART_TX_BUFFER_SIZE];

	// Software FIFO size for reception used by MicroEJ framework
	uint8_t rxBuffer[LLCOMM_UART_RX_BUFFER_SIZE];

} LLCOMM_UART;

/* Public functions ----------------------------------------------------------*/

/*
 * Generic UART interrupt callback
 */
void LLCOMM_UART_callback(LLCOMM_UART* llcomm);

/* EOF -----------------------------------------------------------------------*/

#endif // _LLCOMM_UART
