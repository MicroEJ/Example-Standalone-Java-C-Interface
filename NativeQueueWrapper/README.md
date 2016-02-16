[](Markdown)
[](Copyright 2016 IS2T. All rights reserved.)
[](IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.)
# Overview

This project implements a SNI wrapper around OS native Queue service API.

It currently supports FreeRTOS only.

## Project Setup

The project is made up of Java and C source files. Both need to be referenced from your application.

### Java sources

Add this project to your project build Path :

* Right-Click on your project and select **Properties** from the context menu
* Select **Java Build Path**
* Select the **Projects** tab
* Click on **Add** Button
* Select **NativeQueueWrapper**
* Click on **Ok**
* Click on **Ok** 

### C sources

* The files in [src/main/c](/NativeQueueWrapper/src/main/c) shall be added to your BSP project.

## API Usage

### Queue creation

#### Creating a queue service descriptor
* OS Queues shall be created using regular C API such as FreeRTOS **xQueueCreate**
* The **LLQueue_init(queue_service_descriptor_t* queue, xQueueHandle queueHandle, jint itemSize, jint maxItems)** shall then be used to initialize a **queue_service_descriptor_t**. See :
	* **SNI_PRODUCER_accelerometer_queue_init()** function in [sni-producer-accelerometer.c](/ProducerConsumerUsingSNIAndImmortals/src/main/c/sni-producer-accelerometer.c)
	* **SNI_PRODUCER_messenger_queue_init()** in [sni-producer-messenger.c](/ProducerConsumerUsingSNIAndImmortals/src/main/c/sni-producer-messenger.c)

#### Reusing the queue service descriptor from the Java world
* The queue service descriptor can then be retrieved from the Java world by using a native casting the pointer to the descriptor to a jint. See :
	* **Java_com_microej_examples_java2c_AccelerometerData_getQueuePtr()** function in [sni-producer-accelerometer.c](/ProducerConsumerUsingSNIAndImmortals/src/main/c/sni-producer-accelerometer.c)
	* **getQueuePtr()** method in [AccelerometerData.java](/ProducerConsumerData/src/main/java/com/microej/examples/java2c/AccelerometerData.java) 

* A QueueService instance shall then be constructed, passing the descriptor as a parameter. See **main** methods in :
	* [SNIAndImmortalsFixedSizeExample.java](/ProducerConsumerUsingSNIAndImmortals/src/main/java/com/microej/examples/java2c/SNIAndImmortalsFixedSizeExample.java)
	* [SNIAndImmortalsVariableSizeExample.java](/ProducerConsumerUsingSNIAndImmortals/src/main/java/com/microej/examples/java2c/SNIAndImmortalsVariableSizeExample.java)
	
* This QueueService instance shall then be used by the Java producer and consumer threads. See **constructors** and **run** methods in :
	* [AccelerometerDataConsumer.java](/ProducerConsumerUsingSNIAndImmortals/src/main/java/com/microej/examples/java2c/AccelerometerDataConsumer.java)
	* [MessengerDataConsumer.java](/ProducerConsumerUsingSNIAndImmortals/src/main/java/com/microej/examples/java2c/MessengerDataConsumer.java)
	* [AccelerometerDataProducer.java](/ProducerConsumerUsingSNIAndImmortals/src/main/java/com/microej/examples/java2c/AccelerometerDataProducer.java)

### Item Handling
* When using Java, the data to be read or written shall be marked as belonging to the Immortals pool, to prevent garbage collection and to allow interaction with the SNI APIs. See **constructors** in :
	* [AccelerometerDataConsumer.java](/ProducerConsumerUsingSNIAndImmortals/src/main/java/com/microej/examples/java2c/AccelerometerDataConsumer.java)
	* [MessengerDataConsumer.java](/ProducerConsumerUsingSNIAndImmortals/src/main/java/com/microej/examples/java2c/MessengerDataConsumer.java)
	* [AccelerometerDataProducer.java](/ProducerConsumerUsingSNIAndImmortals/src/main/java/com/microej/examples/java2c/AccelerometerDataProducer.java)

#### Producing data
* Data must be posted using the **LLQueue_write** API

For sample API usage, refer to the following methods and functions :
* **run** method in [AccelerometerDataProducer.java](/ProducerConsumerUsingSNIAndImmortals/src/main/java/com/microej/examples/java2c/AccelerometerDataProducer.java)
* **SNI_PRODUCER_accelerometer_produce** function in [sni-producer-accelerometer.c](/ProducerConsumerUsingSNIAndImmortals/src/main/c/sni-producer-accelerometer.c)
* **SNI_PRODUCER_messenger_produce** function in [sni-producer-messenger.c](/ProducerConsumerUsingSNIAndImmortals/src/main/c/sni-producer-messenger.c)

#### Consuming data
* Data must be retrieved using the **LLQueue_read** API

For sample API usage, refer to the **run** methods in :
* [AccelerometerDataConsumer.java](/ProducerConsumerUsingSNIAndImmortals/src/main/java/com/microej/examples/java2c/AccelerometerDataConsumer.java)
* [MessengerDataConsumer.java](/ProducerConsumerUsingSNIAndImmortals/src/main/java/com/microej/examples/java2c/MessengerDataConsumer.java)