[](Markdown)
[](Copyright 2016 IS2T. All rights reserved.)
[](IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.)

# Overview

This document describes how to create the **STM32F429IDISCO-SNI_SP_FreeRTOS-CM4_ARMCC** MicroEJ platform.

The following steps will be taken :

* Building a Java Platform
	* (Optional) Recreating the platform
	* Building the platform
	* Building a skeleton Java executable
		* For Shielded Plug
		* For SNI
	* Running Java on the board

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