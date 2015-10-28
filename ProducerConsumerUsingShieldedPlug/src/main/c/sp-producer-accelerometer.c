/*
 * C
 *
 * Copyright 2015 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY. Use is subject to license terms.
 */

/* Includes ------------------------------------------------------------------*/

#include "sp-producer-accelerometer.h"
#include "accelerometer-data.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define DATABASE_ID 0
#define DATABASE_FIELD_ID_ACCELEROMETER 0

/* Public API Implementation -----------------------------------------------------------------------*/

void PRODUCER_accelerometer_init_ALL(void)
{
	//since producers are passed as FreeRTOS task parameters, they must live for the lifetime of the task (i.e. forever)
	//we therefore declare them as static
	static PRODUCER_accelerometer_t accelerometerProducer1 = {{"PRODUCER_Accelerometer_Task_1",1000,&PRODUCER_accelerometer_produce_adapter,DATABASE_ID},1};
	static PRODUCER_accelerometer_t accelerometerProducer2 = {{"PRODUCER_Accelerometer_Task_2",1500,&PRODUCER_accelerometer_produce_adapter,DATABASE_ID},2}; //

	PRODUCER_accelerometer_init(&accelerometerProducer1);
	PRODUCER_accelerometer_init(&accelerometerProducer2);
}

void PRODUCER_accelerometer_init(PRODUCER_accelerometer_t* pAccelerometer)
{
	if ( NULL != pAccelerometer )
	{
		PRODUCER_init(&pAccelerometer->super);
	}
}

void PRODUCER_accelerometer_produce_adapter(PRODUCER_t* pProducer)
{
	if ( NULL != pProducer )
	{
		PRODUCER_accelerometer_produce((PRODUCER_accelerometer_t*) pProducer);
	}
}

void PRODUCER_accelerometer_produce(PRODUCER_accelerometer_t* pProducer)
{
	ShieldedPlug* pDatabase = pProducer->super.pDatabase;
	if ( NULL != pDatabase )
	{
		Accelerometer_data_t data = Accelerometer_data_generate_sample(pProducer->sensor_ID);
		
		int32_t writeStatus = SP_write(*pDatabase,0,&data);
		if (SP_SUCCESS != writeStatus )
		{
			printf("%s could not write data. writeStatus : %d\n",__PRETTY_FUNCTION__,writeStatus);
		}
		else
		{
			char dataAsString[ACCELEROMETER_DATA_MAX_STRING_LENGTH];
			Accelerometer_data_toString(&data,dataAsString);
			printf("+%s\n",dataAsString);
		}
	}
	else
	{
		printf("%s pDatabase is NULL !\n",__PRETTY_FUNCTION__);
	}
}
