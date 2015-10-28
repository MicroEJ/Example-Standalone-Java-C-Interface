/*
 * C
 *
 * Copyright 2015 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY. Use is subject to license terms.
 */
#ifndef PRODUCER_H
#define PRODUCER_H

#include <stdint.h>
#include <sp.h>

typedef struct PRODUCER_t PRODUCER_t;								//forward declaration of struct type needed for self-reference to struct in pointer to function

struct PRODUCER_t{
	//== "public" data - needs to be set when calling PRODUCER_init(...)
	const signed char name[32];										//name of the producing task (useful for debugging purposes)
	int16_t productionPeriodInMS;									//period (in Milliseconds) for producing data
	void (*productionFunction)(PRODUCER_t* pProducer);			//pointer to production function - with pointer to producer argument

	int8_t shieldedPlugDatabaseId;									//shielded plug database ID

	//== "private" data
	ShieldedPlug* pDatabase;										//pointer to shielded plug database - set by PRODUCER_taskBody(...)
																	//for reuse by productionFunction
};

void PRODUCER_init(PRODUCER_t* pProducer);
void PRODUCER_taskBody(void* arg);


#endif // PRODUCER_H
