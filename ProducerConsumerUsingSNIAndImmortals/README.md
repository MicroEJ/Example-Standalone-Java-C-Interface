[](Markdown)
[](Copyright 2015 IS2T. All rights reserved.)
[](IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.)

# Overview

This example shows an implementation of the producer consumer pattern.

There are two kinds of data produced :
- fixed size [AccelerometerData.java](/ProducerConsumerData/src/main/java/com/microej/examples/java2c/AccelerometerData.java)
- variable size [MessengerData.java](/ProducerConsumerData/src/main/java/com/microej/examples/java2c/MessengerData.java)

There is one consumer (written in Java) and two producers (written in C) for each kind of data.

Data is exchanged via message queues. A Java wrapper has been created to access the C message queues API. This wrapper is defined in the [NativeQueueWrapper](/NativeQueueWrapper) project.

* The producers send data at a fixed period if space is available in the relevant queue.
* The consumer gets new data from the database as soon as it is available. If no data is available immediately, the queue wrapper will block until some becomes available.


The following steps will be taken :

* Project setup
* Java design
	* Java data class
	* Java consumer class
* C design
	* C data structs
	* C producer tasks
* Integration

# Project setup

For reference, BSP toolchain setup related instructions are described in the [Building a java platform](/STM32F429IDISCO-SNI_SP_FreeRTOS-CM4_ARMCC-configuration/README.md#building-a-java-platform) section of the [STM32F429IDISCO-SNI_SP_FreeRTOS-CM4_ARMCC-configuration README.md](/STM32F429IDISCO-SNI_SP_FreeRTOS-CM4_ARMCC-configuration/README.md) file.

These steps have already been done in this workspace and you do not need to repeat them.

# Java design

## Java data class

In this section, we shall create classes to represent the data being exchanged between producer(s) and consumer(s).

For fixed size data, the source code is in the [AccelerometerData.java](/ProducerConsumerData/src/main/java/com/microej/examples/java2c/AccelerometerData.java) file.

For variable size data, the source code is in the [MessengerDataConsumer.java](/ProducerConsumerData/src/main/java/com/microej/examples/java2c/MessengerData.java) file.


# Java consumer classes

In this section, we shall create classes to consume data as soon as it is available.

Each consumer :
* runs on a dedicated thread
* retrieves data through delegation to a QueueService instance

In pseudocode, the thread body roughly looks like this

	queueService = createQueueuService(QUEUE_ID)
	while (true)
	{
		data = queueService.getNextAvailableData() //blocking call until some data is available
	}

The source code is in the following files :
* [AccelerometerDataConsumer.java](/ProducerConsumerUsingSNIAndImmortals/src/main/java/com/microej/examples/java2c/AccelerometerDataConsumer.java) file.
* [MessengerDataConsumer.java](/ProducerConsumerUsingSNIAndImmortals/src/main/java/com/microej/examples/java2c/MessengerDataConsumer.java) file.

## Starting the consumer thread

* Open the [ProducerConsumerExample.java](/ProducerConsumerUsingSNIAndImmortals/src/main/java/com/microej/examples/java2c/ProducerConsumerExample.java) source file

* Update the main() method using the code below
	
		public static void main(String[] args) {
			AccelerometerDataConsumer accelerometerDataConsumer = new AccelerometerDataConsumer(ACCELEROMETER_DATA_QUEUE_ID,ACCELEROMETER_DATA_SIZE);
			new Thread(accelerometerDataConsumer).start();
	
			MessengerDataConsumer messengerConsumer = new MessengerDataConsumer(MESSENGER_DATA_QUEUE_ID,MESSENGER_DATA_SIZE);
			new Thread(messengerConsumer).start();
		}


# C design

We shall now design a way to generate data from the C world, since the actual data production is more likely to originate from a device with a driver implemented in C.

## C data struct

Let us define some
* struct to hold the data **Accelerometer_data_t** and **Messenger_data_t**
* method to generate/retrieve the data (and trace where it comes from - that is what the **Accelerometer_data_t::sensor_ID** and **Messenger_data_t::sender_ID** fields are for)
* debug helper function (the **print** function)

The source code is available in the following files :
* [accelerometer-data.h](/ProducerConsumerData/src/main/c/accelerometer-data.h)
* [accelerometer-data.c](/ProducerConsumerData/src/main/c/accelerometer-data.c)
* [messenger-data.h](/ProducerConsumerData/src/main/c/messenger-data.h)
* [messenger-data.c](/ProducerConsumerData/src/main/c/messenger-data.c)

## C producer task

In this section, we shall create C code to produce data sporadically.

### Domain-agnostic producer code

For reuse purposes, we shall separate the producer-consumer pattern implementation from the sensor-oriented and message sender domain logic.

Here we shall define that a producer is a task that must periodically call a produce function, which contents are actually domain specific.

This leads us to defining a producer "class" with the following contents
* attributes (will be set by the domain-specific producer)
	* production period
	* pointer to configuration function (with pointer to producer argument so as to be able to retrieve configuration information from a producer)
	* pointer to production function (with pointer to producer argument so as to be able to associate produced data with producer)
* methods
	* initialisation method (will start the production task)
	* taskbody (calls the configuration function once then calls the production function at every production period expiration)

Although the design is to some extent object-oriented, the implementation in this example is in C, not in C++.

The source code is available in the following files.
* [sp-producer.h](/ProducerConsumerUsingSNIAndImmortals/src/main/c/sni-producer.h)
* [sp-producer.c](/ProducerConsumerUsingSNIAndImmortals/src/main/c/sni-producer.c)


### Domain-specific producer code

Here we shall define the relevant "classes" :
* an accelerometer "class" with the following contents :
	* attributes
		* sensor_ID (useful for tracing from which sensor the data comes from)
		* "parent" producer member (so as to reuse Domain-agnostic producer code)
	* methods
		* initialisation method (will propagate initialisation to "parent")
		* adapter configuration function
			* with signature matching the one of the pointer to production function in the domain-agnostic producer struct
			* used as an adapter method to call a more specialized configuration function that can set implementation-specific info (such as creating the relevant message queue)
		* adapter production function
			* with signature matching the one of the pointer to production function in the domain-agnostic producer struct
			* used as an adapter method to call a more specialized production function that can use domain-specific producer info (such as sensor_ID)
* a messenger "class" with the following contents :
	* attributes
		* sender_ID (useful for tracing from who the message comes from)
		* "parent" producer member (so as to reuse Domain-agnostic producer code)
	* methods
		* initialisation method (will propagate initialisation to "parent")
		* adapter configuration function
			* with signature matching the one of the pointer to production function in the domain-agnostic producer struct
			* used as an adapter method to call a more specialized configuration function that can set implementation-specific info (such as creating the relevant message queue)
		* adapter production function
			* with signature matching the one of the pointer to production function in the domain-agnostic producer struct
			* used as an adapter method to call a more specialized production function that can use domain-specific producer info (such as sender_ID)

The source code is available in the following files.
* [sni-producer-accelerometer.h](/ProducerConsumerUsingSNIAndImmortals/src/main/c/sni-producer-accelerometer.h)
* [sni-producer-accelerometer.c](/ProducerConsumerUsingSNIAndImmortals/src/main/c/sni-producer-accelerometer.c)
* [sni-producer-messenger.h](/ProducerConsumerUsingSNIAndImmortals/src/main/c/sni-producer-messenger.h)
* [sni-producer-messenger.c](/ProducerConsumerUsingSNIAndImmortals/src/main/c/sp-producer-messenger.c)


### Instantiation code

The `SNI_PRODUCER_init_factory` function instantiates two producers with different IDs and production periods for each kind of data

The source code is available in the following files.
* [sni-producer-factory.h](/ProducerConsumerUsingSNIAndImmortals/src/main/c/sp-producer-factory.h)
* [sni-producer-factory.c](/ProducerConsumerUsingSNIAndImmortals/src/main/c/sp-producer-factory.c)


# Integration

For reference, BSP toolchain integration related instructions are described in the [Produced Consumer Using SNI and Immortals Integration](/STM32F429IDISCO-SNI_SP_FreeRTOS-CM4_ARMCC-configuration/README.md#producer-consumer-using-sni-and-immortals-integration) section of the [STM32F429IDISCO-SNI_SP_FreeRTOS-CM4_ARMCC-configuration/README.md](/STM32F429IDISCO-SNI_SP_FreeRTOS-CM4_ARMCC-configuration/README.md) file.

These steps have already been done in this workspace and you do not need to repeat them.

However, you still need to perform the following operation :
uncomment the call to `SNI_PRODUCER_init_factory` in the [main.c](/STM32F429IDISCO-SNI_SP_FreeRTOS-CM4_ARMCC-bsp/Project/MicroEJ/src/main.c) source file

	
## Checking the behavior
* After flashing the board, set up a terminal on the board serial port and press the reset input. You shall get an output similar to the one below :

		+ID : 2 {x : -110, y : 125, z : 85}
		-ID : 2 {x : -110, y : 125, z : 85}
		+ID : 11 {2.1. File #2 has different lines, some of wID : 11hich are lo {2.1. Filnger}
		e #2 has different lines, some of which are longer}
		+ID : 22 {2.1. File #2 has different lines-ID : 2, some of 2 {2.1. Fiwhich are lle #2 has onger}
		different lines, some of which are longer}
		+ID : 1 {x : -124, y : 80, z : 99}
		-ID : 1 {x : -124, y : 80, z : 99}
		+ID : 2 {x : 67, y : -38, z : -18}
		-ID : 2 {x : 67, y : -38, z : -18}
		+ID : 11 {2.2. But it only has two lines}
		ID : 11 {2.2. But it only has two lines}
		+ID : 22 {2.2. But it only has two lines}
		-ID : 22 {2.2. But it only has two lines}
		+ID : 2 {x : -87, y : 17, z : -112}
		-ID : 2 {x : -87, y : 17, z : -112}
		+ID : 1 {x : 108, y : 79, z : -65}
		-ID : 1 {x : 108, y : 79, z : -65}
		+ID : 2 {x : -90, y : -54, z : 85}
		-ID : 2 {x : -90, y : -54, z : 85}
		+ID : 1 {x : 77, y : 28, z : 8}
		-ID : 1 {x : 77, y : 28, z : 8}
		+ID : 11 {1.1. This is the first line}
		-ID : 11 {1.1. This is the first line}
		+ID : 2 {x : -69, y : -69, z : -14}
		-ID : 2 {x : -69, y : -69, z : -14}
		+ID : 11 {1.2. This is the next line}
		-ID : 11 {1.2. This is the next line}
		+ID : 1 {x : 38, y : -127, z : 9}
		-ID : 1 {x : 38, y : -127, z : 9}
		+ID : 11 {1.3. This is the final line}
		-ID : 11 {1.3. This is the final line}
		+ID : 2 {x : 40, y : 36, z : -89}
		-ID : 2 {x : 40, y : 36, z : -89}
		+ID : 1 {x : 55, y : -66, z : 2}
		-ID : 1 {x : 55, y : -66, z : 2}
		+ID : 22 {1.1. This is the first line}
		-ID : 22 {1.1. This is the first line}
		+ID : 2 {x : 45, y : 69, z : -76}
		-ID : 2 {x : 45, y : 69, z : -76}
		+ID : 11 {2.1. File #2 has different lines-ID : , some of w11 {2.1. Fihich are lle #2 has onger}
		different lines, some of which are longer}
		+ID : 1 {x : -64, y : 19, z : 51}
		-ID : 1 {x : -64, y : 19, z : 51}
		+ID : 22 {1.2. This is the next line}
		-ID : 22 {1.2. This is the next line}
		+ID : 2 {x : 10, y : 94, z : 89}
		-ID : 2 {x : 10, y : 94, z : 89}
		+ID : 11 {2.2. But it only has two lines}
		ID : 11 {2.2. But it only has two lines}
		+ID : 1 {x : 1, y : -64, z : -36}
		-ID : 1 {x : 1, y : -64, z : -36}
		+ID : 22 {1.3. This is the final line}
		-ID : 22 {1.3. This is the final line}
		+ID : 2 {x : -1, y : -81, z : -42}
		-ID : 2 {x : -1, y : -81, z : -42}
		+ID : 1 {x : 58, y : 5, z : 61}
		-ID : 1 {x : 58, y : 5, z : 61}
		+ID : 2 {x : 76, y : 120, z : 107}
		-ID : 2 {x : 76, y : 120, z : 107}



* The '-' prefix indicates data consumption
* The '+' prefix indicates data production
* The number right after the ID indicates which sensor or sender the data originates from. The 4 different IDs in the trace show us that data from our 4 different producers get consumed.
* Note that the trace for long messages from both producer and consumer sometimes get mixed up in the output because the producer outputs after successfully posting the data, not before. Therefore between the posting time and the flushing of the producer trace, the consumer has time to output some trace of its own on the shared output stream.  


# Advanced use cases

The Use Case shown in this document only covers a specific configuration of the producer consumer problem. Some other important use cases might be considered such as :
* allocating different processing priorities depending on the message queue
* using priority queues where some items are to be processed faster than some others

