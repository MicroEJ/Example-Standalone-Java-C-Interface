#include "LLNativeQueueService.h"

//TODO implement tests for each API

//jint LLQueue_createQueue(jint queueId, jint itemSize, jint maxItems);
void testLLQueue_createQueue()
{
	//check valid ranges for input params
	// QUEUE_CREATE_FAILED if ( queueId ! in [0,MAX_QUEUES_IN_REGISTRY] )
	// QUEUE_CREATE_FAILED if ( itemSize ! in [0,???] )
	// QUEUE_CREATE_FAILED if ( maxItems ! in [0,???] )
	//check QUEUE_CREATE_FAILED == result for already existing queue
}

//jint LLQueue_destroyQueue(jint queueId);
void testLLQueue_destroyQueue()
{
	//check valid ranges for input params
	//check result for non-existing queue
		// for never created before ID
		// for already created before ID, but already deleted since
}

//jint LLQueue_getItemSize(jint queueId, jint* result);
void testLLQueue_getItemSize()
{
	//check valid ranges for input params
	// QUEUE_READ_FAILED if ( queueId ! in [0,MAX_QUEUES_IN_REGISTRY] )
	//check QUEUE_INVALID_ID == result for non-existing queue
}

//jint LLQueue_getItemsCount(jint queueId, jint* result);
void testLLQueue_getItemsCount()
{
	//check valid ranges for input params
	// QUEUE_READ_FAILED if ( queueId ! in [0,MAX_QUEUES_IN_REGISTRY] )
	// QUEUE_READ_FAILED if ( NULL == result )
	//check QUEUE_INVALID_ID == result for non-existing queue
	//check result for empty queue
}

//jint LLQueue_getMaxItems(jint queueId, jint* result);
void testLLQueue_getMaxItems()
{
	//check valid ranges for input params
	// QUEUE_READ_FAILED if ( queueId ! in [0,MAX_QUEUES_IN_REGISTRY] )
	// QUEUE_READ_FAILED if ( NULL == result )
	//check QUEUE_INVALID_ID == result for non-existing queue
	//check result == relevant value use when calling createQueue before
}

//jint LLQueue_read(jint fromQueueId, jbyte* itemDataAsByteArray);
void testLLQueue_read()
{
	//check valid ranges for input params
	//check QUEUE_INVALID_ID == result for non-existing queue
	//check result for empty queue
	//check result for itemDataAsByteArray { < | > | == } testLLQueue_getItemSize()
	//check result == relevant value use when calling write before using byte-wise comparison
}

//jint LLQueue_write(jint toQueueId, jbyte* itemDataAsByteArray);
void testLLQueue_write()
{
	//check valid ranges for input params
	//check QUEUE_INVALID_ID == result for non-existing queue
	//check result for full queue
	//check available memory after call == available memory before call - itemSize
}


void testLLQueue_FIFO_behavior()
{
	//ensure that elements go out in the same order as they came in
}
