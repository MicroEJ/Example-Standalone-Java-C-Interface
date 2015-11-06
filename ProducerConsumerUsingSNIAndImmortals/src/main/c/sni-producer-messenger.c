/*
 * C
 *
 * Copyright 2015 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY. Use is subject to license terms.
 */

/* Includes ------------------------------------------------------------------*/

#include "sni-producer-messenger.h"
#include "messenger-data.h"
#include "LLNativeQueueService.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


#define PRODUCER_MESSENGER_QUEUE_ID					8 //this queue ID should not be reused for a message queue holding anything else than messenger data
#define PRODUCER_MESSENGER_QUEUE_MAX_ITEMS 	10
#define PRODUCER_MESSENGER_QUEUE_ITEM_SIZE 	255


//== constructor
void SNI_PRODUCER_messenger_init(SNI_PRODUCER_messenger_t* pAccelerometer)
{
	if ( NULL != pAccelerometer )
	{
		SNI_PRODUCER_init(&pAccelerometer->super);
		printf("%s\n",__PRETTY_FUNCTION__);
	}
}

//== functions adapters
void SNI_PRODUCER_messenger_configure_adapter(SNI_PRODUCER_t* pProducer)
{
	if ( NULL != pProducer )
	{
		SNI_PRODUCER_messenger_configure((SNI_PRODUCER_messenger_t*) pProducer);
	}
}

void SNI_PRODUCER_messenger_produce_adapter(SNI_PRODUCER_t* pProducer)
{
	if ( NULL != pProducer )
	{
		SNI_PRODUCER_messenger_produce((SNI_PRODUCER_messenger_t*) pProducer);
	}
}

//== functions implementations
void SNI_PRODUCER_messenger_configure(SNI_PRODUCER_messenger_t* pProducer)
{
	printf("%s\n",__PRETTY_FUNCTION__);

	xQueueHandle producerQueue = xQueueCreate(PRODUCER_MESSENGER_QUEUE_MAX_ITEMS,PRODUCER_MESSENGER_QUEUE_ITEM_SIZE);
	LLQueue_registerQueue(PRODUCER_MESSENGER_QUEUE_ID,producerQueue,PRODUCER_MESSENGER_QUEUE_ITEM_SIZE,PRODUCER_MESSENGER_QUEUE_MAX_ITEMS);

}

void SNI_PRODUCER_messenger_produce(SNI_PRODUCER_messenger_t* pProducer)
{	
	char* data = Messenger_data_get_next_line(&pProducer->file_index, &pProducer->line_index);
	if ( data != NULL )
	{
		if ( QUEUE_WRITE_OK == LLQueue_write(PRODUCER_MESSENGER_QUEUE_ID,(jbyte*)(data)) ) //TODO error handling
		{
			//printf("+%s\n",data);
		}
		else
		{
			printf("%s error writing data \n",__PRETTY_FUNCTION__);
		}
	}
}
