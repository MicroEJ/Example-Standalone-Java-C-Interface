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


//== constructor
void SP_PRODUCER_accelerometer_init(SP_PRODUCER_accelerometer_t* pAccelerometer)
{
	if ( NULL != pAccelerometer )
	{
		SP_PRODUCER_init(&pAccelerometer->super);
		printf("%s sensor_ID : %d\n",__PRETTY_FUNCTION__,pAccelerometer->sensor_ID);
	}
}

//== functions adapters
void SP_PRODUCER_accelerometer_configure_adapter(SP_PRODUCER_t* pProducer)
{
	if ( NULL != pProducer )
	{
		SP_PRODUCER_accelerometer_configure((SP_PRODUCER_accelerometer_t*) pProducer);
	}
}

void SP_PRODUCER_accelerometer_produce_adapter(SP_PRODUCER_t* pProducer)
{
	if ( NULL != pProducer )
	{
		SP_PRODUCER_accelerometer_produce((SP_PRODUCER_accelerometer_t*) pProducer);
	}
}

//== functions implementations
void SP_PRODUCER_accelerometer_configure(SP_PRODUCER_accelerometer_t* pProducer)
{
	//ShieldedPlug is a typedef to void*, so the database will outlive the scope of the function
	ShieldedPlug database = SP_getDatabase(pProducer->shieldedPlugDatabaseId);
	pProducer->pDatabase = database;

	int32_t SP_number_of_blocks = SP_getSize(pProducer->pDatabase );
	printf("%s SP_number_of_blocks %d\n",__PRETTY_FUNCTION__,SP_number_of_blocks);
	int32_t SP_block_length_accelerometer = SP_getLength(pProducer->pDatabase,ACCELEROMETER_SP_DATABASE_FIELD_ID_ACCELEROMETER);
	printf("%s SP_block_length_accelerometer %d\n",__PRETTY_FUNCTION__,SP_block_length_accelerometer);
}

void SP_PRODUCER_accelerometer_produce(SP_PRODUCER_accelerometer_t* pProducer)
{
	ShieldedPlug* pDatabase = pProducer->pDatabase;
	if ( NULL != pDatabase )
	{
		Accelerometer_data_t data = Accelerometer_data_generate_sample(pProducer->sensor_ID);
		
		int32_t writeStatus = SP_write(pDatabase,ACCELEROMETER_SP_DATABASE_FIELD_ID_ACCELEROMETER,&data);
		if (SP_SUCCESS == writeStatus )
		{
			char dataAsString[ACCELEROMETER_DATA_MAX_STRING_LENGTH];
			Accelerometer_data_toString(&data,dataAsString);
			printf("+%s\n",dataAsString);
			fflush(stdout);
		}
		else
		{
			printf("%s could not write data. writeStatus : %d\n",__PRETTY_FUNCTION__,writeStatus);
		}
	}
	else
	{
		printf("%s pDatabase is NULL !\n",__PRETTY_FUNCTION__);
	}
}
