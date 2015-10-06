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

#define DATABASE_ID 0
#define DATABASE_FIELD_ID_ACCELEROMETER 0

/* Private Functions declarations ------------------------------------------------------------------*/
PUBLISHER_accelerometer_data_t PUBLISHER_accelerometer_generate_sample(int8_t sensorID);
void print(const PUBLISHER_accelerometer_t* pPublisher, const PUBLISHER_accelerometer_data_t* data);

/* Private Functions implementations ---------------------------------------------------------------*/
PUBLISHER_accelerometer_data_t PUBLISHER_accelerometer_generate_sample(int8_t sensor_ID)
{
	PUBLISHER_accelerometer_data_t result;

	result.x = (int8_t)rand();
	result.y = (int8_t)rand();
	result.z = (int8_t)rand();
	result.sensor_ID = sensor_ID;

	return result;

}

void print(const PUBLISHER_accelerometer_t* pPublisher, const PUBLISHER_accelerometer_data_t* data)
{
	if ( NULL != pPublisher )
	{
		if ( NULL != data )
		{
			printf("%s : {x : %d, y : %d, z : %d}\n",pPublisher->super.name, data->x,data->y,data->z);
		}
		else
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

void PUBLISHER_accelerometer_init_ALL(void)
{
	//since publishers are passed as FreeRTOS task parameters, they must live for the lifetime of the task (i.e. forever)
	//we therefore declare them as static
	static PUBLISHER_accelerometer_t accelerometerPublisher1 = {{"PUBLISHER_Accelerometer_Task_1",1000,&PUBLISHER_accelerometer_publish,DATABASE_ID},1};
	static PUBLISHER_accelerometer_t accelerometerPublisher2 = {{"PUBLISHER_Accelerometer_Task_2",1500,&PUBLISHER_accelerometer_publish,DATABASE_ID},2}; //

	PUBLISHER_accelerometer_init(&accelerometerPublisher1);
	PUBLISHER_accelerometer_init(&accelerometerPublisher2);
}

void PUBLISHER_accelerometer_init(PUBLISHER_accelerometer_t* pAccelerometer)
{
	if ( NULL != pAccelerometer )
	{
		PUBLISHER_init(&pAccelerometer->super);
	}
}


void PUBLISHER_accelerometer_publish(PUBLISHER_t* pPublisher)
{
	if ( NULL != pPublisher )
	{
		PUBLISHER_accelerometer_publish_override((PUBLISHER_accelerometer_t*) pPublisher);
	}
}

void PUBLISHER_accelerometer_publish_override(PUBLISHER_accelerometer_t* pPublisher)
{
	ShieldedPlug* pDatabase = pPublisher->super.pDatabase;
	if ( NULL != pDatabase )
	{
		PUBLISHER_accelerometer_data_t data = PUBLISHER_accelerometer_generate_sample(pPublisher->sensor_ID);
		
		int32_t writeStatus = SP_write(*pDatabase,0,&data);
		if (SP_SUCCESS != writeStatus )
		{
			printf("%s could not write data. writeStatus : %d\n",__PRETTY_FUNCTION__,writeStatus);
		}
	}
	else
	{
		printf("%s pDatabase is NULL !\n",__PRETTY_FUNCTION__);
	}
}
