/*
 * C
 *
 * Copyright 2016-2020 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 *
 */

/* Includes ------------------------------------------------------------------*/

#include "accelerometer-data.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Private Functions implementations ---------------------------------------------------------------*/
Accelerometer_data_t Accelerometer_data_generate_sample(int8_t sensor_ID)
{
	Accelerometer_data_t result;

	result.x = (int8_t)rand();
	result.y = (int8_t)rand();
	result.z = (int8_t)rand();
	result.sensor_ID = sensor_ID;

	return result;

}

void Accelerometer_data_toString(const Accelerometer_data_t* data, char * output)
{
	if ( NULL != data )
	{
		snprintf(output,ACCELEROMETER_DATA_MAX_STRING_LENGTH,"ID : %d {x : %d, y : %d, z : %d}",data->sensor_ID, data->x,data->y,data->z);
	}
	else
	{
		strcpy(output, "");
		printf("%s error : pointer to data is NULL !\n",__PRETTY_FUNCTION__);
	}
}
