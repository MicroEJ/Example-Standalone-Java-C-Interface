[](Markdown)
[](Copyright 2016 IS2T. All rights reserved.)
[](IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.)

# Overview

This example shows an implementation of the producer consumer pattern.

There are two kinds of data produced :
- fixed size (cf [AccelerometerData.java](/ProducerConsumerData/src/main/java/com/microej/examples/java2c/AccelerometerData.java) source file).
- variable size (cf [MessengerData.java](/ProducerConsumerData/src/main/java/com/microej/examples/java2c/MessengerData.java) source file). 

For fixed size data, there is one consumer (written in Java) and three producers (one written in Java, two written in C).
For variable size data, there is one consumer (written in Java) and two producers (written in C).

Data is exchanged via message queues. A Java wrapper has been created to access the C message queues API. This wrapper is defined in the [NativeQueueWrapper](/NativeQueueWrapper) project.

* The producers send data at a fixed period if space is available in the relevant queue.
* The consumers get new data from the message queue as soon as it is available. If no data is available immediately, the queue wrapper will block until some becomes available.


The following steps will be taken :

* Project setup
* Java design
	* Fixed size data exchange
		* Java data class
		* Java producer class
		* Java consumer class
	* Variable size data exchange
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

## Fixed size data exchange

Note that we create both a consumer and a producer class in Java, but "force" them to communicate via our native message queue API wrapper instead of using regular Java APIs for cross-thread data exchange. We create an AccelerometerData producer/consumer ecosystem in Java so as to test the native message queue API wrapper.

### Java data class

In this section, we shall create a class to represent the data being exchanged between producer(s) and consumer(s).

For fixed size data, the source code is in the [AccelerometerData.java](/ProducerConsumerData/src/main/java/com/microej/examples/java2c/AccelerometerData.java) file.

### Java producer class

In this section, we shall create a class to produce data sporadically.

Each producer :

* runs on a dedicated thread
* posts data through delegation to a QueueService instance

In pseudocode, the thread body roughly looks like this

	queueService = createQueueuService(QUEUE_ID)
	while (true)
	{
		data = generateData()
		queueService.write(data)
		wait(production_period)
	}
	
 The actual source code is in the [AccelerometerDataProducer.java](/ProducerConsumerUsingSNIAndImmortals/src/main/java/com/microej/examples/java2c/AccelerometerDataProducer.java) file.

#### Starting the producer thread

* Open the [SNIAndImmortalsFixedSizeExample.java](/ProducerConsumerUsingSNIAndImmortals/src/main/java/com/microej/examples/java2c/SNIAndImmortalsFixedSizeExample.java) source file

* Update the main() method using the code below
	
		public static void main(String[] args) {
				QueueService accelerometerQueue = new QueueService(AccelerometerData.getQueuePtr());

				AccelerometerDataProducer accelerometerDataProducer = new AccelerometerDataProducer(ACCELEROMETER_DATA_QUEUE_ID, 1100, 3);
				new Thread(accelerometerDataProducer).start();
		}


### Java consumer class

In this section, we shall create a class to consume data as soon as it is available.

Each consumer :
* runs on a dedicated thread
* retrieves data through delegation to a QueueService instance

In pseudocode, the thread body roughly looks like this

	queueService = createQueueuService(QUEUE_ID)
	while (true)
	{
		data = queueService.read() //blocking call until some data is available
	}

The source code is in the following files :
* [AccelerometerDataConsumer.java](/ProducerConsumerUsingSNIAndImmortals/src/main/java/com/microej/examples/java2c/AccelerometerDataConsumer.java) file.

#### Starting the consumer thread

* Open the [SNIAndImmortalsFixedSizeExample.java](/ProducerConsumerUsingSNIAndImmortals/src/main/java/com/microej/examples/java2c/SNIAndImmortalsFixedSizeExample.java) source file

* Update the main() method using the code below
	
		public static void main(String[] args) {
				QueueService accelerometerQueue = new QueueService(AccelerometerData.getQueuePtr());

				AccelerometerDataProducer accelerometerDataProducer = new AccelerometerDataProducer(ACCELEROMETER_DATA_QUEUE_ID, 1100, 3);
				new Thread(accelerometerDataProducer).start();
		
				AccelerometerDataConsumer accelerometerDataConsumer = new AccelerometerDataConsumer(ACCELEROMETER_DATA_QUEUE_ID);
				new Thread(accelerometerDataConsumer).start();
		}

## Variable size data exchange

Note that we do not create a producer in Java for MessengerData, we will do that later using C.

### Java data class

In this section, we shall create a class to represent the data being exchanged between producer(s) and consumer(s).

For variable size data, the source code is in the [MessengerDataConsumer.java](/ProducerConsumerData/src/main/java/com/microej/examples/java2c/MessengerData.java) file.

### Java consumer class

In this section, we shall create a class to consume data as soon as it is available.

Each consumer :
* runs on a dedicated thread
* retrieves data through delegation to a QueueService instance

