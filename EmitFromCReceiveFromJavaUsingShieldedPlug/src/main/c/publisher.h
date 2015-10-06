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

typedef struct PUBLISHER_t PUBLISHER_t;								//forward declaration of struct type needed for self-reference to struct in pointer to function

struct PUBLISHER_t{
	const signed char name[32];										//name of the publishing task
	int16_t publicationPeriodInMS;									//period (in Milliseconds) for publishing data
	void (*publicationFunction)(const PUBLISHER_t* pPublisher);		//pointer to publication function - with pointer to publisher argument
};

void PUBLISHER_init(const PUBLISHER_t* pPublisher);
void PUBLISHER_taskBody(void* arg);

#endif // PUBLISHER_H
