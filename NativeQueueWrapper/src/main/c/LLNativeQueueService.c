/*
 * C
 *
 * Copyright 2016 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY. Use is subject to license terms.
 *
 */
#include "LLNativeQueueService.h"
#include <stdio.h>
#include "FreeRTOS.h"
#include "queue.h"

const struct queue_service_descriptor_t UNINITIALIZED_registry_entry =
{
		NULL,
		0,
		0,
		0
};

//== helper methods declarations
void _LLQueue_pauseCurrentJavaThread(queue_service_descriptor_t* fromQueue);
void _LLQueue_resumePendingJavaThread(const queue_service_descriptor_t* fromQueue);
jint _LLQueue_read(queue_service_descriptor_t* fromQueue, volatile jbyte* itemDataAsByteArray, const jboolean fromJava);
jint _LLQueue_write(const queue_service_descriptor_t* toQueue, volatile jbyte* itemDataAsByteArray, const jboolean fromJava);


//== regular queue API
jboolean LLQueue_init(queue_service_descriptor_t* queue, const xQueueHandle queueHandle, const jint itemSize, const jint maxItems )
{
	jboolean result = JFALSE;
	if ( NULL != queue )
	{
		if ( NULL == queue->queueHandle )
		{
			queue->queueHandle = queueHandle;
			queue->itemSize = itemSize;
			queue->maxItems = maxItems;
			queue->pendingJavaThreadId = 0;
			result = JTRUE;
		}
		else
		{
			//queue->queueHandle should be null before initialization
			result = JFALSE;
		}
	}	
	return result;
}


//--------------------
// each function shall check queue parameter

jint LLQueue_getItemSize(const queue_service_descriptor_t* queue,  jint* result)
{
	jint errorCode = QUEUE_READ_FAILED;
	if ( NULL != queue )
	{
		*result = 0;
		*result = queue->itemSize;
		errorCode = QUEUE_SERVICE_OK;
	}
	else
	{
		errorCode = QUEUE_INVALID_QUEUE;
	}
	return errorCode;
}

jint LLQueue_getItemsCount(const queue_service_descriptor_t* queue, jint* result)
{
	jint errorCode = QUEUE_READ_FAILED;
	if ( NULL != queue )
	{
		*result = 0;
		if ( NULL != queue->queueHandle )
		{
			*result = uxQueueMessagesWaiting(queue->queueHandle);
		}
		errorCode = QUEUE_SERVICE_OK;
	}
	else
	{
		errorCode = QUEUE_INVALID_QUEUE;
	}

	return errorCode;
}

jint LLQueue_getMaxItems(const queue_service_descriptor_t* queue, jint* result)
{
	jint errorCode = QUEUE_READ_FAILED;
	if ( NULL != queue )
	{
		*result = queue->maxItems;
		errorCode = QUEUE_SERVICE_OK;
	}
	else
	{
		errorCode = QUEUE_INVALID_QUEUE;
	}

	return errorCode;
}

jint LLQueue_read(queue_service_descriptor_t* fromQueue, volatile jbyte* itemDataAsByteArray){
	return _LLQueue_read(fromQueue,itemDataAsByteArray,JFALSE);
}

jint LLQueue_write(const queue_service_descriptor_t* toQueue, volatile jbyte* itemDataAsByteArray)
{
	return _LLQueue_write(toQueue, itemDataAsByteArray, JFALSE);
}


//== helper methods implementations
void _LLQueue_pauseCurrentJavaThread(queue_service_descriptor_t* fromQueue)
{
	if ( NULL != fromQueue )
	{
		xQueueHandle currentQueue = fromQueue->queueHandle;
		if ( NULL != currentQueue )
		{
			fromQueue->pendingJavaThreadId = SNI_getCurrentJavaThreadID();
			SNI_suspendCurrentJavaThread(fromQueue->pendingJavaThreadId);
		}
	}
}

