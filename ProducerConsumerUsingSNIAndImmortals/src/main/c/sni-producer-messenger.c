/*
 * C
 *
 * Copyright 2016 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY. Use is subject to license terms.
 *
 */

/* Includes ------------------------------------------------------------------*/

#include "sni-producer-messenger.h"
#include "messenger-data.h"
#include "LLNativeQueueService.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


#define PRODUCER_MESSENGER_QUEUE_MAX_ITEMS 	10
#define PRODUCER_MESSENGER_QUEUE_ITEM_SIZE 	255

xQueueHandle messengerQueueHandle;
queue_service_descriptor_t messengerQueue = {0};

//== service initialization
jboolean SNI_PRODUCER_messenger_queue_init(void)
{
	printf("%s\n",__PRETTY_FUNCTION__);

	jboolean result = JFALSE;
	if( 0 == messengerQueueHandle )
	{
		messengerQueueHandle = xQueueCreate(PRODUCER_MESSENGER_QUEUE_MAX_ITEMS, PRODUCER_MESSENGER_QUEUE_ITEM_SIZE);
		if( 0 == messengerQueueHandle )
		{
			// Queue was not created and must not be used.
		}
		else
		{
			result = LLQueue_init(&messengerQueue, messengerQueueHandle, PRODUCER_MESSENGER_QUEUE_ITEM_SIZE, PRODUCER_MESSENGER_QUEUE_MAX_ITEMS);
		}
	}
	else
	{
		//assume the queue has already been initialized ?
		result = JTRUE;
	}
	return result;
}

//== constructor
void SNI_PRODUCER_messenger_init(SNI_PRODUCER_messenger_t* pAccelerometer)
{
	if ( NULL != pAccelerometer )
	{
		SNI_PRODUCER_init(&pAccelerometer->super);
		printf("%s\n",__PRETTY_FUNCTION__);
	}
}

//== production function adapter
void SNI_PRODUCER_messenger_produce_adapter(SNI_PRODUCER_t* pProducer)
{
	if ( NULL != pProducer )
	{
		SNI_PRODUCER_messenger_produce((SNI_PRODUCER_messenger_t*) pProducer);
	}
}

//== actual production function
void SNI_PRODUCER_messenger_produce(SNI_PRODUCER_messenger_t* pProducer)
{
	Messenger_data_t data = Messenger_data_get_next_line(pProducer->sender_ID,&(pProducer->file_index), &(pProducer->line_index));
	if ( 0 != data.content_Length )
	{
		//trace for debugging/documentation purposes
		{
			char dataAsString[PRODUCER_MESSENGER_QUEUE_ITEM_SIZE];
			Messenger_data_toString(&data,dataAsString);
			printf("+%s\n",dataAsString);
		}
		if ( QUEUE_SERVICE_OK != LLQueue_write(&messengerQueue,(jbyte*)(&data)) )
		{
			printf("%s error writing data \n",__PRETTY_FUNCTION__);
		}
	}
}

//== SNI function
jint Java_com_microej_examples_java2c_SNIAndImmortalsVariableSizeExample_getMessengerQueuePtr(void)
{
	return (jint)&messengerQueue;
}
