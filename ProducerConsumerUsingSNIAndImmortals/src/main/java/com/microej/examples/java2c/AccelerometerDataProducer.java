/**
 *
 * Java
 *
 * Copyright 2015 IS2T. All rights reserved.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 *
 */
package com.microej.examples.java2c;

import java.io.IOException;

import com.microej.examples.java2c.AccelerometerData;
import com.microej.examples.nativequeue.api.QueueOperationReturnCode;
import com.microej.examples.nativequeue.api.QueueService;

import ej.bon.Immortals;


public class AccelerometerDataProducer implements Runnable{

	public static final int MAX_ITEMS_IN_QUEUE = 10;

	//ensure no one else accesses this
	private QueueService queueService;
	private int queueId;
	private int dataSize;
	private int maxItemsInQueue;
	private final byte[] data;

	private byte sensorID;
	private int productionPeriodInMilliseconds;

	public AccelerometerDataProducer(int queueId, int productionPeriodInMilliseconds, int sensorID) {
		this.queueId = queueId;
		this.dataSize = AccelerometerData.ACCELEROMETER_DATA_SIZE;
		this.maxItemsInQueue = AccelerometerDataProducer.MAX_ITEMS_IN_QUEUE;
		this.sensorID = (byte)sensorID;
		this.productionPeriodInMilliseconds = productionPeriodInMilliseconds;

		this.queueService = new QueueService(this.queueId);
		this.queueService.createQueue(this.dataSize, this.maxItemsInQueue);
		
		//need to make the byte [] immortal to bypass SNI API constraints on parameters that specify that
		//"the native functions cannot access Java objects field nor methods"
		this.data = (byte[]) Immortals.setImmortal(new byte[this.dataSize]);
	}


	@Override
	public void run() {

		//passing elements by reference is not possible in Java
		//we use a workaround by wrapping the reference elements into arrays of size 1
		int[] itemSizeReferenceHolder 	= {0};
		int[] itemsCountReferenceHolder = {0};
		int[] maxItemsReferenceHolder 	= {0};
		//using Immortals API to bypass SNI API constraints
		Immortals.setImmortal(itemSizeReferenceHolder);
		Immortals.setImmortal(itemsCountReferenceHolder);
		Immortals.setImmortal(maxItemsReferenceHolder);

		Integer returnCode = 0;
		
		try {
			returnCode = this.queueService.getItemSize(itemSizeReferenceHolder);
			System.out.println("Return Code : " + QueueOperationReturnCode.toString(returnCode));
			System.out.println("ItemSize : " + itemSizeReferenceHolder[0] );
			returnCode = this.queueService.getItemsCount(itemsCountReferenceHolder);
			System.out.println("Return Code : " + QueueOperationReturnCode.toString(returnCode));
			System.out.println("ItemsCount : " + itemsCountReferenceHolder[0]);
			returnCode = this.queueService.getMaxItems(maxItemsReferenceHolder);
			System.out.println("Return Code : " + QueueOperationReturnCode.toString(returnCode));
			System.out.println("MaxItems : " + maxItemsReferenceHolder[0]);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

		while(true)
		{
			//use the QueueService to post data
			try {
				AccelerometerData accelerometerData = AccelerometerData.generateRandomData(sensorID);
				System.arraycopy(accelerometerData.toByteArray(), 0, this.data, 0, this.dataSize);	
				final int writeStatus = this.queueService.write(this.data);
				if ( QueueOperationReturnCode.QUEUE_WRITE_OK == writeStatus)
				{
					System.out.println("+" + accelerometerData.toString());
				}
				else
				{
					System.out.println(this.getClass().getName() + " failure writing data " + QueueOperationReturnCode.toString(writeStatus));
				}
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
