/*
 *
 * Java
 *
 * Copyright 2016-2019 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 *
 */
package com.microej.example.java2c;

import ej.sp.EmptyBlockException;
import ej.sp.SPReader;
import ej.sp.ShieldedPlug;

/**
 * An {@link AccelerometerDataUnmarshaller} deserializes {@link ShieldedPlug} block data into newly created {@link AccelerometerData} objects.
 */
class AccelerometerDataUnmarshaller implements SPReader {

	byte[] accelerometerDataAsByteArray = new byte[AccelerometerData.ACCELEROMETER_DATA_SIZE];

	@Override
	public Object readObject(ShieldedPlug sp, int blockID) throws EmptyBlockException {
		// read the data
		sp.read(blockID, accelerometerDataAsByteArray);
		// convert to
		return new AccelerometerData(accelerometerDataAsByteArray);
	}

}