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

void PRODUCER_accelerometer_init(PRODUCER_accelerometer_t* pAccelerometer)
{
	if ( NULL != pAccelerometer )
	{
		PRODUCER_init(&pAccelerometer->super);
		printf("%s sensor_ID : %d\n",__PRETTY_FUNCTION__,pAccelerometer->sensor_ID);
	}
}

//== functions adapters
void PRODUCER_accelerometer_configure_adapter(PRODUCER_t* pProducer)
{
	if ( NULL != pProducer )
	{
		PRODUCER_accelerometer_configure((PRODUCER_accelerometer_t*) pProducer);
	}
}

void PRODUCER_accelerometer_produce_adapter(PRODUCER_t* pProducer)
{
	if ( NULL != pProducer )
	{
		PRODUCER_accelerometer_produce((PRODUCER_accelerometer_t*) pProducer);
	}
}

//== functions implementations
void PRODUCER_accelerometer_configure(PRODUCER_accelerometer_t* pProducer)
{
	//ShieldedPlug is a typed to void*, so the database will outlive the scope of the function
	ShieldedPlug database = SP_getDatabase(pProducer->shieldedPlugDatabaseId);
	pProducer->pDatabase = database;

	int32_t SPS_size = SP_getSize(pProducer->pDatabase );
	printf("%s SPS_size %d\n",__PRETTY_FUNCTION__,SPS_size);
	int32_t SPS_length_block_0 = SP_getLength(pProducer->pDatabase ,0);
	printf("%s SPS_length_block_0 %d\n",__PRETTY_FUNCTION__,SPS_length_block_0);
}

void PRODUCER_accelerometer_produce(PRODUCER_accelerometer_t* pProducer)
{
	ShieldedPlug* pDatabase = pProducer->pDatabase;
	if ( NULL != pDatabase )
	{
		Accelerometer_data_t data = Accelerometer_data_generate_sample(pProducer->sensor_ID);
		
		int32_t writeStatus = SP_write(pDatabase,0,&data);
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


//== "static" factory function
void PRODUCER_accelerometer_init_factory(void)
{
	//since producers are passed as FreeRTOS task parameters, they must live for the lifetime of the task (i.e. forever)
	//we therefore declare them as static
	static PRODUCER_accelerometer_t accelerometerProducer1 = {
			{
					"PRODUCER_Accelerometer_Task_1",
					1000,
					&PRODUCER_accelerometer_configure_adapter,
					&PRODUCER_accelerometer_produce_adapter
			},
			1,
			DATABASE_ID,
			NULL
	};

	static PRODUCER_accelerometer_t accelerometerProducer2 = {
			{
					"PRODUCER_Accelerometer_Task_2",
					1500,
					&PRODUCER_accelerometer_configure_adapter,
					&PRODUCER_accelerometer_produce_adapter
			},
			2,
			DATABASE_ID,
			NULL
	};

	PRODUCER_accelerometer_init(&accelerometerProducer1);
	PRODUCER_accelerometer_init(&accelerometerProducer2);
}
