[](Markdown)
[](Copyright 2016 IS2T. All rights reserved.)
[](For demonstration purpose only.)
[](IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.)

# Overview

This example shows an implementation of the producer consumer pattern.

## Dependencies

It relies on the following project:
- [ProducerConsumerData](/ProducerConsumerData) describes the data being exchanged

Here, we shall briefly list the projects and libraries required for building this example:

* Right-click on the project from MicroEJ.
	* Go to "Properties > Java Build Path", this will give us access to the project's build path dependencies
		* In the "Projects" tab:
			* [ProducerConsumerData](/ProducerConsumerData) describes the data being exchanged
		* In the "Libraries" tab, the following required MicroEJ API libraries are listed :
			* EDC-1.2
			* SP-1.0

## Producers/Consumers interaction
There are one consumer (written in Java) and three producers (one written in Java, two written in C).

Data is exchanged via a shared in-memory database using the Shielded Plug API.

* The producers overwrite data at a fixed period without checking if data has been consumed by any other task.
* The consumer gets new data from the database as soon as it is available.

# Java design

## Java producer class

Each producer runs on a dedicated thread. In pseudocode, the thread body roughly looks like this

	while (true)
	{
		data = generateData()
		deliver(data)
		wait(production_period)
	}
	
Because of the fixed periodic nature of the task and of its short execution time, a producer implementation based on Timer and TimerTask API is proposed in the [AccelerometerDataProducerTask.java](/ProducerConsumerUsingShieldedPlug/src/main/java/com/microej/examples/java2c/AccelerometerDataProducerTask.java) file.


## Java consumer class

Each consumer runs on a dedicated thread. In pseudocode, the thread body roughly looks like this

	while (true)
	{
		data = getNextAvailableData() //blocking call until some data is available
	}

The source code is available in the following files:
* [AccelerometerDataConsumer.java](/ProducerConsumerUsingShieldedPlug/src/main/java/com/microej/examples/java2c/AccelerometerDataConsumer.java)
* [AccelerometerDataUnmarshaller.java](/ProducerConsumerUsingShieldedPlug/src/main/java/com/microej/examples/java2c/AccelerometerDataUnmarshaller.java) (helper class for easy conversion from a shielded plug data block to a readily usable application domain object via a copy of byte array elements into a Java object )

## Starting the Java threads

As illustrated in the [ProducerConsumerExample.java](/ProducerConsumerUsingShieldedPlug/src/main/java/com/microej/examples/java2c/ProducerConsumerExample.java) source file, the Java producer and consumer Threads start-up is straightforward.

# C design

Since the actual data production is more likely to originate from a device with a driver implemented in C, an implementation of the producer written in C is provided.

The source code is available in the following files:
* [sp-producer-accelerometer.h](/ProducerConsumerUsingShieldedPlug/src/main/c/sp-producer-accelerometer.h)
* [sp-producer-accelerometer.c](/ProducerConsumerUsingShieldedPlug/src/main/c/sp-producer-accelerometer.c)


## C producer

In this section, we shall describe in more details the design of the **sp-producer-accelerometer** source files.

A producer is viewed as a task that must periodically call a produce function, which contents are actually domain specific.

* attributes (will be set by the domain-specific producer)
	* production period
	* sensor_ID (useful for tracing from which sensor the data comes from)
producer)
* methods
	* initialisation method (will start the production task)
	* taskbody (calls the production function at every production period expiration)


## Instantiation code

The `SP_PRODUCER_init_factory` function instantiates two producers with different IDs and production periods

The source code is available in the following files:
* [sp-producer-factory.h](/ProducerConsumerUsingShieldedPlug/src/main/c/sp-producer-factory.h)
* [sp-producer-factory.c](/ProducerConsumerUsingShieldedPlug/src/main/c/sp-producer-factory.c)


# Launch configuration parameters

## Shielded Plug database definition

One requirement of the Shielded Plug API is that you define the database schema via a configuration file.

The source code for that file is available here :
* [database-definition.xml](/ProducerConsumerUsingShieldedPlug/src/main/resources/database-definition.xml)

In this file, some database and field IDs are defined :

* Their C and Java counterparts (with matching values) are defined as :
	* **DATABASE_ID** and **DATABASE_FIELD_ID_ACCELEROMETER\*** in [ProducerConsumerExample.java](/ProducerConsumerUsingShieldedPlug/src/main/java/com/microej/examples/java2c/ProducerConsumerExample.java)
	* **ACCELEROMETER_SP_DATABASE_ID** and **ACCELEROMETER_SP_DATABASE_FIELD_ID_ACCELEROMETER\*** in [sp-producer-factory.c](/ProducerConsumerUsingShieldedPlug/src/main/c/sp-producer-factory.c)

