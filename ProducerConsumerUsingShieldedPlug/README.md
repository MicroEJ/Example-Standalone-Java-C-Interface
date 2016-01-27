[](Markdown)
[](Copyright 2016 IS2T. All rights reserved.)
[](IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.)

# Overview

This example shows an implementation of the producer consumer pattern.

There is one consumer (written in Java) and four producers (two written in Java, two written in C).

Data is exchanged via a shared in-memory database using the Shielded Plug API.

* The producers overwrite data at a fixed period without checking if data has been consumed by any other task.
* The consumer gets new data from the database as soon as it is available.

The following steps will be taken :

* Project setup
* Java design
	* Java data class
	* Java producer class
	* Java consumer class
* C design
	* C data struct
	* C producer task
* Integration

# Project setup

For reference, BSP toolchain setup related instructions are described in the [Building a java platform](/STM32F429IDISCO-SNI_SP_FreeRTOS-CM4_ARMCC-configuration/README.md#building-a-java-platform) section of the [STM32F429IDISCO-SNI_SP_FreeRTOS-CM4_ARMCC-configuration README.md](/STM32F429IDISCO-SNI_SP_FreeRTOS-CM4_ARMCC-configuration/README.md) file.

These steps have already been done in this workspace and you do not need to repeat them.

# Java design

Before interfacing with the C world, we shall design a Java-only system that exchanges data between produced and consumer via the shielded plug mechanism

## Java data class

In this section, we shall create a class to represent the data being exchanged between producer(s) and consumer(s). The source code is in the [AccelerometerData.java](/ProducerConsumerData/src/main/java/com/microej/examples/java2c/AccelerometerData.java) file.


# Java producer class

In this section, we shall create a class to produce data sporadically.

Each producer runs on a dedicated thread. In pseudocode, the thread body roughly looks like this

	while (true)
	{
		data = generateData()
		deliver(data)
		wait(production_period)
	}
	
 The actual source code is in the [AccelerometerDataProducer.java](/ProducerConsumerUsingShieldedPlug/src/main/java/com/microej/examples/java2c/AccelerometerDataProducer.java) file. 
 
## Alternate implementation using TimerTask

Because of the fixed periodic nature of the task and of its short execution time, an alternate implementation based on Timer and TimerTask API is proposed in the [AccelerometerDataProducerUsingTimerTask.java](/ProducerConsumerUsingShieldedPlug/src/main/java/com/microej/examples/java2c/AccelerometerDataProducerUsingTimerTask.java) file.


## Starting the producer threads

* Open the [DataConsumerExample.java](/ProducerConsumerUsingShieldedPlug/src/main/java/com/microej/examples/java2c/DataConsumerExample.java) source file
* Replace the main() method by the code below
	
		public static void main(String[] args) {
			AccelerometerDataProducer producer = new AccelerometerDataProducer(3,1200);
			new Thread(producer).start();

			AccelerometerDataProducerUsingTimerTask producerUsingTimerTask = new AccelerometerDataProducerUsingTimerTask(4,1200);
			producerUsingTimerTask.start();
		}

At this point, you may flash on target and check that both producers generate data using a debug connection on the serial port.

# Java consumer class

In this section, we shall create a class to consume data as soon as it is available.

Each consumer runs on a dedicated thread. In pseudocode, the thread body roughly looks like this

	while (true)
	{
		data = getNextAvailableData() //blocking call until some data is available
	}

The source code is in the [AccelerometerDataConsumer.java](/ProducerConsumerUsingShieldedPlug/src/main/java/com/microej/examples/java2c/AccelerometerDataConsumer.java) file.

## Starting the consumer thread

* Open the [DataConsumerExample.java](/ProducerConsumerUsingShieldedPlug/src/main/java/com/microej/examples/java2c/DataConsumerExample.java) source file
* Update the main() method using the code below
	
		public static void main(String[] args) {
			AccelerometerDataConsumer consumer = new AccelerometerDataConsumer();
			new Thread(consumer).start();
	
			AccelerometerDataProducer producer = new AccelerometerDataProducer(3,1200);
			new Thread(producer).start();
	
			AccelerometerDataProducerUsingTimerTask producerUsingTimerTask = new AccelerometerDataProducerUsingTimerTask(4,1200);
			producerUsingTimerTask.start();
		}

At this point, you may flash on target and check that consumer reads data using a debug connection on the serial port.

# C design

Now that we are confident with the production-consumption mechanism in Java, we shall design a way to generate data from the C world, since the actual data production is more likely to originate from a device with a driver implemented in C.

## C data struct

Let us define some
* struct to hold the data : **Accelerometer_data_t**
* method to generate/retrieve the data (and trace where it comes from - that is what the **Accelerometer_data_t::sensor_ID** field is for)
* debug helper function (the **print** function)

The source code is available in the following files :
* [accelerometer-data.h](/ProducerConsumerData/src/main/c/accelerometer-data.h)
* [accelerometer-data.c](/ProducerConsumerData/src/main/c/accelerometer-data.c)

## C producer task

In this section, we shall create C code to produce data sporadically.

### Domain-agnostic producer code

For reuse purposes, we shall separate the producer-consumer pattern implementation from the sensor-oriented domain logic.

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
* [sp-producer.h](/ProducerConsumerUsingShieldedPlug/src/main/c/sp-producer.h)
* [sp-producer.c](/ProducerConsumerUsingShieldedPlug/src/main/c/sp-producer.c)


### Domain-specific producer code

Here we shall define an accelerometer "class" with the following contents
* attributes
	* sensor_ID (useful for tracing from which sensor the data comes from)
	* "parent" producer member (so as to reuse Domain-agnostic producer code)
* methods
	* initialisation method (will propagate initialisation to "parent")
	* adapter configuration function
		* with signature matching the one of the pointer to production function in the domain-agnostic producer struct
		* used as an adapter method to call a more specialized configuration function that can set implementation-specific info (such as pointer to Shielded Plug database)
	* adapter production function
		* with signature matching the one of the pointer to production function in the domain-agnostic producer struct
		* used as an adapter method to call a more specialized production function that can use domain-specific producer info (such as sensor_ID)
	* source code
		* [sp-producer-accelerometer.h](/ProducerConsumerUsingShieldedPlug/src/main/c/sp-producer-accelerometer.h)
		* [sp-producer-accelerometer.c](/ProducerConsumerUsingShieldedPlug/src/main/c/sp-producer-accelerometer.c)

### Instantiation code

The `SP_PRODUCER_init_factory` function instantiates two producers with different IDs and production periods

The source code is available in the following files.
* [sp-producer-factory.h](/ProducerConsumerUsingShieldedPlug/src/main/c/sp-producer-factory.h)
* [sp-producer-factory.c](/ProducerConsumerUsingShieldedPlug/src/main/c/sp-producer-factory.c)


# Integration

For reference, BSP toolchain integration related instructions are described in the [Producer Consumer Using Shielded Plug Integration](/STM32F429IDISCO-SNI_SP_FreeRTOS-CM4_ARMCC-configuration/README.md#producer-consumer-integration-using-shielded-plug-integration) section of the [STM32F429IDISCO-SNI_SP_FreeRTOS-CM4_ARMCC-configuration/README.md](/STM32F429IDISCO-SNI_SP_FreeRTOS-CM4_ARMCC-configuration/README.md) file.

These steps have already been done in this workspace and you do not need to repeat them.

However, you still need to perform the following operation :
uncomment the call to `SP_PRODUCER_init_factory` in the [main.c](/STM32F429IDISCO-SNI_SP_FreeRTOS-CM4_ARMCC-bsp/Project/MicroEJ/src/main.c) source file


	
## Checking the behavior
* After flashing the board, set up a terminal on the board serial port and press the reset input. You shall get an output similar to the one below :

		+ID : 3 {x : -48, y : -101, z : -18}
		-ID : 3 {x : -48, y : -101, z : -18}
		+ID : 4 {x : -82, y : -26, z : 14}
		-ID : 4 {x : -82, y : -26, z : 14}
		+ID : 1 {x : 45, y : 124, z : -24}
		-ID : 1 {x : 45, y : 124, z : -24}
		+ID : 2 {x : 27, y : -101, z : 82}
		-ID : 2 {x : 27, y : -101, z : 82}
		+ID : 1 {x : 6, y : -31, z : -23}
		-ID : 1 {x : 6, y : -31, z : -23}
		+ID : 3 {x : 101, y : 1, z : -51}
		-ID : 3 {x : 101, y : 1, z : -51}
		+ID : 4 {x : -48, y : -54, z : 103}
		-ID : 4 {x : -48, y : -54, z : 103}
		+ID : 2 {x : -120, y : 4, z : 7}
		-ID : 2 {x : -120, y : 4, z : 7}
		+ID : 1 {x : 23, y : 30, z : -30}
		-ID : 1 {x : 23, y : 30, z : -30}
		+ID : 3 {x : -119, y : 68, z : 61}
		-ID : 3 {x : -119, y : 68, z : 61}
		+ID : 4 {x : -2, y : -13, z : -95}
		-ID : 4 {x : -2, y : -13, z : -95}
		+ID : 1 {x : -115, y : 37, z : 20}
		-ID : 1 {x : -115, y : 37, z : 20}

* The '-' prefix indicates data consumption
* The '+' prefix indicates data production
* The number right after the ID indicates which sensor the data originates from. The 4 different IDs in the trace show us that data from our 4 different producers get consumed.


# Advanced use cases

The Use Case shown in this document only covers a most basic usage of the shared memory feature provided by Shielded Plug to solve a specific configuration of the producer consumer problem. Here, some data may be overwritten before getting a chance to be consumed. If this is an issue, the following options shall be considered :

* **Bufferization of the data being produced**: Here, only one data slot is available at any time, and it is overwritten if data has not been consumed faster than it was produced. Assuming production rate is not always greater than consumption rate, using a buffer able to accommodate production/consumption peak ratios can be considered.

* **Consumer-Producer synchronization**: If production can be paused, the consumer(s) could notify the producer(s) that consumable data has been processed. The producer(s) may then adapt its production to the (variable) consumption rate.
 
