[](Markdown)
[](Copyright 2016 IS2T. All rights reserved.)
[](IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.)

# Overview

This example shows an implementation of the producer consumer pattern.

## Dependencies

Here, we shall briefly list the projects and libraries required for building this example :

* Right-click on the project from MicroEJ.
	* Go to "Properties > Java Build Path", this will give us access to the project's build path dependencies
		* In the "Projects" tab:
			* [ProducerConsumerData](/ProducerConsumerData) describes the data being exchanged
			* [NativeQueueWrapper](/NativeQueueWrapper) provides the required data delivery infrastructure, based on message queues.
		* In the "Libraries" tag, the following MicroEJ APIs are listed as required using variables:
			* EDC-1.2
			* SP-1.0
			* SNI-1.2.0

## Producers/Consumers interaction
For fixed size data, there are one consumer (written in Java) and three producers (one written in Java, two written in C).
For variable size data, there is one consumer (written in Java) and two producers (written in C).

* The producers send data at a fixed period if space is available in the relevant queue.
* The consumers get new data from the message queue as soon as it is available. If no data is available immediately, the queue wrapper will block until some becomes available.

# Java Design

## Fixed Size data exchange

Note that for fixed size data, we create both a consumer and a producer class in Java, but "force" them to communicate via our native message queue API wrapper instead of using regular Java APIs for cross-thread data exchange. We create an AccelerometerData producer/consumer ecosystem in Java so as to test the native message queue API wrapper.

### Java producer class

Usually, data production will be done from a C context and data consumption will be done from a Java context. We will however go through the effort of creating a Java producer class for pedagogical purposes and to ensure that the queue wrapper service allows bidirectional data exchange.

Each producer:

* runs on a dedicated thread
* posts data through delegation to a QueueService instance

In pseudocode, the thread body roughly looks like this

	queueService = createQueueService(QUEUE_ID)
	while (true)
	{
		data = generateData()
		queueService.write(data)
		wait(production_period)
	}
	
 The actual source code is in the [AccelerometerDataProducer.java](/ProducerConsumerUsingSNIAndImmortals/src/main/java/com/microej/examples/java2c/AccelerometerDataProducer.java) file.

#### Starting the producer thread

As illustrated in the [SNIAndImmortalsFixedSizeExample.java](/ProducerConsumerUsingSNIAndImmortals/src/main/java/com/microej/examples/java2c/SNIAndImmortalsFixedSizeExample.java) source file, once a **QueueService** is available, it is passed on to an **AccelerometerDataProducer** instance constructor that can then use it as a means to deliver its production while running its own thread.

### Java consumer class

Each consumer:
* runs on a dedicated thread
* retrieves data through delegation to a QueueService instance as soon as some is available

In pseudocode, the thread body roughly looks like this

	queueService = createQueueService(QUEUE_ID)
	while (true)
	{
		data = queueService.read() //blocking call until some data is available
	}

The source code is in the following file:
* [AccelerometerDataConsumer.java](/ProducerConsumerUsingSNIAndImmortals/src/main/java/com/microej/examples/java2c/AccelerometerDataConsumer.java) file.

#### Starting the consumer thread

As illustrated in the [SNIAndImmortalsFixedSizeExample.java](/ProducerConsumerUsingSNIAndImmortals/src/main/java/com/microej/examples/java2c/SNIAndImmortalsFixedSizeExample.java) source file, once a **QueueService** is available, it is passed on to an **AccelerometerDataConsumer** instance constructor that can then use it as a means to retrieve data from the queue while running its own thread, which is quite similar to the way the producer thread is started.

## Variable size data exchange

### Consumer class

The design is similar to the one used for fixed size data exchange

The source code is in the following files:
* [MessengerDataConsumer.java](/ProducerConsumerUsingSNIAndImmortals/src/main/java/com/microej/examples/java2c/MessengerDataConsumer.java) file.

#### Starting the consumer thread

As illustrated in the [SNIAndImmortalsVariableSizeExample.java](/ProducerConsumerUsingSNIAndImmortals/src/main/java/com/microej/examples/java2c/SNIAndImmortalsVariableSizeExample.java) source file, once a **QueueService** is available, it is passed on to a **MessengerDataConsumer** instance constructor that can then use it as a means  to retrieve data from the queue while running its own thread.

