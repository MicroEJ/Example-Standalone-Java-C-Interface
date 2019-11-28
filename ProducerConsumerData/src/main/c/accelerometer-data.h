/*
 * C
 *
 * Copyright 2016-2019 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 *
 */
#ifndef ACCELEROMETER_DATA_H
#define ACCELEROMETER_DATA_H

#include <stdint.h>

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
