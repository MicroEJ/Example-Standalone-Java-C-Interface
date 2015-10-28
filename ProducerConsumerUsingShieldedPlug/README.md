[](Markdown)
[](Copyright 2015 IS2T. All rights reserved.)
[](IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.)

# Overview

This example shows an implementation of the producer consumer pattern.

There is one consumer (written in Java) and three producers (one written in Java, two written in C).

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

In this section, we shall create a class to represent the data being exchanged between producer(s) and consumer(s). The source code is in the [AccelerometerData.java](/ProducerConsumerUsingShieldedPlug/src/main/java/com/is2t/examples/java2C/AccelerometerData.java) file.


# Java producer class

In this section, we shall create a class to produce data sporadically.

Each producer runs on a dedicated thread. In pseudocode, the thread body roughly looks like this

	while (true)
	{
		data = generateData()
		produce(data)
		wait(production_period)
	}
	
 The actual source code is in the [AccelerometerDataProducer.java](/ProducerConsumerUsingShieldedPlug/src/main/java/com/is2t/examples/java2C/AccelerometerDataProducer.java) file. 

## Starting the producer thread

* Open the [DataConsumerExample.java](/ProducerConsumerUsingShieldedPlug/src/main/java/com/is2t/examples/java2C/DataConsumerExample.java) source file
* Replace the main() method by the code below
	
	public static void main(String[] args) {
		AccelerometerDataProducer producer = new AccelerometerDataProducer(3,1200);
		new Thread(producer).start();
	}

# Java consumer class

In this section, we shall create a class to consume data as soon as it is available.

Each consumer runs on a dedicated thread. In pseudocode, the thread body roughly looks like this

	while (true)
	{
		data = getNextData() //blocking call until some data is available
	}

The source code is in the [AccelerometerDataConsumer.java](/ProducerConsumerUsingShieldedPlug/src/main/java/com/is2t/examples/java2C/AccelerometerDataConsumer.java) file.

## Starting the consumer thread

* Open the [DataConsumerExample.java](/ProducerConsumerUsingShieldedPlug/src/main/java/com/is2t/examples/java2C/DataConsumerExample.java) source file
* Update the main() method using the code below
	
	public static void main(String[] args) {

		AccelerometerDataConsumer consumer = new AccelerometerDataConsumer();
		AccelerometerDataProducer producer = new AccelerometerDataProducer(3,1200);
		new Thread(consumer).start();
		new Thread(producer).start();
	}

# C design

Now that we are confident with the production-consumption mechanism in Java, we shall design a way to generate data from the C world, since the actual data production is more likely to originate from a device with a driver implemented in C.

## C data struct

Let us define :
* some struct to hold the data
* some method to generate/retrieve the data (and trace where it comes from - that is what the **sensor_ID** field is for)
* some debug helper function (the **print** function)

The source code is available in the following files.
* [accelerometer-data.h](/ProducerConsumerUsingShieldedPlug/src/main/c/accelerometer-data.h)
* [accelerometer-data.c](/ProducerConsumerUsingShieldedPlug/src/main/c/accelerometer-data.c)

## C producer task

In this section, we shall create C code to produce data sporadically.

### Domain-agnostic producer code

For reuse purposes, we shall separate the producer-consumer pattern implementation from the sensor-oriented domain logic.

Here we shall define that a producer is a task that must periodically call a produce function, which contents are actually domain specific.

This leads us to defining a producer "class" with the following contents
* attributes (will be set by the domain-specific producer)
	* production period
	* pointer to production function (with pointer to producer argument so as to be able to associate produceed data with producer)
* methods
	* initialisation method (will start the production task)
	* taskbody (calls the production function at every production period expiration) 

Although the design is to some extent object-oriented, the implementation in this example is in C, not in C++.

The source code is available in the following files.
* [producer.h](/ProducerConsumerUsingShieldedPlug/src/main/c/sp-producer.h)
* [producer.c](/ProducerConsumerUsingShieldedPlug/src/main/c/sp-producer.c)


### Domain-specific producer code

Here we shall define an accelerometer "class" with the following contents
* attributes
	* sensor_ID (useful for tracing from which sensor the data comes from)
	* "parent" producer member (so as to reuse Domain-agnostic producer code)
* methods
	* initialisation method (will propagate initialisation to "parent")
	* adapter production function
		* with signature matching the one of the pointer to production function in the domain-agnostic producer struct
		* used as an adapter method to call a more specialized production function that can use domain-specific producer info (such as sensor_ID)
		
### Instantiation code

The `PRODUCER_accelerometer_init_ALL` function instantiates two producers with different IDs and production periods

The source code is available in the following files.
* [producer-accelerometer.h](/ProducerConsumerUsingShieldedPlug/src/main/c/sp-producer-accelerometer.h)
* [producer-accelerometer.c](/ProducerConsumerUsingShieldedPlug/src/main/c/sp-producer-accelerometer.c)


# Integration

For reference, BSP toolchain integration related instructions are described in the [Produced Consumer Integration](/STM32F429IDISCO-SNI_SP_FreeRTOS-CM4_ARMCC-configuration/README.md#producer-consumer-integration) section of the [STM32F429IDISCO-SNI_SP_FreeRTOS-CM4_ARMCC-configuration/README.md](/STM32F429IDISCO-SNI_SP_FreeRTOS-CM4_ARMCC-configuration/README.md) file.

These steps have already been done in this workspace and you do not need to repeat them.

## Checking the behavior
* After flashing the board, set up a terminal on the board serial port and press the reset input. You shall get an output similar to the one below :

	-ID : 2 {x : -68, y : 40, z : 91}
	+ID : 1 {x : -37, y : -110, z : 70}
	-ID : 1 {x : -37, y : -110, z : 70}
	+ID : 3 {x : -68, y : -122, z : 106}
	-ID : 3 {x : -68, y : -122, z : 106}
	+ID : 1 {x : 33, y : 41, z : -56}
	-ID : 1 {x : 33, y : 41, z : -56}
	+ID : 2 {x : 68, y : 71, z : 87}
	-ID : 2 {x : 68, y : 71, z : 87}
	+ID : 3 {x : -100, y : -76, z : -86}
	-ID : 3 {x : -100, y : -76, z : -86}
	+ID : 1 {x : 94, y : 34, z : -51}
	-ID : 1 {x : 94, y : 34, z : -51}
	+ID : 2 {x : 101, y : 84, z : -32}
	-ID : 2 {x : 101, y : 84, z : -32}
	+ID : 1 {x : -77, y : 83, z : -86}
	-ID : 1 {x : -77, y : 83, z : -86}
	+ID : 3 {x : 76, y : -24, z : 54}

* The '-' prefix indicates data consumption
* The '+' prefix indicates data production
* The number right after the ID indicates which sensor the data originates from


# Additional references

## Advanced use cases

The Use Case shown in this document only covers a most basic usage of the shared memory feature provided by Shielded Plug. Here, some data may be overwritten before getting a chance to be consumed. If this is an issue, the following options shall be considered :

* **Bufferization of the data being produced**: Here, only one data slot is available at any time, and it is overwritten if data has not been consumed faster than it was produced. Assuming production rate is not always greater than consumption rate, using a buffer able to accomodate production/consumption peak ratios can be considered.

* **Consumer-Producer synchronization**: If production can be paused, the consumer(s) could notify the producer(s) that consumable data has been processed. The producer(s) may then adapt its production to the (variable) consumption rate.
 
