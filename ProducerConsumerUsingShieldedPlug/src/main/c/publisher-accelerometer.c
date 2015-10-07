/*
 * C
 *
 * Copyright 2015 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY. Use is subject to license terms.
 */

/* Includes ------------------------------------------------------------------*/

#include "publisher-accelerometer.h"
#include "accelerometer-data.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define DATABASE_ID 0
#define DATABASE_FIELD_ID_ACCELEROMETER 0

/* Public API Implementation -----------------------------------------------------------------------*/

void PUBLISHER_accelerometer_init_ALL(void)
{
	//since publishers are passed as FreeRTOS task parameters, they must live for the lifetime of the task (i.e. forever)
	//we therefore declare them as static
	static PUBLISHER_accelerometer_t accelerometerPublisher1 = {{"PUBLISHER_Accelerometer_Task_1",1000,&PUBLISHER_accelerometer_publish_adapter,DATABASE_ID},1};
	static PUBLISHER_accelerometer_t accelerometerPublisher2 = {{"PUBLISHER_Accelerometer_Task_2",1500,&PUBLISHER_accelerometer_publish_adapter,DATABASE_ID},2}; //

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

void PUBLISHER_accelerometer_publish_adapter(PUBLISHER_t* pPublisher)
{
	if ( NULL != pPublisher )
	{
		PUBLISHER_accelerometer_publish((PUBLISHER_accelerometer_t*) pPublisher);
	}
}

void PUBLISHER_accelerometer_publish(PUBLISHER_accelerometer_t* pPublisher)
{
	ShieldedPlug* pDatabase = pPublisher->super.pDatabase;
	if ( NULL != pDatabase )
	{
		Accelerometer_data_t data = Accelerometer_data_generate_sample(pPublisher->sensor_ID);
		
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
