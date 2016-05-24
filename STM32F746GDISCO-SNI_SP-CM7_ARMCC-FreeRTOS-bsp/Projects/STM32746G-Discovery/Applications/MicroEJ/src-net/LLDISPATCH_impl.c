/* 
 * C
 * 
 * Copyright 2014-2016 IS2T. All rights reserved.
 * Modification and distribution is permitted under certain conditions.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

#include "LLDISPATCH_CONSTANTS.h"
#include "LLDISPATCH_impl.h"
#include "LLDISPATCH_TIME_impl.h"
#include "LLDISPATCH_lwip.h"

#include <stdio.h>
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include <lwipopts.h>
#include <sys/socket.h>
#include <lwip/sockets.h>

#include "LLNET_CONSTANTS.h"
#include "LLNET_Cfg.h"

// function used to initialize network BSP part
extern int32_t  LLBSP_NET_init(void);

// function used to retrieve current time
extern int64_t LLMJVM_IMPL_getCurrentTime__Z(uint8_t system);

static SemaphoreHandle_t LLDISPATCH_mutex;
static SemaphoreHandle_t LLDISPATCH_idle_mutex;
static int8_t LLDISPATCH_mutex_initialized = 0;

// Time to wait for the mutex to be available
#define MUTEX_TIMEOUT 	portMAX_DELAY

static TaskHandle_t	NetEmbeddedTaskStartTCB;

#define NET_EMBEDDED_TASK_STACK_SIZE (2048/4)
static uint8_t NET_EMBEDDED_TASK_NAME[] = "NetEmbedded";

static uint8_t isSleeping = 0;

static fd_set read_fds;
static fd_set write_fds;
static fd_set err_fds;

static int32_t fakeFD = -1; // used to unblock select function call


/***********************************************************************
 * Internal API functions
 ***********************************************************************/

void LLDISPATCH_wakeUpDispatchThread(){
	// DISPATCH_EVENT task blocked on select operation. Call
	// close function on fakeFD socket to unblock the select
	int32_t tmp_fakeFD = fakeFD;
	fakeFD = -1;
	int32_t res = (int)close(tmp_fakeFD);
	if(res == -1){
		LLNET_DEBUG_TRACE("LLDISPATCH_IMPL_wakeUpWaitForOperation Error on unblocking select!!!! (fakeFD: %d)\n", tmp_fakeFD);
	}
}

/***********************************************************************
 * Private functions
 ***********************************************************************/

static inline int32_t LLDISPATCH_IMPL_createFakeFD(){
	// add the fake file descriptor
	fakeFD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(fakeFD == -1){
		LLNET_DEBUG_ASSERT("Error on fakeFD creation!\n");
		return -1;
	}
	return fakeFD;
}

void LLDISPATCH_IMPL_NetEmbeddedThreadStart(void (*dispatchRunMethod)(void*)){
	// Sanity check. Synchronization has to be activated in LWIP configuration file.
#if (SYS_LIGHTWEIGHT_PROT != 1)
	#error LWIP synchronization not activated in lwipopts.h file. Please add "#define SYS_LIGHTWEIGHT_PROT	1" in lwipopts.h header file.
#endif

	vSemaphoreCreateBinary(LLDISPATCH_idle_mutex);
	xSemaphoreTake(LLDISPATCH_idle_mutex, 0);

	int32_t llbsp_net_init_res = LLBSP_NET_init();
	if(llbsp_net_init_res !=  0)
	{
		LLNET_DEBUG_ASSERT("[ERROR] net init error (%d)\n", llbsp_net_init_res);
		return;
	}
	LLNET_DEBUG_TRACE("[INFO] net init ok \n");

	dispatchRunMethod(NULL);
}


/***********************************************************************
 * Public functions
 ***********************************************************************/

void LLDISPATCH_IMPL_startThread(void* method)
{
	LLNET_DEBUG_TRACE("%s[thread %d]\n", __func__, SNI_getCurrentJavaThreadID());
	portBASE_TYPE res = xTaskCreate((TaskFunction_t)LLDISPATCH_IMPL_NetEmbeddedThreadStart, (char*) &NET_EMBEDDED_TASK_NAME[0], NET_EMBEDDED_TASK_STACK_SIZE, method, NET_EMBEDDED_TASK_PRIORITY, &NetEmbeddedTaskStartTCB);
}

void LLDISPATCH_IMPL_lock(void)
{
	if(!LLDISPATCH_mutex_initialized)
	{
		vSemaphoreCreateBinary(LLDISPATCH_mutex);
		LLDISPATCH_mutex_initialized = 1;
	}

	xSemaphoreTake(LLDISPATCH_mutex, MUTEX_TIMEOUT);
}

void LLDISPATCH_IMPL_unlock(void)
{
	xSemaphoreGive(LLDISPATCH_mutex);
}

