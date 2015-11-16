# Overview

This project implements a wrapper service around OS Queue API.

It currently supports FreeRTOS only.

## Project Setup

The project is made up of java and C source files. Both need to be referenced from your application.

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

* The files in src/main/c shall be added to your BSP project.

## API Usage

### Producer

* After creation, OS Queues have to be registered using the LLQueue_registerQueue API.
* Data must be posted using the LLQueue_write API

For sample API usage, refer to the following source files :
* [AccelerometerDataProducer.java](/ProducerConsumerUsingSNIAndImmortals/src/main/java/com/microej/examples/java2c/AccelerometerDataProducer.java)
* [sni-producer-accelerometer.c](/ProducerConsumerUsingSNIAndImmortals/src/main/c/sni-producer-accelerometer.c)
* [sni-producer-messenger.c](/ProducerConsumerUsingSNIAndImmortals/src/main/c/sni-producer-messenger.c)

### Consumer

* One shall start by creating an instance of the QueueService class, specifying the OS Queue ID and the data item size in bytes
* Then one shall attempt to get an item from the queue using the QueueService read API, passing as a parameter the buffer to write data on. This parameter shall be marked as belonging to the Immortals pool.

For sample API usage, refer to the following source files :
* [AccelerometerDataConsumer.java](/ProducerConsumerUsingSNIAndImmortals/src/main/java/com/microej/examples/java2c/AccelerometerDataConsumer.java)
* [ProducerConsumerExample.java](/ProducerConsumerUsingSNIAndImmortals/src/main/java/com/microej/examples/java2c/ProducerConsumerExample.java)