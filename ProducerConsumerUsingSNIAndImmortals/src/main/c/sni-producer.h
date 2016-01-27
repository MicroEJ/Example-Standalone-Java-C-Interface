/*
 * C
 *
 * Copyright 2016 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY. Use is subject to license terms.
 *
 */
#ifndef SNI_PRODUCER_H
#define SNI_PRODUCER_H

#include <stdint.h>
#include <sni.h>

typedef struct SNI_PRODUCER_t SNI_PRODUCER_t;						//forward declaration of struct type needed for self-reference to struct in pointer to function

struct SNI_PRODUCER_t{
	//== "public" data - needs to be set when calling PRODUCER_init(...)
	const signed char name[32];								//name of the producing task (useful for debugging purposes)
	int16_t productionPeriodInMS;							//period (in Milliseconds) for producing data
	jboolean (*configurationFunction)(SNI_PRODUCER_t* pProducer);	//pointer to configuration function - with pointer to producer argument
	void (*productionFunction)(SNI_PRODUCER_t* pProducer);		//pointer to production function - with pointer to producer argument
};

//==== "public" method(s)
//== constructor
void SNI_PRODUCER_init(SNI_PRODUCER_t* pProducer);

//==== "private" method(s)
void _SNI_PRODUCER_taskBody(void* arg);

#endif // SNI_PRODUCER_H
