#include "LLNativeQueueService.h"
#include <stdio.h>

const struct queue_registry_entry_t UNINITIALIZED_registry_entry =
{
		NULL,
		0,
		0,
		0
};

//== helper methods declarations
inline jboolean _LLQueue_isValidQueueId(jint QueueId);
void _LLQueue_pauseCurrentJavaThread(jint fromQueueId);
void _LLQueue_resumePendingJavaThread(jint fromQueueId);
jint _LLQueue_read(jint fromQueueId, jbyte* itemDataAsByteArray, jboolean fromJava);
jint _LLQueue_write(jint toQueueId, jbyte* itemDataAsByteArray, jboolean fromJava);

//== regular queue API
// each function shall check queueId parameter
jint LLQueue_createQueue(jint queueId, jint itemSize, jint maxItems)
{
	jint result = QUEUE_CREATE_FAILED;
	if ( _LLQueue_isValidQueueId(queueId) )
	{
		xQueueHandle newQueue = xQueueCreate(maxItems,itemSize);
		LLQueue_registerQueue(queueId,newQueue,itemSize,maxItems);
		result = QUEUE_CREATE_OK;
	}
	else
	{
		result = QUEUE_INVALID_ID;
	}
	return result;
}

jint LLQueue_registerQueue(jint queueId, xQueueHandle queueHandle, jint itemSize, jint maxItems)
{
	jint result = QUEUE_UNREGISTERED;
	if ( _LLQueue_isValidQueueId(queueId) )
	{
		queue_registry[queueId].queueHandle = queueHandle;
		queue_registry[queueId].itemSize = itemSize;
		queue_registry[queueId].maxItems = maxItems;
		result = QUEUE_REGISTERED;
	}
	else
	{
		result = QUEUE_INVALID_ID;
	}
	return result;
}

jint LLQueue_destroyQueue(jint queueId)
{
	jint errorCode = QUEUE_DELETE_FAILED;
	if ( _LLQueue_isValidQueueId(queueId) )
	{
		if ( NULL != queue_registry[queueId].queueHandle )
		{
			vQueueDelete(queue_registry[queueId].queueHandle);
		}
		errorCode = QUEUE_DELETE_OK;
	}
	else
	{
		errorCode = QUEUE_INVALID_ID;
	}
	return errorCode;
}

jint LLQueue_unregisterQueue(jint queueId, xQueueHandle queueHandle)
{
	jint  result = QUEUE_REGISTERED;
	if ( _LLQueue_isValidQueueId(queueId) )
	{
		queue_registry[queueId] = UNINITIALIZED_registry_entry;
		result = QUEUE_UNREGISTERED;
	}
	else
	{
		result = QUEUE_INVALID_ID;
	}
	return result;
}

jint LLQueue_getItemSize(jint queueId,  jint* result)
{
	jint errorCode = QUEUE_READ_FAILED;
	if ( _LLQueue_isValidQueueId(queueId) )
	{
		if ( NULL != result )
		{
			*result = 0;
			*result = queue_registry[queueId].itemSize;
			errorCode = QUEUE_READ_OK;
		}
	}
	else
	{
		errorCode = QUEUE_INVALID_ID;
	}
	return errorCode;
}

jint LLQueue_getItemsCount(jint queueId,  jint* result)
{
	jint errorCode = QUEUE_READ_FAILED;
	if ( _LLQueue_isValidQueueId(queueId) )
	{
		if ( NULL != result )
		{
			*result = 0;
			if ( NULL != queue_registry[queueId].queueHandle )
			{
				*result = uxQueueMessagesWaiting(queue_registry[queueId].queueHandle);
			}
			errorCode = QUEUE_READ_OK;
		}
		else
		{
			printf("%s result == NULL \n",__PRETTY_FUNCTION__);
		}
	}
	else
	{
		errorCode = QUEUE_INVALID_ID;
	}

	return errorCode;
}

jint LLQueue_getMaxItems(jint queueId, jint* result)
{
	jint errorCode = QUEUE_READ_FAILED;
	if ( _LLQueue_isValidQueueId(queueId) )
	{
		if ( NULL != result )
		{
			*result = 0;
			*result = queue_registry[queueId].maxItems;
			errorCode = QUEUE_READ_OK;
		}
	}
	else
	{
		errorCode = QUEUE_INVALID_ID;
	}

	return errorCode;
}

jint LLQueue_read(jint fromQueueId, jbyte* itemDataAsByteArray){
	jint errorCode = QUEUE_READ_FAILED;
	if ( _LLQueue_isValidQueueId(fromQueueId) )
	{
		errorCode =_LLQueue_read(fromQueueId,itemDataAsByteArray,JFALSE);
	}
	else
	{
		errorCode = QUEUE_INVALID_ID;
	}

	return errorCode;
}

