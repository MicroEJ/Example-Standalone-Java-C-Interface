/*
 *
 * Java
 *
 * Copyright 2016 IS2T. All rights reserved.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 *
 */
package com.microej.examples.java2c;

import com.is2t.sp.EmptyBlockException;
import com.is2t.sp.ShieldedPlug;
import com.microej.examples.java2c.AccelerometerData;

public class AccelerometerDataConsumer implements Runnable{

	@Override
	public void run() {
		byte[] accelerometerDataAsByteArray = new byte[AccelerometerData.ACCELEROMETER_DATA_SIZE];
		
		ShieldedPlug database = ShieldedPlug.getDatabase(AccelerometerData.DATABASE_ID);
		
		while (true) {
			try {
				// wait for a change on accelerometer data
				database.waitFor(AccelerometerData.DATABASE_FIELD_ID_ACCELEROMETER);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
			try {
				// read the data
				database.read(AccelerometerData.DATABASE_FIELD_ID_ACCELEROMETER, accelerometerDataAsByteArray);
				// convert to 
				AccelerometerData accelerometerData = new AccelerometerData(accelerometerDataAsByteArray);
				System.out.println("-" + accelerometerData.toString());
			} catch (EmptyBlockException e) {
				e.printStackTrace();
			}

		}		
	}

}
