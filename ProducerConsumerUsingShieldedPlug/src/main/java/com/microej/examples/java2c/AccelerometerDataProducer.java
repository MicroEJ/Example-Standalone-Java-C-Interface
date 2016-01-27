/*
 *
 * Java
 *
 * Copyright 2016 IS2T. All rights reserved.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 *
 */
package com.microej.examples.java2c;

import com.is2t.sp.ShieldedPlug;
import com.microej.examples.java2c.AccelerometerData;

public class AccelerometerDataProducer implements Runnable{

	private byte sensorID;
	private int productionPeriodInMilliseconds;
	
	public AccelerometerDataProducer(int sensorID,int productionPeriodInMilliseconds) {
		super();
		this.sensorID = (byte)sensorID;
		this.productionPeriodInMilliseconds = productionPeriodInMilliseconds;
	}

	@Override
	public void run() {
		
		ShieldedPlug database = ShieldedPlug.getDatabase(AccelerometerData.DATABASE_ID);

		while (true) {
			try {
				AccelerometerData accelerometerData = AccelerometerData.generateRandomData(sensorID);
				database.write(AccelerometerData.DATABASE_FIELD_ID_ACCELEROMETER, accelerometerData.toByteArray());
				System.out.println("+" + accelerometerData.toString());
			} catch (RuntimeException e) {
				e.printStackTrace();
			}
			try {
				Thread.sleep(productionPeriodInMilliseconds);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}		
	}

}
