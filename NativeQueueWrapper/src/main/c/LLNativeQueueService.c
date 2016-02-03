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

const struct queue_registry_entry_t UNINITIALIZED_registry_entry =
{
		NULL,
		0,
		0,
		0
};

//== helper methods declarations
void _LLQueue_pauseCurrentJavaThread(queue_registry_entry_t* fromQueue);
void _LLQueue_resumePendingJavaThread(queue_registry_entry_t* fromQueue);
jint _LLQueue_read(queue_registry_entry_t* fromQueue, jbyte* itemDataAsByteArray, jboolean fromJava);
jint _LLQueue_write(queue_registry_entry_t* toQueue, jbyte* itemDataAsByteArray, jboolean fromJava);


//== regular queue API
jboolean LLQueue_init(queue_registry_entry_t* queue, xQueueHandle queueHandle, jint itemSize, jint maxItems )
{
	jboolean result = JFALSE;
	if ( NULL != queue )
	{
		if ( NULL == queue->queueHandle )
		{
			queue->queueHandle = queueHandle;
			queue->itemSize = itemSize;
			queue->maxItems = maxItems;
			queue->javaThreadId = 0;
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

jint LLQueue_getItemSize(queue_registry_entry_t* queue,  jint* result)
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

jint LLQueue_getItemsCount(queue_registry_entry_t* queue, jint* result)
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

jint LLQueue_getMaxItems(queue_registry_entry_t* queue, jint* result)
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

jint LLQueue_read(queue_registry_entry_t* fromQueue, jbyte* itemDataAsByteArray){
	return _LLQueue_read(fromQueue,itemDataAsByteArray,JFALSE);
}

jint LLQueue_write(queue_registry_entry_t* toQueue, jbyte* itemDataAsByteArray)
{
	return _LLQueue_write(toQueue, itemDataAsByteArray, JFALSE);
}


//== helper methods implementations
void _LLQueue_pauseCurrentJavaThread(queue_registry_entry_t* fromQueue)
{
	if ( NULL != fromQueue )
	{
		xQueueHandle currentQueue = fromQueue->queueHandle;
		if ( NULL != currentQueue )
		{
			fromQueue->javaThreadId = SNI_getCurrentJavaThreadID();
			SNI_suspendCurrentJavaThread(fromQueue->javaThreadId);
		}
	}
}

void _LLQueue_resumePendingJavaThread(queue_registry_entry_t* fromQueue)
{
	if ( NULL != fromQueue )
	{
		xQueueHandle currentQueue = fromQueue->queueHandle;
		if ( NULL != currentQueue )
		{
			SNI_resumeJavaThread(fromQueue->javaThreadId );
		}
	}
}

jint _LLQueue_read(queue_registry_entry_t* fromQueue, jbyte* itemDataAsByteArray, jboolean fromJava)
{
	jint result = QUEUE_READ_FAILED;
	if ( NULL != fromQueue )
	{
		xQueueHandle currentQueue = fromQueue->queueHandle;
		if ( NULL != currentQueue )
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
					_LLQueue_pauseCurrentJavaThread(fromQueue);
				}
				else {
					result = QUEUE_SERVICE_OK;
				}
			}
		}
	}
	else
	{
		result = QUEUE_INVALID_QUEUE;
	}
	return result;
}

jint _LLQueue_write(queue_registry_entry_t* toQueue, jbyte* itemDataAsByteArray, jboolean fromJava)
{
	jint result = QUEUE_WRITE_FAILED;
	if ( NULL != toQueue )
	{
		xQueueHandle currentQueue = toQueue->queueHandle;
		if ( NULL != currentQueue )
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
				portBASE_TYPE dataSent = xQueueSend(currentQueue, itemDataAsByteArray, 10);
				if (!dataSent)
				{
					result = QUEUE_WRITE_FAILED;
				}
				else
				{
					result = QUEUE_SERVICE_OK;
				}
			}
		}
		_LLQueue_resumePendingJavaThread(toQueue);
	}
	else
	{
		result = QUEUE_INVALID_QUEUE;
	}
	return result;
}

//== SNI wrappers
jint Java_com_microej_examples_nativequeue_api_NativeQueueService_getItemSize(jint queueId,  jint* result)
{
	return LLQueue_getItemSize((queue_registry_entry_t*)queueId,result);
}

jint Java_com_microej_examples_nativequeue_api_NativeQueueService_getItemsCount(jint queueId, jint* result)
{
	return LLQueue_getItemsCount((queue_registry_entry_t*)queueId,result);
}

jint Java_com_microej_examples_nativequeue_api_NativeQueueService_getMaxItems(jint queueId, jint* result)
{
	return LLQueue_getMaxItems((queue_registry_entry_t*)queueId,result);
}

jint Java_com_microej_examples_nativequeue_api_NativeQueueService_read(jint fromQueueId, jbyte* itemDataAsByteArray)
{	
	return _LLQueue_read((queue_registry_entry_t*)fromQueueId,itemDataAsByteArray,JTRUE);
}

jint Java_com_microej_examples_nativequeue_api_NativeQueueService_write(jint toQueueId, jbyte* itemDataAsByteArray)
{
	return _LLQueue_write((queue_registry_entry_t*)toQueueId,itemDataAsByteArray,JTRUE);
}
