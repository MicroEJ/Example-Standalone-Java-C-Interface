/*
 * C
 *
 * Copyright 2013-2015 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "sni.h"
#include "LLMJVM.h"
#include "FreeRTOS.h"
#include "task.h"
#include "microjvm_main.h"
#include "stm32f4xx.h"
#include "stm32f429i_discovery_sdram.h"

#include "sni-producer-factory.h"
#include "sp-producer-factory.h"

/* Defines -------------------------------------------------------------------*/

#define MICROJVM_STACK_SIZE 4096
#define JAVA_TASK_PRIORITY      ( 3 ) /** Should be > tskIDLE_PRIORITY & < configTIMER_TASK_PRIORITY */
#define JAVA_TASK_STACK_SIZE     MICROJVM_STACK_SIZE/4

/* Private functions ---------------------------------------------------------*/

void xJavaTaskFunction(void * pvParameters)
{
	microjvm_main();
	vTaskDelete( xTaskGetCurrentTaskHandle() );
}

/* Public functions ----------------------------------------------------------*/

/*
 * Generic main function
 */
int main(void)
{
	//disable buffering on stdout
	setbuf(stdout,NULL);

	printf("START\n");

	SDRAM_Init();
	
    // Enable fault handlers
    SCB->SHCSR |= SCB_SHCSR_MEMFAULTENA_Msk;
    SCB->SHCSR |= SCB_SHCSR_BUSFAULTENA_Msk;
    SCB->SHCSR |= SCB_SHCSR_USGFAULTENA_Msk;

    //-- uncomment the code below when Java app was built using the SNI_And_Immortals_Fixed_Size_Example_Build.launch
    //-- keep it commented out otherwise
    //SNI_PRODUCER_init_factory();

    //-- uncomment the code below when Java app was built using the ProducerConsumerUsingShieldedPlug_Build.launch
    //-- keep it commented out otherwise
    SP_PRODUCER_init_factory();

	// start the main task
	xTaskCreate( xJavaTaskFunction, NULL, JAVA_TASK_STACK_SIZE, NULL, JAVA_TASK_PRIORITY, NULL );
	
	vTaskStartScheduler();

	printf("END\n");
}

