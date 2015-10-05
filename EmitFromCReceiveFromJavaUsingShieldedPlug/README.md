<!--
	Markdown
	
	Copyright 2015 IS2T. All rights reserved.
	IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
-->

# Overview

This example shows the minimum steps required to create a Java program that communicates with a C program via a shared in-memory database using the Shielded Plug API, starting from an empty MicroEJ workspace.

The Java program will spawn a Java thread consuming data produced by some tasks written in C :

* The C producers will overwrite data at a fixed period without checking if data has been consumed by any other task.
* The data will be augmented with some ID indicating which task produced the data
* The Java consumer will poll for new data at a fixed period. Some incremental id will be added to the data to distinguish new data from old data.

The following steps will be taken :

* Building a Java Platform
* Building a skeleton Java executable
* Writing the C data producer code
* Converting data to a Shielded-Plug compatible format
* Writing the Java consumer code

## Requirements

* JRE 7 (or later) x86.
* MicroEJ 3.1 or later.
* BSP specific toolchain with RTOS support

## Adapting this example to your BSP toolchain

This document shows the process for a STM32F429I-Disco (&trade;) target using a Keil MicroVision (&trade;) toolchain and FreeRTOS.

The BSP toolchain specific steps are marked *(BSP specific)*

Please refer to the relevant "Build a Java Platform" guide in the "Getting started" section of MicroEJ Resource Center for indications on how to adapt this process to your specific target.


# Building a Java Platform

We need to generate and build a Java Platform (JPF), i.e. the set of object files and libraries required to execute Java code on the target platform linked by an executable C program.

## Importing existing platform configuration (BSP Specific)

* Select **File > Import... > Existing Projects into Workspace** menu item
	* Click on **Select archive file** radio button
	* Click **Browse** and select the STM32F429IDISCO-SNI_SP_FreeRTOS-CM4_ARMCC.zip file
	* Check the **Copy projects into workspace**
	* Click on **Finish**

##Build
* Open the **example.platform** file in the **STM32F429IDISCO-SNI_SP_FreeRTOS-CM4_ARMCC-configuration** project
* From the **Overview** tab of the generated example.platform file
	* Click on the **Build platform** hyperlink. This will generate the following project :
		* **STM32F429IDISCO-SNI_SP_FreeRTOS-CM4_ARMCC-1.0.0**-1.0.0
* Click on **Window > Preferences > MicroEJ**
	* In the **MicroEJ Repository** frame
		* Click on the **Refresh** button 

# Building a skeleton Java executable

In this section, we will create a skeleton Java program that does nothing relevant (yet), but is required so as to generate a proper BSP toolchain project from MicroEJ.

## MicroEJ project creation

This project will host the application specific code

* Select **File > New > Java Project** menu item
	* Set **Project Name** field to **"EmitFromCReceiveFromJavaUsingShieldedPlug"**
	* In the **Runtime Environment** frame, select the following MicroEJ Libraries :
		* EDC 1.2 (selected by default)
		* SP 1.0 (will be required later)
* Click on **Next**
* Click on **Finish**


## Java Class creation
* Select **File > New > Source Folder** menu item
	* Set the **Folder name** field to /src/main/java
* Select **File > New > Class** menu item
	* Set the to "InterfacingJavaAndC/src/main/java"
	* Set the **Package** field to "com.is2t.examples.java2C"
	* Set the **Name** field to "DataConsumerExample"
	* Click on **Finish**
	* Copy and paste the following code inside the generated **DataConsumerExample.java** file 

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


## Compiling the Java code for the target

Here, we will create a MicroEJ "Run Configuration" that will compile the Java code we just created for the JPF we created previously

* Select **File > New > Folder** menu item
	* Set the **Enter or select the parent folder** field to "EmitFromCReceiveFromJavaUsingShieldedPlug" (Project root)
	* Set the **Folder Name** field to "launches"
* Select **Run Configurations...** from the **Run configurations** drop down list (or Press Ctrl+Shift+3 and type "Run Configurations...")
* Select **MicroEJ Application** group and click **New**
	* Set **Name** field to "EmitFromCReceiveFromJavaUsingShieldedPlug_Build"
	* In **Main** tab
		* Set the **Project** field to "EmitFromCReceiveFromJavaUsingShieldedPlug"
		* Click on **Select Main type...** and type DataConsumerExample
	* In **Execution** tab
		* In **Target** frame
			* Click the **Browse** button next to the JPF Field and select your platform
		* In **Execution** frame
			* Select **Execute on EmbJPF** radio button
			* Leave the **Output Folder** field set to "${project_loc:EmitFromCReceiveFromJavaUsingShieldedPlug}"
	* In **Configuration** tab
		* Select the **Target > Deploy** Node
			* Set the **Means** field to "Copy at a location known by BSP Project"
	* In **Common** tab
		* In **Save as** frame
			* Select the **Shared file** radio button
			* Click on **Browse** and select **EmitFromCReceiveFromJavaUsingShieldedPlug\\launches** folder	
	* Click on "Run"

# Running Java on the board (BSP specific)

## Opening the generated C project
* From the **Project Explorer** view
	* Navigate to the **EmitFromCReceiveFromJavaUsingShieldedPlug**-bsp/Project/MicroEJ/MDK-ARM folder
	* Double-click on the Project.uvproj file => this will open the BSP project in the MicroVision IDE

## Building the binary
* From the MicroVision IDE
	* Select **Project > Build Target** menu item (or press F7 keyboard shortcut)

## Flashing the board (BSP specific)
* Connect your board
* Select **Flash > Download** menu item (or press F8 keyboard shortcut).

## Checking the behavior
* Set up a terminal on the board serial port and press the reset input. You shall get the following output :

	START
	VM START
	-- main alive --
	-- main alive --
	-- main alive --
	-- main alive --
	...
