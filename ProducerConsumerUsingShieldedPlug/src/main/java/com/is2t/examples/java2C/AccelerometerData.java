package com.is2t.examples.java2C;

import com.is2t.sp.ShieldedPlug;
import java.util.Random;
public class AccelerometerData {

	static Random generator = new Random();

	byte id;
	byte x;
	byte y;
	byte z;

	public AccelerometerData(byte[] accelerometerDataAsByteArray) {
		id = accelerometerDataAsByteArray[0];
		x = accelerometerDataAsByteArray[1];
		y = accelerometerDataAsByteArray[2];
		z = accelerometerDataAsByteArray[3];
	}

	public static AccelerometerData generatedRandomData(byte id)
	{
		byte[] resultAsByteArray = new byte[ACCELEROMETER_DATA_SIZE];
		generator.nextBytes(resultAsByteArray);
		resultAsByteArray[0] = id;
		AccelerometerData result = new AccelerometerData(resultAsByteArray);
		return result;
	}

	public byte[] toByteArray() {
		byte[] result = new byte[ACCELEROMETER_DATA_SIZE];
		result [0] = (byte) id;
		result [1] = x;
		result [2] = y;
		result [3] = z;
		return result;
	}
	
	@Override
	public String toString() {
		StringBuilder sb = new StringBuilder();
		sb.append("ID : ");
		sb.append(id);
		sb.append(" {x : ");
		sb.append(x);
		sb.append(", y : ");
		sb.append(y);
		sb.append(", z : ");
		sb.append(z);
		sb.append("}");
		return sb.toString();
	}

	//see src/main/resources/database-definition.xml file for relevant values
	public static final int DATABASE_ID = 0;
	public static final int DATABASE_FIELD_ID_ACCELEROMETER = 0;

	//see PUBLISHER_accelerometer_t struct in publisher-accelerometer.h file
	public static final int ACCELEROMETER_DATA_SIZE = 4;

}