void _LLQueue_resumePendingJavaThread(const queue_service_descriptor_t* fromQueue)
{
	if ( NULL != fromQueue )
	{
		xQueueHandle currentQueue = fromQueue->queueHandle;
		if ( NULL != currentQueue )
		{
			if ( 0 != fromQueue->pendingJavaThreadId )
			{
				SNI_resumeJavaThread(fromQueue->pendingJavaThreadId );
			}
		}
	}
}

jint _LLQueue_read(queue_service_descriptor_t* fromQueue, volatile jbyte* itemDataAsByteArray, const jboolean fromJava)
{
	jint result = QUEUE_INVALID_QUEUE;
	if ( NULL != fromQueue )
	{
		xQueueHandle currentQueue = fromQueue->queueHandle;
		if ( NULL != currentQueue )
		{
			result = QUEUE_READ_FAILED;
			if ( NULL != itemDataAsByteArray )
			{
				jboolean sizeCheckPassed = JFALSE;
				if ( JTRUE == fromJava)
				{
					jint arrayLength = SNI_getArrayLength(itemDataAsByteArray);
					if ( arrayLength == fromQueue->itemSize )
					{
						sizeCheckPassed = JTRUE;
					}
				}
				else
				{
					sizeCheckPassed = JTRUE;
				}

				if ( JTRUE == sizeCheckPassed )
				{
					portBASE_TYPE dataReceived = xQueueReceive(currentQueue, itemDataAsByteArray, 0);
					if(!dataReceived){
						if ( JTRUE == fromJava )
						{
							_LLQueue_pauseCurrentJavaThread(fromQueue);
						}
					}
					else {
						result = QUEUE_SERVICE_OK;
					}
				}
			}
		}
	}
	return result;
}

jint _LLQueue_write(const queue_service_descriptor_t* toQueue, volatile jbyte* itemDataAsByteArray, const jboolean fromJava)
{
	jint result = QUEUE_INVALID_QUEUE;
	if ( NULL != toQueue )
	{
		xQueueHandle currentQueue = toQueue->queueHandle;
		if ( NULL != currentQueue )
		{
			result = QUEUE_WRITE_FAILED;
			if ( NULL != itemDataAsByteArray )
			{
				jboolean sizeCheckPassed = JFALSE;
				if ( JTRUE == fromJava)
				{
					jint arrayLength = SNI_getArrayLength(itemDataAsByteArray);
					if ( arrayLength == toQueue->itemSize )
					{
						sizeCheckPassed = JTRUE;
					}
				}
				else
				{
					sizeCheckPassed = JTRUE;
				}

				if ( JTRUE == sizeCheckPassed )
				{
					portBASE_TYPE dataSent = xQueueSend(currentQueue, (void * const) itemDataAsByteArray, 10);
					if (dataSent)
						result = QUEUE_SERVICE_OK;
					}
				}
			}
		_LLQueue_resumePendingJavaThread(toQueue);
	}
	return result;
}

//== SNI wrappers
jint Java_com_microej_examples_nativequeue_api_NativeQueueService_getItemSize(const jint queueId,  jint* result)
{
	return LLQueue_getItemSize((queue_service_descriptor_t*)queueId,result);
}

jint Java_com_microej_examples_nativequeue_api_NativeQueueService_getItemsCount(const jint queueId, jint* result)
{
	return LLQueue_getItemsCount((queue_service_descriptor_t*)queueId,result);
}

jint Java_com_microej_examples_nativequeue_api_NativeQueueService_getMaxItems(const jint queueId, jint* result)
{
	return LLQueue_getMaxItems((queue_service_descriptor_t*)queueId,result);
}

jint Java_com_microej_examples_nativequeue_api_NativeQueueService_read(const jint fromQueueId, volatile jbyte* itemDataAsByteArray)
{	
	return _LLQueue_read((queue_service_descriptor_t*)fromQueueId,itemDataAsByteArray,JTRUE);
}

jint Java_com_microej_examples_nativequeue_api_NativeQueueService_write(const jint toQueueId, volatile jbyte* itemDataAsByteArray)
{
	return _LLQueue_write((queue_service_descriptor_t*)toQueueId,itemDataAsByteArray,JTRUE);
}
