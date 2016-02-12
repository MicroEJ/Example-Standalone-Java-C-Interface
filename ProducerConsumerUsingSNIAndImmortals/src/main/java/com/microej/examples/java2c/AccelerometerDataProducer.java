/*
 *
 * Java
 *
 * Copyright 2016 IS2T. All rights reserved.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 *
 */
package com.microej.examples.java2c;

import java.io.IOException;

import com.microej.examples.java2c.AccelerometerData;
import com.microej.examples.nativequeue.api.QueueService;

import ej.bon.Immortals;


public class AccelerometerDataProducer implements Runnable{

	//ensure no one else accesses this
	private final QueueService queueService;

	private final byte sensorID;
	private final int productionPeriodInMilliseconds;

	private final byte[] data;

	public AccelerometerDataProducer(QueueService queueService, int sensorID, int productionPeriodInMilliseconds) {
		this.queueService = queueService;
		this.sensorID = (byte)sensorID;
		this.productionPeriodInMilliseconds = productionPeriodInMilliseconds;
		
		//need to make the byte [] immortal to bypass SNI API constraints on parameters that specify that
		//"the native functions cannot access Java objects field nor methods"
		this.data = (byte[]) Immortals.setImmortal(new byte[AccelerometerData.ACCELEROMETER_DATA_SIZE]);
	}


	@Override
	public void run() {

		try {
			System.out.println("ItemSize 	: " + this.queueService.getItemSize() );
			System.out.println("ItemsCount	: " + this.queueService.getItemsCount() );
			System.out.println("MaxItems	: " + this.queueService.getMaxItems() );
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

		while(true)
		{
			//use the QueueService to post data
			try {
				AccelerometerData accelerometerData = AccelerometerData.generateRandomData(sensorID);
				System.arraycopy(accelerometerData.toByteArray(), 0, this.data, 0, AccelerometerData.ACCELEROMETER_DATA_SIZE);	
				this.queueService.write(this.data);
				System.out.println("+" + accelerometerData.toString());
			}
			catch ( IOException e)
			{
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
