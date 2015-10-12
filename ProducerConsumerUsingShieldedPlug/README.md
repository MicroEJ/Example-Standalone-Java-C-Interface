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
* Building a Java Platform
	* Building a skeleton Java executable
	* Running Java on the board (BSP specific)
* Java design
	* Java data class
	* Java producer class
	* Java consumer class
* C design
	* C data struct
	* C producer task
* Integration
	* Spawning the C tasks from the main C program
	* Building the updated C project
	* Deploying the binary

## Requirements

* JRE 7 (or later) x86.
* MicroEJ 3.1 or later.
* BSP specific toolchain with RTOS support

## Adapting this example to your BSP toolchain

This document shows the process for a STM32F429I-Disco (&trade;) target using a Keil MicroVision (&trade;) toolchain and FreeRTOS.

The BSP toolchain specific steps are marked *(BSP specific)*

Please refer to the relevant "Build a Java Platform" guide in the "Getting started" section of MicroEJ Resource Center for indications on how to adapt this process to your specific target.


# Project setup
## Building a Java Platform

We need to generate and build a Java Platform (JPF), i.e. the set of object files and libraries required to execute Java code on the target platform linked by an executable C program.

### (Optional) Recreating the platform (BSP Specific)

The platform provided is **based** on the [STM32F429I-DISCO Platform \(Keil,FreeRTOS\)](http://developer.microej.com/ivy/artifacts/com/is2t/CM4/examples/st/STM32F429IDISCO-HMI-CM4_ARMCC-FreeRTOS-eval/1.0.0/STM32F429IDISCO-HMI-CM4_ARMCC-FreeRTOS-eval_src-1.0.0.zip) platform available from [developer.microej.com](http://developer.microej.com)

However, as such, this platform does not provided Shielded Plug support. Besides, the UI support is not needed for this example. Therefore, 
the following modifications have been performed on the platform:

#### Renaming of the platform

So as to avoid confusion, the platform has been renamed to **STM32F429IDISCO-SNI_SP_FreeRTOS-CM4_ARMCC**

#### Selection of the modules

* From the **Content** tab of the [example.platform](STM32F429IDISCO-SNI_SP_FreeRTOS-CM4_ARMCC-configuration/example.platform) file
	* In the **modules** frame
		* Click on the **Show/Hide modules** button
		* In the **modules** list, check **Java to C Interface**
		
#### Stripping the UI support
* BSP UI support has been removed
* microuiGeneratorExtension project has been removed

#### Implementation of the Shielded Plug API in C

* Add [LLSP_FreeRTOS.c](/STM32F429IDISCO-SNI_SP_FreeRTOS-CM4_ARMCC-bsp/Project/MicroEJ/src/LLSP_FreeRTOS.c) source file to the (/STM32F429IDISCO-SNI_SP_FreeRTOS-CM4_ARMCC-bsp/Project/MicroEJ/src)[STM32F429IDISCO-SNI_SP_FreeRTOS-CM4_ARMCC-bsp/Project/MicroEJ/src] directory
* From the **Project Explorer** view
	* Double-click on the [Project.uvproj](/STM32F429IDISCO-SNI_SP_FreeRTOS-CM4_ARMCC-bsp/Project/MicroEJ/MDK-ARM/Project.uvproj) file (this will open the BSP project in the MicroVision IDE)
* From the MicroVision IDE
* Right-click on the **STM32F429i-DISCO>MicroEJ** node of your project and select **Add Existing Files to group 'MicroEJ'**
	* Select the [LLSP_FreeRTOS.c](/STM32F429IDISCO-SNI_SP_FreeRTOS-CM4_ARMCC-bsp/Project/MicroEJ/src/LLSP_FreeRTOS.c) source file 
	* Click **Add**
	* Click **Close

### Importing existing platform configuration (BSP Specific)

* Select **File > Import... > Existing Projects into Workspace** menu item
	* Click on **Select archive file** radio button
	* Click **Browse** and select the STM32F429IDISCO-SNI_SP_FreeRTOS-CM4_ARMCC.zip file
	* Check the **Copy projects into workspace**
	* Click on **Finish**

### Build
* Open the [example.platform](STM32F429IDISCO-SNI_SP_FreeRTOS-CM4_ARMCC-configuration/example.platform) file in the [(STM32F429IDISCO-SNI_SP_FreeRTOS-CM4_ARMCC-configuration)](STM32F429IDISCO-SNI_SP_FreeRTOS-CM4_ARMCC-configuration) project
* From the **Overview** tab of the generated example.platform file
	* Click on the **Build platform** hyperlink. This will generate the following project :
		* [STM32F429IDISCO-SNI_SP_FreeRTOS-CM4_ARMCC-1.0.0](STM32F429IDISCO-SNI_SP_FreeRTOS-CM4_ARMCC-1.0.0)
* Click on **Window > Preferences > MicroEJ**
	* In the **MicroEJ Repository** frame
		* Click on the **Refresh** button 

## Building a skeleton Java executable

In this section, we will create a skeleton Java program that does nothing relevant (yet), but is required so as to generate a proper BSP toolchain project from MicroEJ.

### MicroEJ project creation

This project will host the application specific code

* Select **File > New > Java Project** menu item
	* Set **Project Name** field to **"ProducerConsumerUsingShieldedPlug"**
	* In the **Runtime Environment** frame, select the following MicroEJ Libraries :
		* EDC 1.2 (selected by default)
		* SP 1.0 (will be required later)
* Click on **Next**
* Click on **Finish**


### Java Class creation
* Select **File > New > Source Folder** menu item
	* Set the **Folder name** field to /src/main/java
* Select **File > New > Class** menu item
	* Set the **Source folder** field to "ProducerConsumerUsingShieldedPlug/src/main/java"
	* Set the **Package** field to "com.is2t.examples.java2C"
	* Set the **Name** field to "DataConsumerExample"
	* Click on **Finish**
	* Copy and paste the following code inside the generated [DataConsumerExample.java](ProducerConsumerUsingShieldedPlug/src/main/java/com/is2t/examples/java2C/DataConsumerExample.java) file 

			package com.is2t.examples.java2C;
			public class DataConsumerExample {
			
					public static void main(String[] args) {
						while (true) {
							System.out.println("-- main alive --");
							try {
									Thread.sleep(2000);
							} catch (InterruptedException e) {
									// It is OK to ignore because this is just a basic example
							}
						}
					}
				
			}


### Compiling the Java code for the target

Here, we will create a MicroEJ "Run Configuration" that will compile the Java code we just created for the JPF we created previously

* Select **File > New > Folder** menu item
	* Set the **Enter or select the parent folder** field to "ProducerConsumerUsingShieldedPlug" (Project root)
	* Set the **Folder Name** field to "launches"
* Select **Run Configurations...** from the **Run configurations** drop down list (or Press Ctrl+Shift+3 and type "Run Configurations...")
* Select **MicroEJ Application** group and click **New**
	* Set **Name** field to "ProducerConsumerUsingShieldedPlug_Build"
	* In **Main** tab
		* Set the **Project** field to "ProducerConsumerUsingShieldedPlug"
		* Click on **Select Main type...** and type DataConsumerExample
	* In **Execution** tab
		* In **Target** frame
			* Click the **Browse** button next to the JPF Field and select your platform
		* In **Execution** frame
			* Select **Execute on EmbJPF** radio button
			* Leave the **Output Folder** field set to "${project_loc:ProducerConsumerUsingShieldedPlug}"
	* In **Configuration** tab
		* Select the **Target > Deploy** Node
			* Set the **Means** field to "Copy at a location known by BSP Project"
	* In **Common** tab
		* In **Save as** frame
			* Select the **Shared file** radio button
			* Click on **Browse** and select [ProducerConsumerUsingShieldedPlug/launches](ProducerConsumerUsingShieldedPlug/launches) folder	
	* Click on "Run"

## Running Java on the board (BSP specific)

### Opening the generated C project
* From the **Project Explorer** view
	* Navigate to the [STM32F429IDISCO-SNI_SP_FreeRTOS-CM4_ARMCC-bsp/Project/MicroEJ/MDK-ARM](STM32F429IDISCO-SNI_SP_FreeRTOS-CM4_ARMCC-bsp/Project/MicroEJ/MDK-ARM) folder
	* Double-click on the [Project.uvproj](/STM32F429IDISCO-SNI_SP_FreeRTOS-CM4_ARMCC-bsp/Project/MicroEJ/MDK-ARM/Project.uvproj) file (this will open the BSP project in the MicroVision IDE)

### Building the binary
* From the MicroVision IDE
	* Select **Project > Build Target** menu item (or press F7 keyboard shortcut)

### Flashing the board (BSP specific)
* Connect your board
* Select **Flash > Download** menu item (or press F8 keyboard shortcut).

### Checking the behavior
* Set up a terminal on the board serial port and press the reset input. You shall get the following output :

	START
	VM START
	-- main alive --
	-- main alive --
	-- main alive --
	-- main alive --
	...

# Java design

Before interfacing with the C world, we shall design a Java-only system that exchanges data between produced and consumer via the shielded plug mechanism

## Java data class

In this section, we shall create a class to represent the data being exchanged between producer(s) and consumer(s). The source code is in the [AccelerometerData.java](/ProducerConsumerUsingShieldedPlug/src/main/java/AccelerometerData.java) file.


# Java producer class

In this section, we shall create a class to produce data sporadically.

Each producer runs on a dedicated thread. In pseudocode, the thread body roughly looks like this

	while (true)
	{
		data = generateData()
		produce(data)
		wait(production_period)
	}
	
 The actual source code is in the [AccelerometerDataProducer.java](/ProducerConsumerUsingShieldedPlug/src/main/java/AccelerometerDataProducer.java) file. 

## Starting the producer thread

* Open the [DataConsumerExample.java](/ProducerConsumerUsingShieldedPlug/src/main/java/com/is2t/examples/java2C/DataConsumerExample.java) source file
* Replace the main() method by the code below
	
	public static void main(String[] args) {
		AccelerometerDataProducer producer = new AccelerometerDataProducer(3,1200);
		new Thread(producer).start();
	}

# J	ava consumer class

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
* [producer.h](/ProducerConsumerUsingShieldedPlug/src/main/c/producer.h)
* [producer.c](/ProducerConsumerUsingShieldedPlug/src/main/c/producer.c)


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
* [producer-accelerometer.h](/ProducerConsumerUsingShieldedPlug/src/main/c/producer-accelerometer.h)
* [producer-accelerometer.c](/ProducerConsumerUsingShieldedPlug/src/main/c/producer-accelerometer.c)


# Integration

## Spawning the C tasks from the main C program

We need to modify the main C program so as to call our producers instantiation code.

### Opening the generated C project (BSP specific)
* From the **Project Explorer** view
	* Open the [main.c](/STM32F429IDISCO-SNI_SP_FreeRTOS-CM4_ARMCC-bsp/Project/MicroEJ/src/main.c) source file
	* Add `#include "producer-accelerometer.h"` to the include directives
	* In the `main()` function, insert a call to `	PRODUCER_accelerometer_init_ALL()` function just before the following line
			xTaskCreate( xJavaTaskFunction, NULL, JAVA_TASK_STACK_SIZE, NULL, JAVA_TASK_PRIORITY, NULL );
			
## Building the updated C project
### Adding the C source files to the generated C project (BSP specific)

#### Opening the generated C project
* From the **Project Explorer** view
	* Double-click on the [Project.uvproj](/STM32F429IDISCO-SNI_SP_FreeRTOS-CM4_ARMCC-bsp/Project/MicroEJ/MDK-ARM/Project.uvproj) file (this will open the BSP project in the MicroVision IDE)
	
#### Updating the include path
* From the MicroVision IDE
* Right-click on the root node of your project (the one called **STM32F429i-DISCO**)
* Select **Options for Target STM32F429i-DISCO**
	* Go to C/C++ tab
	* Click on **...** next to the **Include Paths** field
		* Click on the **New** button
			* Click on **...** next to the newly created entry
				* Navigate to the [ProducerConsumerUsingShieldedPlug\src\main\c](ProducerConsumerUsingShieldedPlug\src\main\c) folder

#### Adding the C file to the BSP IDE project structure (BSP specific)
* Select the root node of your project
	* Right-Click and select **Add Group** this will add a group called "New Group"
	* Select this group and hit **F2** key so as to rename it to "Production"
	* Right-Click on the **Publication** group and select **Add Existing Files to group 'Production'...**
	* Navigate to the [ProducerConsumerUsingShieldedPlug/src/main/c](/ProducerConsumerUsingShieldedPlug/src/main/c) folder
	* Select all the .c source files in the folder
	* Click **Add**
	* Click **Close**

### Building the binary
* From the MicroVision IDE
	* Select **Project > Build Target** menu item (or press F7 keyboard shortcut)

## Deploying the binary

### Flashing the board (BSP specific)
* Connect your board
* Select **Flash > Download** menu item (or press F8 keyboard shortcut).

### Checking the behavior
* Set up a terminal on the board serial port and press the reset input. You shall get an output similar to the one below :

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
## SNI Library Specification

The **Shielded Plug Profile Specification** document contains detailed information on :

* the concepts underlying the Shielded Plug API
* how to map Java primitive types to portable C types for methods/functions parameters and return types
* how to create dedicated readers/writers that can translate Shielded Plug Blocks into Java Objects
* the reference API for both C and Java languages

To access the document from MicroEJ :

* Select **Help > MicroEJ Resource Center** menu item
	* Select **Library Specification > SP 1.0**

## Advanced use cases

The Use Case shown in this document only covers a most basic usage of the shared memory feature provided by Shielded Plug. Here, some data may be overwritten before getting a chance to be consumed. If this is an issue, the following options shall be considered :

* **Bufferization of the data being produced**: Here, only one data slot is available at any time, and it is overwritten if data has not been consumed faster than it was produced. Assuming production rate is not always greater than consumption rate, using a buffer able to accomodate production/consumption peak ratios can be considered.

* **Consumer-Producer synchronization**: If production can be paused, the consumer(s) could notify the producer(s) that consumable data has been processed. The producer(s) may then adapt its production to the (variable) consumption rate.
 
