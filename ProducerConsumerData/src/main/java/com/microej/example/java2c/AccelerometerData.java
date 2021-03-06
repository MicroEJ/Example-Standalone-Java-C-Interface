/*
 *
 * Java
 *
 * Copyright 2016-2019 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 *
 */
package com.microej.example.java2c;

import java.util.Random;
public class AccelerometerData {

	public static final int ACCELEROMETER_DATA_SIZE = 4;

	private static Random Generator = new Random();

	private final byte sensorID;
	private final byte x;
	private final byte y;
	private final byte z;

	public AccelerometerData(byte[] accelerometerDataAsByteArray) {
		sensorID = accelerometerDataAsByteArray[0];
		x = accelerometerDataAsByteArray[1];
		y = accelerometerDataAsByteArray[2];
		z = accelerometerDataAsByteArray[3];
	}

	public static AccelerometerData generateRandomData(byte id)
	{
		byte[] resultAsByteArray = new byte[ACCELEROMETER_DATA_SIZE];
		Generator.nextBytes(resultAsByteArray);
		resultAsByteArray[0] = id;
		AccelerometerData result = new AccelerometerData(resultAsByteArray);
		return result;
	}

	public byte[] toByteArray() {
		byte[] result = new byte[ACCELEROMETER_DATA_SIZE];
		result [0] = sensorID;
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

}
