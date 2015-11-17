/*
 * Copyright 2013-2014 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

/*
 * This implementation uses a dedicated hardware timer for time implementation,
 * instead of internal FreeRTOS timer which is a 32bits timer.
 */

/* Includes ------------------------------------------------------------------*/

#include <stdlib.h>
#include <stdio.h>

#include "FreeRTOS.h"
#include "portmacro.h"
#include "timers.h"
#include "task.h"
#include "semphr.h"

#include "LLMJVM_impl.h"
#include "microej.h"
#include "time_hardware_timer.h"
#include "interrupts.h"

/* Defines -------------------------------------------------------------------*/

/*
 * Useful macros and definitions
 */

// ID for the FreeRTOS Timer
#define WAKE_UP_TIMER_ID	42

/* Globals -------------------------------------------------------------------*/

/*
 * Shared variables
 */
// Application time offset
static int64_t LLMJVM_FREERTOS_application_time_offset = 0;

//Absolute time in ticks at which timer will be launched
static int64_t LLMJVM_FREERTOS_next_wake_up_time = INT64_MAX;

// Timer for scheduling next alarm
static xTimerHandle LLMJVM_FREERTOS_wake_up_timer;

// Java task handle
static xTaskHandle LLMJVM_FREERTOS_java_task_handle;

// Binary semaphore to wakeup microJVM
static xSemaphoreHandle LLMJVM_FREERTOS_Semaphore;

/* Private functions ---------------------------------------------------------*/

// Since LLMJVM_schedule() prototype does not match the timer callback prototype,
// we create a wrapper around it and check the ID of the timer.
static void wake_up_timer_callback(xTimerHandle timer)
{
    uint32_t id = (uint32_t ) pvTimerGetTimerID(timer);
	if(id == WAKE_UP_TIMER_ID)
	{
		LLMJVM_schedule();
	}
}

/*
 * Convert a time in milliseconds in ticks.
 * Result is round up to the next tick (ie. converting back the resulting ticks
 * to milliseconds results in a value greater than or equals to given time).
 */
static int64_t LLMJVM_FREERTOS_timeToTick(int64_t time)
{
	int64_t timeus = time * 1000;
	int64_t os_tick = 1000000 / configTICK_RATE_HZ; // tick in microseconds
	return (timeus + (os_tick-1)) / os_tick;
}

/* Public functions ----------------------------------------------------------*/

/*
 * Implementation of functions from LLMJVM_impl.h
 * and other helping functions.
 */
 
// Creates the timer used to callback the LLMJVM_schedule() function.
// After its creation, the timer is idle.
int32_t LLMJVM_IMPL_initialize()
{   
    // Create a timer to scheduler alarm for the VM
    LLMJVM_FREERTOS_wake_up_timer = xTimerCreate(NULL, 100, pdTRUE, (void*) WAKE_UP_TIMER_ID, wake_up_timer_callback);

	if(LLMJVM_FREERTOS_wake_up_timer == 0)
	{
		return LLMJVM_ERROR;
	}

	vSemaphoreCreateBinary(LLMJVM_FREERTOS_Semaphore);

	if(LLMJVM_FREERTOS_Semaphore == NULL)
	{
		return LLMJVM_ERROR;
	}

	// Created semaphore is in a state such that the first call to 'take' the semaphore would pass.
	// Just execute a take to init it at 0.
	// Note: From FreeRTOS version 7.6.0, the function xSemaphoreCreateBinary can be used to avoid
	// the call to xSemaphoreTake.
	xSemaphoreTake(LLMJVM_FREERTOS_Semaphore, portMAX_DELAY);

	time_hardware_timer_initialize();
	return LLMJVM_OK;
}

// Once the task is started, save a handle to it.
int32_t LLMJVM_IMPL_vmTaskStarted()
{
	LLMJVM_FREERTOS_java_task_handle = xTaskGetCurrentTaskHandle();
	return LLMJVM_OK;
}

