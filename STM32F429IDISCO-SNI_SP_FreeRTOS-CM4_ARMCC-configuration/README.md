[](Markdown)
[](Copyright 2015 IS2T. All rights reserved.)
[](IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.)

# Overview

This document describes how to create the **STM32F429IDISCO-SNI_SP_FreeRTOS-CM4_ARMCC** MicroEJ platform and how to integrate the code from the [ProducerConsumerUsingShieldedPlug](/ProducerConsumerUsingShieldedPlug) project into it.

The following steps will be taken :

* Building a Java Platform
	* (Optional) Recreating the platform
	* Building the platform
	* Building a skeleton Java executable
	* Running Java on the board
* Producer Consumer Integration
	* Spawning the C tasks from the main C program
	* Building the updated C project
	* Deploying the binary

## Requirements

* JRE 7 (or later) x86.
* MicroEJ 3.1 or later.
* BSP specific toolchain with FreeRTOS support (Keil MicroVision (&trade;) v5 or later with v4 legacy pack)

## Adapting this configuration to your BSP toolchain

This document shows the process for a STM32F429I-Disco (&trade;) target using a Keil MicroVision (&trade;) toolchain and FreeRTOS.

Please refer to the relevant "Build a Java Platform" guide in the "Getting started" section of MicroEJ Resource Center for indications on how to adapt this process to your specific target.


# Building a Java Platform

We need to generate and build a Java Platform (JPF), i.e. the set of object files and libraries required to execute Java code on the target platform linked by an executable C program.

## (Optional) Recreating the platform

The platform provided is **based** on the [STM32F429I-DISCO Platform \(Keil,FreeRTOS\)](http://developer.microej.com/ivy/artifacts/com/is2t/CM4/examples/st/STM32F429IDISCO-HMI-CM4_ARMCC-FreeRTOS-eval/1.0.0/STM32F429IDISCO-HMI-CM4_ARMCC-FreeRTOS-eval_src-1.0.0.zip) platform available from [developer.microej.com](http://developer.microej.com)

However, as such, this platform does not provided Shielded Plug support. Besides, the UI support is not needed for this example. Therefore, 
the following modifications have been performed on the platform:

### Renaming of the platform

So as to avoid confusion, the platform has been renamed to **STM32F429IDISCO-SNI_SP_FreeRTOS-CM4_ARMCC**

### Selection of the modules

* From the **Content** tab of the [example.platform](STM32F429IDISCO-SNI_SP_FreeRTOS-CM4_ARMCC-configuration/example.platform) file
	* In the **modules** frame
		* Click on the **Show/Hide modules** button
		* In the **modules** list, check **Java to C Interface**
		
### Stripping the UI support
* BSP UI support has been removed
* microuiGeneratorExtension project has been removed

### Implementation of the Shielded Plug API in C

* Add [LLSP_FreeRTOS.c](/STM32F429IDISCO-SNI_SP_FreeRTOS-CM4_ARMCC-bsp/Project/MicroEJ/src/LLSP_FreeRTOS.c) source file to the (/STM32F429IDISCO-SNI_SP_FreeRTOS-CM4_ARMCC-bsp/Project/MicroEJ/src)[STM32F429IDISCO-SNI_SP_FreeRTOS-CM4_ARMCC-bsp/Project/MicroEJ/src] directory
* From the **Project Explorer** view
	* Double-click on the [Project.uvproj](/STM32F429IDISCO-SNI_SP_FreeRTOS-CM4_ARMCC-bsp/Project/MicroEJ/MDK-ARM/Project.uvproj) file (this will open the BSP project in the MicroVision IDE)
* From the MicroVision IDE
* Right-click on the **STM32F429i-DISCO>MicroEJ** node of your project and select **Add Existing Files to group 'MicroEJ'**
	* Select the [LLSP_FreeRTOS.c](/STM32F429IDISCO-SNI_SP_FreeRTOS-CM4_ARMCC-bsp/Project/MicroEJ/src/LLSP_FreeRTOS.c) source file 
	* Click **Add**
	* Click **Close**

## Building the platform
* Open the [example.platform](STM32F429IDISCO-SNI_SP_FreeRTOS-CM4_ARMCC-configuration/example.platform) file in the [(STM32F429IDISCO-SNI_SP_FreeRTOS-CM4_ARMCC-configuration)](STM32F429IDISCO-SNI_SP_FreeRTOS-CM4_ARMCC-configuration) project
* From the **Overview** tab of the generated example.platform file
	* Click on the **Build platform** hyperlink. This will generate the following project :
		* STM32F429IDISCO-SNI_SP_FreeRTOS-CM4_ARMCC-1.0.0
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
	* Set the **Package** field to "com.is2t.examples.java2c"
	* Set the **Name** field to "DataConsumerExample"
	* Click on **Finish**
	* Copy and paste the following code inside the generated [DataConsumerExample.java](ProducerConsumerUsingShieldedPlug/src/main/java/com/is2t/examples/java2c/DataConsumerExample.java) file 

			package com.is2t.examples.java2c;
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

## Running Java on the board

### Opening the generated C project
* From the **Project Explorer** view
	* Navigate to the [STM32F429IDISCO-SNI_SP_FreeRTOS-CM4_ARMCC-bsp/Project/MicroEJ/MDK-ARM](STM32F429IDISCO-SNI_SP_FreeRTOS-CM4_ARMCC-bsp/Project/MicroEJ/MDK-ARM) folder
	* Double-click on the [Project.uvproj](/STM32F429IDISCO-SNI_SP_FreeRTOS-CM4_ARMCC-bsp/Project/MicroEJ/MDK-ARM/Project.uvproj) file (this will open the BSP project in the MicroVision IDE)

### Building the binary
* From the MicroVision IDE
	* Select **Project > Build Target** menu item (or press F7 keyboard shortcut)

### Flashing the board
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
	

# Producer Consumer Integration 

## Spawning the C tasks from the main C program

We need to modify the main C program so as to call our producers instantiation code.

### Opening the generated C project (BSP specific)
* From the **Project Explorer** view
	* Open the [main.c](/STM32F429IDISCO-SNI_SP_FreeRTOS-CM4_ARMCC-bsp/Project/MicroEJ/src/main.c) source file
	* Add `#include "sp-producer-accelerometer.h"` to the include directives
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

### Flashing the board
* Connect your board
* Select **Flash > Download** menu item (or press F8 keyboard shortcut).
