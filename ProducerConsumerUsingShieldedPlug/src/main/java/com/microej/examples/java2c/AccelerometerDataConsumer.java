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

public class AccelerometerDataConsumer implements Runnable{

	@Override
	public void run() {
		
		ShieldedPlug database = ShieldedPlug.getDatabase(AccelerometerData.DATABASE_ID);
		database.setReader(AccelerometerData.DATABASE_FIELD_ID_ACCELEROMETER, new AccelerometerDataUnmarshaller());	

		while (true) {
			try {
				// wait for a change on accelerometer data
				database.waitFor(AccelerometerData.DATABASE_FIELD_ID_ACCELEROMETER);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
			try {
				// read the AccelerometerData
				AccelerometerData accelerometerData = (AccelerometerData) database.readObject(AccelerometerData.DATABASE_FIELD_ID_ACCELEROMETER);
				System.out.println("-" + accelerometerData.toString());
			} catch (EmptyBlockException e) {
				e.printStackTrace();
			}

		}
	}

}
