/*
 * C
 *
 * Copyright 2015 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY. Use is subject to license terms.
 */

/* Includes ------------------------------------------------------------------*/

#include "publisher-accelerometer.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* Private Functions ------------------------------------------------------------------*/
PUBLISHER_accelerometer_t PUBLISHER_accelerometer_generate_sample(void);
void print(const PUBLISHER_t* pPublisher, const PUBLISHER_accelerometer_t* data);

PUBLISHER_accelerometer_t PUBLISHER_accelerometer_generate_sample(void)
{
	PUBLISHER_accelerometer_t result;

	result.x = (int8_t)rand();
	result.y = (int8_t)rand();
	result.z = (int8_t)rand();

	return result;

}

void print(const PUBLISHER_t* pPublisher, const PUBLISHER_accelerometer_t* data)
{
	if ( NULL != pPublisher )
	{
		if ( NULL != data )
		{
			printf("%s : {x : %d, y : %d, z : %d}\n",pPublisher->name, data->x,data->y,data->z);
		}
		{
			printf("%s error : pointer to data is NULL !\n",__PRETTY_FUNCTION__);
		}
	}
	else
	{
		printf("%s error : pointer to publisher is NULL !\n",__PRETTY_FUNCTION__);
	}
}

/* Public API Implementation -----------------------------------------------------------------------*/

void PUBLISHER_accelerometer_init(void)
{
	static PUBLISHER_t accelerometerPublisher1 = {"PUBLISHER_Accelerometer_Task_1",1000,&PUBLISHER_accelerometer_publish}; //
	PUBLISHER_init(&accelerometerPublisher1);
	static PUBLISHER_t accelerometerPublisher2 = {"PUBLISHER_Accelerometer_Task_2",1500,&PUBLISHER_accelerometer_publish}; //
	PUBLISHER_init(&accelerometerPublisher2);
}

void PUBLISHER_accelerometer_publish(const PUBLISHER_t* pPublisher)
{
	PUBLISHER_accelerometer_t data = PUBLISHER_accelerometer_generate_sample();
	print(pPublisher,&data);
}
