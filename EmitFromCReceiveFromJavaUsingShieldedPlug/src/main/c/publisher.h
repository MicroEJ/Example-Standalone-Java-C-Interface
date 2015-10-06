/*
 * C
 *
 * Copyright 2015 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY. Use is subject to license terms.
 */
#ifndef PUBLISHER_H
#define PUBLISHER_H

#include <stdint.h>
#include <sp.h>

typedef struct PUBLISHER_t PUBLISHER_t;								//forward declaration of struct type needed for self-reference to struct in pointer to function

struct PUBLISHER_t{
	//"public" data - needs to be set when calling PUBLISHER_init(...)
	const signed char name[32];										//name of the publishing task
	int16_t publicationPeriodInMS;									//period (in Milliseconds) for publishing data
	void (*publicationFunction)(PUBLISHER_t* pPublisher);			//pointer to publication function - with pointer to publisher argument
	int8_t shieldedPlugDatabaseId;									//shielded plug database ID

	//"private" data
	ShieldedPlug* pDatabase;										//pointer to shielded plug database - set by PUBLISHER_taskBody(...)
																	//for reuse by publicationFunction
};

void PUBLISHER_init(PUBLISHER_t* pPublisher);
void PUBLISHER_taskBody(void* arg);


#endif // PUBLISHER_H