void LLDISPATCH_IMPL_wakeUpWaitForOperation(void)
{
	LLNET_DEBUG_TRACE("%s[thread %d]\n", __func__, SNI_getCurrentJavaThreadID());
	if(isSleeping){
		LLNET_DEBUG_TRACE("%s[thread %d] isSleeping\n", __func__, SNI_getCurrentJavaThreadID());
		// DISPATCH_EVENT task is pending on semaphore
		isSleeping = 0;
		xSemaphoreGive(LLDISPATCH_idle_mutex);
	}else{
		LLDISPATCH_wakeUpDispatchThread();
	}
}
int32_t toHexFDs(unsigned char* fds){
		return fds[0]&0xFF;
}

int32_t LLDISPATCH_IMPL_waitForOperation(int32_t* fds, int32_t* fdOperations, int32_t fdsPtr)
{
	LLNET_DEBUG_TRACE("%s\n", __func__);
	struct timeval timeout;
	timeout.tv_sec = 0x7FFFFFFF;// initialize select timeout to infinite
	timeout.tv_usec = 0x7FFFFFFF;// initialize select timeout to infinite

	if(fdsPtr == -1){
		// no file descriptor is waiting for operation, put the task in sleeping mode
		isSleeping = 1;
		LLNET_DEBUG_TRACE("%s isSleeping\n", __func__);
		xSemaphoreTake(LLDISPATCH_idle_mutex, MUTEX_TIMEOUT);
	}else{
		LLDISPATCH_IMPL_lock();
		{
			FD_ZERO(&read_fds);
			FD_ZERO(&write_fds);
            FD_ZERO(&err_fds);
			int32_t fdIndex;

			// create fake file descriptor if not already done
			if(fakeFD == -1){
				LLNET_DEBUG_TRACE("%s create fake FD\n", __func__);
				// add the fake file descriptor
				fakeFD = LLDISPATCH_IMPL_createFakeFD();
				// create a fake local TCP server to wait on and call close when we want to unblock the select
				struct sockaddr_in sockaddr;
				sockaddr.sin_family = AF_INET;
				sockaddr.sin_port = htons(0);
				inet_aton("127.0.0.1", &sockaddr.sin_addr);
				int32_t ret = bind(fakeFD, (struct sockaddr*)&sockaddr, sizeof(struct sockaddr_in));
				if(ret != -1){
					ret = listen(fakeFD, 1);
					if(ret == -1){
						LLNET_DEBUG_ASSERT("LLDISPATCH_IMPL fakeFD listen error!\n");
					}
				}else{
					LLNET_DEBUG_ASSERT("LLDISPATCH_IMPL fakeFD bind error!\n");
				}
			}
			// add the fake file descriptor to the read/write select list
			FD_SET(fakeFD, &read_fds);

			// add read/write waiting operations in file descriptors select list
			for(fdIndex = fdsPtr; fdIndex>=0; --fdIndex){
				if(fdOperations[fdIndex] == LLDISPATCH_CONSTANTS_REQUEST_READ){
					FD_SET(fds[fdIndex], &read_fds);
				}else if(fdOperations[fdIndex] == LLDISPATCH_CONSTANTS_REQUEST_WRITE){
					FD_SET(fds[fdIndex], &write_fds);
				}else{
					LLNET_DEBUG_ASSERT("LLDISPATCH_IMPL_waitForOperation Wrong fd operation ERROR!!!!\n");
				}
			}
		}
		LLDISPATCH_IMPL_unlock();

		LLNET_DEBUG_TRACE("select\n read_fds=%08x - write_fds=%08x - err_fds=%08x", toHexFDs(read_fds.fd_bits), toHexFDs(write_fds.fd_bits), toHexFDs(err_fds.fd_bits));
		int32_t res = select(MEMP_NUM_TCP_PCB+MEMP_NUM_UDP_PCB, &read_fds, &write_fds, &err_fds, &timeout);

		if(res >= 0){
			LLNET_DEBUG_TRACE("%d socket available\n read_fds=%08x - write_fds=%08x - err_fds=%08x", res, toHexFDs(read_fds.fd_bits), toHexFDs(write_fds.fd_bits), toHexFDs(err_fds.fd_bits));
			return LLDISPATCH_CONSTANTS_WAIT_FOR_OPERATION_SUCCEED;
		}
		LLNET_DEBUG_ASSERT("One of the set contain an invalid file descriptor (err: %d)\n", res);
	}
	return LLDISPATCH_CONSTANTS_WAIT_FOR_OPERATION_INTERRUPTED;
}

uint8_t LLDISPATCH_IMPL_isReadyForOperation(int32_t fd, int32_t operation)
{
	if((operation == LLDISPATCH_CONSTANTS_REQUEST_READ) && (FD_ISSET(fd, &read_fds))){
		return 1;
	}else if((operation == LLDISPATCH_CONSTANTS_REQUEST_WRITE) && (FD_ISSET(fd, &write_fds))){
		return 1;
	}
	// else file descriptor not ready for operation
	return 0;
}

int64_t LLDISPATCH_TIME_IMPL_getCurrentTime(void)
{
	return LLMJVM_IMPL_getCurrentTime__Z(1); // 1 means that system time is required
}