In pseudocode, the thread body roughly looks like this

	queueService = createQueueuService(QUEUE_ID)
	while (true)
	{
		data = queueService.read() //blocking call until some data is available
	}

The source code is in the following files :
* [MessengerDataConsumer.java](/ProducerConsumerUsingSNIAndImmortals/src/main/java/com/microej/examples/java2c/MessengerDataConsumer.java) file.

#### Starting the consumer thread

* Open the [SNIAndImmortalsVariableSizeExample.java](/ProducerConsumerUsingSNIAndImmortals/src/main/java/com/microej/examples/java2c/SNIAndImmortalsVariableSizeExample.java) source file

* Update the main() method using the code below
	
		public static void main(String[] args) {
			QueueService messengerQueue = new QueueService(MessengerData.getQueuePtr());
			
			MessengerDataConsumer messengerConsumer = new MessengerDataConsumer(messengerQueue);
			new Thread(messengerConsumer).start();
		}


# C design

We shall now design a way to generate data from the C world, since the actual data production is more likely to originate from a device with a driver implemented in C.

## C data structs

Let us define some
* structs to hold the data **Accelerometer_data_t** and **Messenger_data_t**
* methods to generate/retrieve the data (and trace where it comes from - that is what the **Accelerometer_data_t::sensor_ID** and **Messenger_data_t::sender_ID** fields are for)
* debug helper functions (the **print** functions)

The source code is available in the following files :
* [accelerometer-data.h](/ProducerConsumerData/src/main/c/accelerometer-data.h)
* [accelerometer-data.c](/ProducerConsumerData/src/main/c/accelerometer-data.c)
* [messenger-data.h](/ProducerConsumerData/src/main/c/messenger-data.h)
* [messenger-data.c](/ProducerConsumerData/src/main/c/messenger-data.c)

## C producer tasks

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
* [sni-producer.h](/ProducerConsumerUsingSNIAndImmortals/src/main/c/sni-producer.h)
* [sni-producer.c](/ProducerConsumerUsingSNIAndImmortals/src/main/c/sni-producer.c)


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
	* source code
		* [sni-producer-accelerometer.h](/ProducerConsumerUsingSNIAndImmortals/src/main/c/sni-producer-accelerometer.h)
		* [sni-producer-accelerometer.c](/ProducerConsumerUsingSNIAndImmortals/src/main/c/sni-producer-accelerometer.c)
			
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
	* source code
		* [sni-producer-messenger.h](/ProducerConsumerUsingSNIAndImmortals/src/main/c/sni-producer-messenger.h)
		* [sni-producer-messenger.c](/ProducerConsumerUsingSNIAndImmortals/src/main/c/sni-producer-messenger.c)

### Instantiation code

The `SNI_PRODUCER_init_factory` function delegates to `SNI_PRODUCER_init_factory_accelerometer` and `SNI_PRODUCER_init_factory_messenger` which both instantiate two producers with different IDs and production periods for each kind of data.

The source code is available in the following files.
* [sni-producer-factory.h](/ProducerConsumerUsingSNIAndImmortals/src/main/c/sni-producer-factory.h)
* [sni-producer-factory.c](/ProducerConsumerUsingSNIAndImmortals/src/main/c/sni-producer-factory.c)


# Integration