jint LLQueue_write(jint toQueueId, jbyte* itemDataAsByteArray)
{
	jint errorCode = QUEUE_WRITE_FAILED;
	if ( _LLQueue_isValidQueueId(toQueueId) )
	{
		errorCode = _LLQueue_write(toQueueId, itemDataAsByteArray, JFALSE);
	}
	else
	{
		errorCode = QUEUE_INVALID_ID;
	}

	return errorCode;		
}

//== helper methods implementations
jboolean _LLQueue_isValidQueueId(jint queueId)
{
	//check that queueId in [0,MAX_QUEUES_IN_REGISTRY]
	jboolean result = ( queueId >= 0 && queueId < MAX_QUEUES_IN_REGISTRY );
	return result;
}

void _LLQueue_pauseCurrentJavaThread(jint fromQueueId)
{
	xQueueHandle currentQueue = queue_registry[fromQueueId].queueHandle;
	if ( NULL != currentQueue )
	{
		queue_registry[fromQueueId].javaThreadId = SNI_getCurrentJavaThreadID();
		SNI_suspendCurrentJavaThread(queue_registry[fromQueueId].javaThreadId);
	}
}

void _LLQueue_resumePendingJavaThread(jint fromQueueId)
{
	xQueueHandle currentQueue = queue_registry[fromQueueId].queueHandle;
	if ( NULL != currentQueue )
	{
		SNI_resumeJavaThread(queue_registry[fromQueueId].javaThreadId );
	}
}

jint _LLQueue_read(jint fromQueueId, jbyte* itemDataAsByteArray, jboolean fromJava)
{
	jint result = QUEUE_READ_FAILED;
	xQueueHandle currentQueue = queue_registry[fromQueueId].queueHandle;
	if ( NULL != currentQueue )
	{
		jboolean sizeCheckPassed = JFALSE;
		if ( JTRUE == fromJava)
		{
			if ( SNI_getArrayLength(itemDataAsByteArray) ==  queue_registry[fromQueueId].itemSize )
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
				_LLQueue_pauseCurrentJavaThread(fromQueueId);
			}
			else {
				result = QUEUE_READ_OK;
			}
		}
	}
	return result;
}

jint _LLQueue_write(jint toQueueId, jbyte* itemDataAsByteArray, jboolean fromJava)
{
	jint result = QUEUE_WRITE_FAILED;

	xQueueHandle currentQueue = queue_registry[toQueueId].queueHandle;
	if ( NULL != currentQueue )
	{
		jboolean sizeCheckPassed = JFALSE;
		if ( JTRUE == fromJava)
		{
			jint arrayLength =  SNI_getArrayLength(itemDataAsByteArray);
			if ( arrayLength == queue_registry[toQueueId].itemSize )
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
			if ( pdPASS == xQueueSend(currentQueue, itemDataAsByteArray, 10) )
			{
				result = QUEUE_WRITE_OK;
			}
			else
			{
				result = QUEUE_WRITE_FAILED;
			}
		}
	}
	_LLQueue_resumePendingJavaThread(toQueueId);
	return result;
}

//== SNI wrappers
jint Java_com_microej_examples_nativequeue_api_NativeQueueService_createQueue(jint queueId, jint itemSize, jint maxItems)
{
	return LLQueue_createQueue(queueId,itemSize,maxItems);
}

jint Java_com_microej_examples_nativequeue_api_NativeQueueService_destroyQueue(jint queueId)
{
	return LLQueue_destroyQueue(queueId);
}

jint Java_com_microej_examples_nativequeue_api_NativeQueueService_getItemSize(jint queueId,  jint* result)
{
	return LLQueue_getItemSize(queueId,result);
}

jint Java_com_microej_examples_nativequeue_api_NativeQueueService_getItemsCount(jint queueId, jint* result)
{
	return LLQueue_getItemsCount(queueId,result);
}

jint Java_com_microej_examples_nativequeue_api_NativeQueueService_getMaxItems(jint queueId, jint* result)
{
	return LLQueue_getMaxItems(queueId,result);
}

jint Java_com_microej_examples_nativequeue_api_NativeQueueService_read(jint fromQueueId, jbyte* itemDataAsByteArray)
{	
	return _LLQueue_read(fromQueueId,itemDataAsByteArray,JTRUE);
}

jint Java_com_microej_examples_nativequeue_api_NativeQueueService_write(jint toQueueId, jbyte* itemDataAsByteArray)
{
	return _LLQueue_write(toQueueId,itemDataAsByteArray,JTRUE);
}
