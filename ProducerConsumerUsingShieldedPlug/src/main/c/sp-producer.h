/*
 * C
 *
 * Copyright 2015 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY. Use is subject to license terms.
 */
#ifndef SP_PRODUCER_H
#define SP_PRODUCER_H

#include <stdint.h>

typedef struct SP_PRODUCER_t SP_PRODUCER_t;						//forward declaration of struct type needed for self-reference to struct in pointer to function

struct SP_PRODUCER_t{
	//== "public" data - needs to be set when calling PRODUCER_init(...)
	const signed char name[32];								//name of the producing task (useful for debugging purposes)
	int16_t productionPeriodInMS;							//period (in Milliseconds) for producing data
	void (*configurationFunction)(SP_PRODUCER_t* pProducer);	//pointer to configuration function - with pointer to producer argument
	void (*productionFunction)(SP_PRODUCER_t* pProducer);		//pointer to production function - with pointer to producer argument
};

//==== "public" method(s)
//== constructor
void SP_PRODUCER_init(SP_PRODUCER_t* pProducer);

//==== "private" method(s)
void _SP_PRODUCER_taskBody(void* arg);

#endif // SP_PRODUCER_H