For reference, BSP toolchain integration related instructions are described in the [Produced Consumer Using SNI and Immortals Integration](/STM32F429IDISCO-SNI_SP_FreeRTOS-CM4_ARMCC-configuration/README.md#producer-consumer-using-sni-and-immortals-integration) section of the [STM32F429IDISCO-SNI_SP_FreeRTOS-CM4_ARMCC-configuration/README.md](/STM32F429IDISCO-SNI_SP_FreeRTOS-CM4_ARMCC-configuration/README.md) file.

These steps have already been done in this workspace and you do not need to repeat them.

	
## Checking the behavior for fixed size example

* You still need to perform the following operation :
	uncomment the call to `	SNI_PRODUCER_init_factory_accelerometer()` in the [main.c](/STM32F429IDISCO-SNI_SP_FreeRTOS-CM4_ARMCC-bsp/Project/MicroEJ/src/main.c) source file

* After flashing the board, set up a terminal on the board serial port and press the reset input. You shall get an output similar to the one below :

		-ID : 1 {x : -24, y : 27, z : -101}
		+ID : 2 {x : 82, y : 6, z : -31}
		-ID : 2 {x : 82, y : 6, z : -31}
		+ID : 3 {x : -67, y : -85, z : 65}
		-ID : 3 {x : -67, y : -85, z : 65}
		+ID : 1 {x : -23, y : -120, z : 4}
		-ID : 1 {x : -23, y : -120, z : 4}
		+ID : 2 {x : 7, y : 23, z : 30}
		-ID : 2 {x : 7, y : 23, z : 30}
		+ID : 1 {x : -30, y : -115, z : 37}
		-ID : 1 {x : -30, y : -115, z : 37}
		+ID : 3 {x : -75, y : -79, z : -4}
		-ID : 3 {x : -75, y : -79, z : -4}
		+ID : 2 {x : 20, y : -96, z : 115}
		-ID : 2 {x : 20, y : -96, z : 115}
		+ID : 1 {x : 19, y : 106, z : 62}
		-ID : 1 {x : 19, y : 106, z : 62}
		+ID : 2 {x : -71, y : -31, z : 32}
		-ID : 2 {x : -71, y : -31, z : 32}
		+ID : 3 {x : 114, y : -10, z : -64}
		-ID : 3 {x : 114, y : -10, z : -64}
		+ID : 1 {x : -68, y : 95, z : -113}
		-ID : 1 {x : -68, y : 95, z : -113}
		+ID : 2 {x : 54, y : 26, z : 101}
		-ID : 2 {x : 54, y : 26, z : 101}

* The '-' prefix indicates data consumption
* The '+' prefix indicates data production
* The number right after the ID indicates which sensor or sender the data originates from. The 3 different IDs in the trace {{1,2} : C Accelerometers, 3 : Java Accelerometer, {11,22} C Messengers} show us that data from our 3 different producers gets produced and consumed.


## Checking the behavior for variable size example

* You still need to perform the following operation :
	uncomment the call to `	SNI_PRODUCER_init_factory_messenger()` in the [main.c](/STM32F429IDISCO-SNI_SP_FreeRTOS-CM4_ARMCC-bsp/Project/MicroEJ/src/main.c) source file
	
* After flashing the board, set up a terminal on the board serial port and press the reset input. You shall get an output similar to the one below :

		VM START
		+ID : 11 {ID : 11.1. This i1 {1.1. Ths the firstis is the  line}
		first line}
		+ID : 22 {ID : 22.1. File #2 {2.1. Fi2 has diffele #2 has rent lines,different  some of whlines, somich are lone of whichger}
		 are longer}
		+ID : 11 {-ID : 1.2. This i11 {1.2. Ts the next his is the line}
		next line}
		+ID : 11 {-ID : 1.3. This i11 {1.3. Ts the finalhis is the  line}
		final line}
		+ID : 22 {ID : 22.2. But it2 {2.2. Bu only has tt it only wo lines}
		has two lines}
		+ID : 11 {-ID : 2.1. File #11 {2.1. F2 has diffeile #2 hasrent lines, different some of wh lines, soich are lonme of whicger}
		h are longer}
		+ID : 11 {ID : 12.2. But it1 {2.2. Bu only has tt it only wo lines}
		has two lines}
		+ID : 22 {1.1. This i-ID : 22s the firs {1.1. Thist line}
		 is the first line}
		+ID : 11 {-ID : 1.1. This i11 {1.1. Ts the firsthis is the  line}
		first line}
		+ID : 22 {-ID : 1.2. This i22 {1.2. Ts the next his is theline}
		 next line}
		+ID : 11 {-ID : 1.2. This i11 {1.2. Ts the next his is the line}
		next line}
		+ID : 11 {-ID : 1.3. This i11 {1.3. Ts the finalhis is the line}
		 final line}
		+ID : 22 {-ID : 1.3. This i22 {1.3. Ts the finalhis is the line}
		 final line}
		+ID : 11 {2.1. File #-ID : 112 has diffe {2.1. Filrent lines,e #2 has d some of whifferent lich are lonines, someger}
		 of which are longer}
		+ID : 11 {ID : 12.2. But it1 {2.2. Bu only has t it only htwo lines}as two line
		s}
		+ID : 22 {ID : 22.1. File #2 {2.1. Fi2 has diffele #2 has rent lines,different  some of whlines, somich are loe of which nger}
		are longer}
		+ID : 22 {ID : 22.2. But it2 {2.2. Bu only has tt it only wo lines}
		has two lines}
		+ID : 11 {ID : 11.1. This i1 {1.1. Ths the firstis is the  line}
		first line}
		+ID : 11 {-ID : 1.2. This i11 {1.2. Ths the nextis is the  line}
		next line}
		+ID : 11 {-ID : 1.3. This i11 {1.3. Ts the finalhis is the line}
		 final line}
		+ID : 11 {2.1. File #-ID : 112 has diffe {2.1. Filrent lines,e #2 has d some of whifferent lich are lonines, someger}
		 of which are longer}
		+ID : 22 {ID : 21.1. This i2 {1.1. Ths the firstis is the  line}
		first line}
		+ID : 11 {ID : 12.2. But it1 {2.2. Bu


Note that the trace for long messages from both producer and consumer do get mixed up in the output because the producer outputs after successfully posting the data, not before. Therefore, between the posting time and the flushing of the producer trace, the consumer has time to output some trace of its own on the shared output stream.

# Advanced use cases

The Use Case shown in this document only covers a specific configuration of the producer consumer problem. Some other important use cases might be considered such as :
* allocating different processing priorities depending on the message queue
* using priority queues where some items are to be processed faster than some others

