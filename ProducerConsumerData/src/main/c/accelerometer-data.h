/*
 * C
 *
 * Copyright 2015 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY. Use is subject to license terms.
 */
#ifndef ACCELEROMETER_DATA_H
#define ACCELEROMETER_DATA_H

#include <stdint.h>

//must match the IDs used in the shielded plug database definition file
#define ACCELEROMETER_SP_DATABASE_ID 0
#define ACCELEROMETER_SP_DATABASE_FIELD_ID_ACCELEROMETER 0

#define ACCELEROMETER_DATA_MAX_STRING_LENGTH 64

typedef struct Accelerometer_data_t{
	int8_t sensor_ID;
	int8_t x;
	int8_t y;
	int8_t z;
} Accelerometer_data_t;


Accelerometer_data_t Accelerometer_data_generate_sample(int8_t sensorID);
void Accelerometer_data_toString(const Accelerometer_data_t* data, char* output);

#endif // ACCELEROMETER_DATA_H
