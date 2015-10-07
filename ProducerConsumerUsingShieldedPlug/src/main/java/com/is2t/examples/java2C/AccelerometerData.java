/**
 *
 * Java
 *
 * Copyright 2015 IS2T. All rights reserved.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 *
 */
package com.is2t.examples.java2C;

import java.util.Random;
public class AccelerometerData {

	static Random generator = new Random();

	public static final int ACCELEROMETER_DATA_SIZE = 4;

	byte sensorID;
	byte x;
	byte y;
	byte z;

	public AccelerometerData(byte[] accelerometerDataAsByteArray) {
		sensorID = accelerometerDataAsByteArray[0];
		x = accelerometerDataAsByteArray[1];
		y = accelerometerDataAsByteArray[2];
		z = accelerometerDataAsByteArray[3];
	}

	public static AccelerometerData generateRandomData(byte id)
	{
		byte[] resultAsByteArray = new byte[ACCELEROMETER_DATA_SIZE];
		generator.nextBytes(resultAsByteArray);
		resultAsByteArray[0] = id;
		AccelerometerData result = new AccelerometerData(resultAsByteArray);
		return result;
	}

	public byte[] toByteArray() {
		byte[] result = new byte[ACCELEROMETER_DATA_SIZE];
		result [0] = (byte) sensorID;
		result [1] = x;
		result [2] = y;
		result [3] = z;
		return result;
	}
	
	@Override
	public String toString() {
		StringBuilder sb = new StringBuilder();
		sb.append("ID : ");
		sb.append(sensorID);
		sb.append(" {x : ");
		sb.append(x);
		sb.append(", y : ");
		sb.append(y);
		sb.append(", z : ");
		sb.append(z);
		sb.append("}");
		return sb.toString();
	}

	//ShieldedPlug related IDs - see src/main/resources/database-definition.xml file for relevant values
	public static final int DATABASE_ID = 0;
	public static final int DATABASE_FIELD_ID_ACCELEROMETER = 0;

}