This configuration file is a required parameter of the MicroEJ Application launch configuration when using the Shielded Plug API:
* Select **Run Configurations...**
* Select the (ProducerConsumerUsingShieldedPlug_Build)[[ProducerConsumerUsingShieldedPlug_Build.launch](/ProducerConsumerUsingShieldedPlug/launches/ProducerConsumerUsingShieldedPlug_Build.launch) ] configuration
* Go to the **Configuration** tab
* Select the **Shielded Plug** node
* You shall see that the **Database definition** field is pointing to the [database-definition.xml](/ProducerConsumerUsingShieldedPlug/src/main/resources/database-definition.xml) file

# Updating the default BSP project
## Adding the native source files to the BSP IDE project structure
* Adding the .h files
	* Right-click on the root node of your MicroVision project
	* Go to the **C/C++** tab
	* Click on the **...** button next to the **Include paths** field
	* Click on the **New** button
	* Click on the **...** button next to the newly created include path
		* Browse to the [src/main/c](/ProducerConsumerData/src/main/c) directory of the [/ProducerConsumerData](/ProducerConsumerData)  project
		* Click **OK**
		* Browse to the [src/main/c](/ProducerConsumerUsingShieldedPlug/src/main/c) directory of the  [/ProducerConsumerUsingShieldedPlug](/ProducerConsumerUsingShieldedPlug) project
		* Click **OK**
	* Click **OK**
* Adding the .c files
	* Select the root node of your project
	* Right-Click and select **Add Group** this will add a group called "New Group"
	* Select this group and hit **F2** key so as to rename it to "JavaNatives"
	* Right-Click on the **JavaNatives** group and select **Add Existing Files to group 'JavaNatives'...**
	* Move up the directory hierarchy until you get up to the parent folder of the [/ProducerConsumerData](/ProducerConsumerData) project
	* Go to the [src/main/c](/ProducerConsumerData/src/main/c) directory of the [/ProducerConsumerData](/ProducerConsumerData) project
		* Select all the .c files
		* Click **Add**
		* Click **Close**
	* Right-Click on the **JavaNatives** group and select **Add Existing Files to group 'JavaNatives'...**
	* Move up the directory hierarchy until you get up to the parent folder of the [ProducerConsumerUsingShieldedPlug](ProducerConsumerUsingShieldedPlug) project
	* Go to the [src/main/c](/ProducerConsumerUsingShieldedPlug/src/main/c) directory of the [ProducerConsumerUsingShieldedPlug](ProducerConsumerUsingShieldedPlug) project
		* Select all the .c files
		* Click **Add**
		* Click **Close**

## Updating the main.c file
* Add the following ```include``` statement at the start of the file :

		#include "sp-producer-factory.h"

* Insert the following function call

		SP_PRODUCER_init_factory();
	
	Before this line

		xTaskCreate( xJavaTaskFunction, "MicroJvm", JAVA_TASK_STACK_SIZE, NULL, JAVA_TASK_PRIORITY, NULL );

# Testing

* Run the [ProducerConsumerUsingShieldedPlug_Build.launch](/ProducerConsumerUsingQueues/launches/ProducerConsumerUsingQueues_Build.launch) launch configuration
* Uncomment the call to `SP_PRODUCER_init_factory` in the [main.c](/STM32F746GDISCO-SNI_SP-CM7_ARMCC-FreeRTOS-bsp/Projects/STM32746G-Discovery/Applications/MicroEJ/src/main.c) source file
* After flashing the board, set up a terminal on the board serial port and press the reset input. You shall get an output similar to the one below :

		-ID : 0 {x : -43, y : 4, z : -48}
		+ID : 0 {x : -43, y : 4, z : -48}
		+ID : 2 {x : -83, y : 108, z : -19}
		-ID : 2 {x : -83, y : 108, z : -19}
		-ID : 0 {x : -29, y : -61, z : 90}
		+ID : 0 {x : -29, y : -61, z : 90}
		-ID : 1 {x : 110, y : 41, z : -111}
		+ID : 1 {x : 110, y : 41, z : -111}
		+ID : 2 {x : 49, y : -107, z : -89}
		-ID : 2 {x : 49, y : -107, z : -89}
		-ID : 0 {x : 16, y : -20, z : -49}
		+ID : 0 {x : 16, y : -20, z : -49}
		-ID : 1 {x : 63, y : 38, z : 74}
		+ID : 1 {x : 63, y : 38, z : 74}
		-ID : 0 {x : -43, y : -51, z : -100}
		+ID : 0 {x : -43, y : -51, z : -100}
		+ID : 2 {x : -4, y : -125, z : -110}
		-ID : 2 {x : -4, y : -125, z : -110}
		-ID : 0 {x : -120, y : 59, z : 59}
		+ID : 0 {x : -120, y : 59, z : 59}
		+ID : 2 {x : -75, y : -89, z : 0}
		-ID : 2 {x : -75, y : -89, z : 0}


* The '-' prefix indicates data consumption
* The '+' prefix indicates data production
* The number right after the ID indicates which sensor the data originates from. The 4 different IDs in the trace show us that data from our 3 different producers get consumed.
