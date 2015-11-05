/*
 * C
 *
 * Copyright 2015 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY. Use is subject to license terms.
 */
#ifndef SP_PRODUCER_MESSENGER_H
#define SP_PRODUCER_MESSENGER_H

#include <stdint.h>
#include "sni-producer.h"

typedef struct SNI_PRODUCER_messenger_t{
	SNI_PRODUCER_t super;						//a SNI_PRODUCER_messenger_t "Is-a" SNI_PRODUCER_t
	int32_t file_index;
	int32_t line_index;
}SNI_PRODUCER_messenger_t;

//== constructor
void SNI_PRODUCER_messenger_init(SNI_PRODUCER_messenger_t* messenger);

//== functions adapters
void SNI_PRODUCER_messenger_configure_adapter(SNI_PRODUCER_t* pProducer);
void SNI_PRODUCER_messenger_produce_adapter(SNI_PRODUCER_t* pProducer);

//== functions implementations
void SNI_PRODUCER_messenger_configure(SNI_PRODUCER_messenger_t* pProducer);
void SNI_PRODUCER_messenger_produce(SNI_PRODUCER_messenger_t* pProducer);

#endif // SP_PRODUCER_MESSENGER_H
