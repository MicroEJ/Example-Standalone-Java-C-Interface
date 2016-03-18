/*
 * C
 *
 * Copyright 2016 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY. Use is subject to license terms.
 *
 */
#ifndef SP_PRODUCER_MESSENGER_H
#define SP_PRODUCER_MESSENGER_H

#include <stdint.h>
#include "sni-producer.h"

typedef struct SNI_PRODUCER_messenger_t{
	SNI_PRODUCER_t super;						//a SNI_PRODUCER_messenger_t "Is-a" SNI_PRODUCER_t
	int8_t sender_ID;

	int32_t file_index;
	int32_t line_index;
}SNI_PRODUCER_messenger_t;

//== static service initialization method, must be called once before production begins
jboolean SNI_PRODUCER_messenger_queue_init(void);

//== constructor
void SNI_PRODUCER_messenger_init(SNI_PRODUCER_messenger_t* messenger);

//== production function adapter
void SNI_PRODUCER_messenger_produce_adapter(SNI_PRODUCER_t* pProducer);

//== actual production function
void SNI_PRODUCER_messenger_produce(SNI_PRODUCER_messenger_t* pProducer);

//== SNI function
jint Java_com_microej_examples_java2c_SNIAndImmortalsVariableSizeExample_getMessengerQueuePtr(void);

#endif // SP_PRODUCER_MESSENGER_H