// Schedules requests from the VM
int32_t LLMJVM_IMPL_scheduleRequest(int64_t absoluteTime)
{
    int64_t relativeTime;
	int64_t relativeTick; // relative from current time to 'absoluteTime'
    
    portBASE_TYPE xTimerChangePeriodResult;
    portBASE_TYPE xTimerStartResult;
    
	//First check if absolute time is lower than current schedule time
	if(absoluteTime < LLMJVM_FREERTOS_next_wake_up_time)
    {
    
        // Save new alarm absolute time
        LLMJVM_FREERTOS_next_wake_up_time = absoluteTime;
        
        // Stop current timer (no delay)
        xTimerStop(LLMJVM_FREERTOS_wake_up_timer, 0);

        // Determine relative time/tick
		relativeTime = absoluteTime - LLMJVM_IMPL_getCurrentTime(MICROEJ_TRUE);
		relativeTick = LLMJVM_FREERTOS_timeToTick(relativeTime);  // = number of ticks before absoluteTime
		
		if(relativeTick > 0)
		{	
            // Schedule the new alarm
			xTimerChangePeriodResult = xTimerChangePeriod(LLMJVM_FREERTOS_wake_up_timer, relativeTick, 0);
			xTimerStartResult = xTimerStart(LLMJVM_FREERTOS_wake_up_timer, 0);
            
            if( (xTimerChangePeriodResult == pdPASS) && (xTimerStartResult == pdPASS) )
                return LLMJVM_OK;
            else
                return LLMJVM_ERROR;
		}
        else
        {
            // 'absoluteTime' has been reached already, notify the VM now
            return LLMJVM_schedule();
        }
	}
    // else : nothing to do. An sooner alarm is scheduled.
	
	return LLMJVM_OK;
}

// Suspends the VM task if the pending flag is not set
int32_t LLMJVM_IMPL_idleVM()
{
	portBASE_TYPE res = xSemaphoreTake(LLMJVM_FREERTOS_Semaphore, portMAX_DELAY);

	return res == pdTRUE ? LLMJVM_OK : LLMJVM_ERROR;
}

// Wakes up the VM task and reset next wake up time
int32_t LLMJVM_IMPL_wakeupVM()
{
	portBASE_TYPE res;
	if(isInInterrupt()){
		portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
		res = xSemaphoreGiveFromISR(LLMJVM_FREERTOS_Semaphore, &xHigherPriorityTaskWoken);
		if( xHigherPriorityTaskWoken != pdFALSE ){
			// Force a context switch here.
			portYIELD_FROM_ISR(pdTRUE);
		}
	}
	else {
		res = xSemaphoreGive(LLMJVM_FREERTOS_Semaphore);
	}
    
	LLMJVM_FREERTOS_next_wake_up_time = INT64_MAX;

	return res == pdTRUE ? LLMJVM_OK : LLMJVM_ERROR;
}

// Clear the pending wake up flag
int32_t LLMJVM_IMPL_ackWakeup()
{
	return LLMJVM_OK;
}

int32_t LLMJVM_IMPL_getCurrentTaskID()
{
	return (int32_t) xTaskGetCurrentTaskHandle();
}

// Sets application time
void LLMJVM_IMPL_setApplicationTime(int64_t t)
{
    int64_t currentTime = LLMJVM_IMPL_getCurrentTime(MICROEJ_TRUE);
	LLMJVM_FREERTOS_application_time_offset = t - currentTime;
}

// Gets the system or the application time in milliseconds
int64_t LLMJVM_IMPL_getCurrentTime(uint8_t system)
{      
    int64_t systemTime = systemTime = time_hardware_timer_getCurrentTime();
    
	if(system)
		return systemTime;
	else
		return systemTime + LLMJVM_FREERTOS_application_time_offset;
}

// Gets the current system time in nanoseconds
int64_t LLMJVM_IMPL_getTimeNanos(){
	return time_hardware_timer_getTimeNanos();
}

int32_t LLMJVM_IMPL_shutdown(void){
	// nothing to do
	return LLMJVM_OK;
}

