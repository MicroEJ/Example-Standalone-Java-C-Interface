/*
 * Java
 *
 * Copyright 2016 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY. Use is subject to license terms.
 */
package com.microej.examples.java2c;

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