------------
# C design


Since the actual data production is more likely to originate from a device with a driver implemented in C, an implementation of the producer written in C is provided.

The source code is available in the following files :
* [sni-producer.h](/ProducerConsumerUsingSNIAndImmortals/src/main/c/sni-producer.h)
* [sni-producer.c](/ProducerConsumerUsingSNIAndImmortals/src/main/c/sni-producer.c)
* [sni-producer-accelerometer.h](/ProducerConsumerUsingSNIAndImmortals/src/main/c/sni-producer-accelerometer.h)
* [sni-producer-accelerometer.c](/ProducerConsumerUsingSNIAndImmortals/src/main/c/sni-producer-accelerometer.c)
* [sni-producer-messenger.h](/ProducerConsumerUsingSNIAndImmortals/src/main/c/sni-producer-messenger.h)
* [sni-producer-messenger.c](/ProducerConsumerUsingSNIAndImmortals/src/main/c/sni-producer-messenger.c)

Note that the partitioning between sp-producer and sp-producer-accelerometer files is done so as to abstract the production task mechanism and associated creation and synchronization (factored out in sp-producer) from the business domain logic (specialized in sp-producer-accelerometer) as much as possible.

## C "abstract" producer

In this section, we shall describe in more details the design of the **sni-producer** source files.

A producer is viewed as a task that must periodically call a produce function, which contents are actually domain specific.

This leads to a producer "class" with the following contents:

* attributes (will be set by the domain-specific producer)
	* production period
	* pointer to production function (with pointer to producer argument so as to be able to associate produced data with producer)
* methods
	* initialisation method (will start the production task)
	* taskbody (calls the production function at every production period expiration)

Although the design is to some extent object-oriented, the implementation in this example is in C, not in C++.

## C "concrete" producer

In this section, we shall describe in more details the design of the **sni-producer-accelerometer** and **sni-producer-messenger** source files.

This leads to:
* an accelerometer "class" with the following contents:
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
			
* a messenger "class" with the following contents:
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

### Instantiation code

The `SNI_PRODUCER_init_factory_accelerometer` and `SNI_PRODUCER_init_factory_messenger` functions both instantiate two producers with different IDs and production periods for each kind of data.

The source code is available in the following files.
* [sni-producer-factory.h](/ProducerConsumerUsingSNIAndImmortals/src/main/c/sni-producer-factory.h)
* [sni-producer-factory.c](/ProducerConsumerUsingSNIAndImmortals/src/main/c/sni-producer-factory.c)


# Testing
	
## For fixed size example

* Run the [SNI_And_Immortals_Fixed_Size_Example_Build.launch](/ProducerConsumerUsingSNIAndImmortals/launches/SNI_And_Immortals_Fixed_Size_Example_Build.launch) launch configuration
* Uncomment the call to `	SNI_PRODUCER_init_factory_accelerometer()` in the [main.c](/STM32F429IDISCO-SNI_SP_FreeRTOS-CM4_ARMCC-bsp/Project/MicroEJ/src/main.c) source file
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
* The number right after the ID indicates which sensor or sender the data originates from. The 3 different IDs in the trace {{1,2} : C Accelerometers, 3 : Java Accelerometer} show us that data from our 3 different producers gets produced and consumed.

## Testing for variable size example

* Run the [SNI_And_Immortals_Variable_Size_Example_Build.launch](/ProducerConsumerUsingSNIAndImmortals/launches/SNI_And_Immortals_Variable_Size_Example_Build.launch) launch configuration
* Uncomment the call to `	SNI_PRODUCER_init_factory_messenger()` in the [main.c](/STM32F429IDISCO-SNI_SP_FreeRTOS-CM4_ARMCC-bsp/Project/MicroEJ/src/main.c) source file
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

* The number right after the ID indicates which sensor or sender the data originates from. The 2 different IDs in the trace {{11,22} : C Messengers} show us that data from our 2 different producers gets produced and consumed.