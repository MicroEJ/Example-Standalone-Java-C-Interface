[](Markdown)
[](Copyright 2016 IS2T. All rights reserved.)
[](IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.)
# Overview

Although MicroEJ lets you leverage Java in an embedded context, there are still some cases where your Java threads need to synchronize and exchange data with OS tasks written in C/C++.

This repository contains examples showing how to enable communication between programs written in C and programs written in Java using MicroEJ APIs.

## Available examples

For tasks/threads synchronisation via function calls and callbacks, MicroEJ provides the SNI API.
For data exchange issues between tasks/threads, MicroEJ provides a few options:
* Shielded Plug API (suitable for sharing structured data, with built-in wait/release synchronisation APIs)
* Immortals API and SNI API (suitable for sharing access to raw data, but you need to provide your own synchronisation mechanism)

The following examples, most of them based on the consumer/producer problem, illustrate how to use those APIs :
* [Making synchronous C functions calls from Java](/CallingCFromJava) using SNI
* [Producer Consumer with ShieldedPlug](/ProducerConsumerUsingShieldedPlug) using Shielded Plug API as the delivery mechanism
* [Producer Consumer with SNI and Immortals](/ProducerConsumerUsingQueues) using SNI and Immortals APIs to wrap the native FreeRTOS message queue API as the delivery mechanism

Each example comes with a launch configuration. These launch configurations require that the target platform (common to each configuration) is built.

# Building the Java Platform (JPF)

We need to generate and build a Java Platform (JPF), i.e. the set of object files and libraries required to execute Java code on the target platform linked by an executable C program.

The following assumes that you have downloaded and imported the STM32F646-DISCO platform from [MicroEJ SDK Getting Started](http://developer.microej.com/getting-startetd-sdk.html) web page.

### Creation (BSP Specific)

* Select **File > New > Other... > MicroEJ > Platform** menu item
	* Click **Browse** button next to the **Architecture** field and select **STMicroElectronics STM32F7J ARMCC**
	* Check the **Create from a platform reference implementation** option
		* Select **STM32F746-Disco > Standalone**
	* Click **Next**
		* Set the name field to "**SNI**"
		* Set the version field to "**1.0.0**"
	* Click **Finish**. This will lead to the generation of a few projects in our workspace :
		* [STM32F746GDISCO-SNI-CM7hardfp_ARMCC5-bsp](STM32F746GDISCO-SNI-CM7hardfp_ARMCC5-bsp)
		* [STM32F746GDISCO-SNI-CM7hardfp_ARMCC5-configuration](STM32F746GDISCO-SNI-CM7hardfp_ARMCC5-configuration)
		* [STM32F746GDISCO-SNI-CM7hardfp_ARMCC5-fp](STM32F746GDISCO-SNI-CM7hardfp_ARMCC5-fp)

### Build
* From the **Overview** tab of the generated [full.platform](/STM32F746GDISCO-SNI-CM7hardfp_ARMCC5-configuration/full.platform) file
	* Click on the **Build platform** hyperlink. This will generate the following project :
		* [STM32F746GDISCO-SNI-CM7hardfp_ARMCC5-1.0.0](STM32F746GDISCO-SNI-CM7hardfp_ARMCC5-1.0.0)


## API Selection Criteria

Here are a few things to consider when choosing between Shielded Plug and an SNI and Immortals based solution

* Performance
	* Generally speaking, Shielded Plug (SP for short) will copy data from/to a database block and will therefore take more processing time and memory than an SNI-based solution relying on a shared memory area with less data copying.
	* SP is more suitable for asynchronous (post) processing of published data

* Data Integrity
	* Given that with SP there is no buffering of data, it makes it more suitable for sampling scenarios where losing a value once in a while is not critical .

* API convenience
	* SP requires that you describe your data in an xml based configuration file.
	* However, the SP synchronization API is quite simple
	* Using SNI and Immortals, it is easier to pass data directly to/from C native functions

In summary, for simple use cases, Shielded Plug shall suffice, but for more intensive and more constrained environments, SNI and Immortals may be a better fit.

## Additional references

### B-ON Library Specification

Among other things, the **Beyond Profile Specification** document contains detailed information on:

* sharing memory areas between Java and C/C++ using **immortal objects**

**immortal objects** : such objects are never garbage collected and keep the same physical memory location forever. This makes them useful for specifying shared memory areas between C and Java.

The document can be downloaded from [www.e-s-r.net/specifications/index.html](www.e-s-r.net/specifications/index.html)

### SNI Library Specification

The **Simple Native Interface for GreenThread Context Profile Specification** document contains detailed information on :

* how to map Java primitive types to portable C types for methods/functions parameters and return types
* naming conventions for C functions implementing Java SNI native implementations
	* these have to be followed to generate valid linker configuration files
* sharing memory areas between Java and C/C++ using **immortal objects**
* controlling (as in suspend and resume) Java Thread execution from C code

The document can be downloaded from [www.e-s-r.net/specifications/index.html](www.e-s-r.net/specifications/index.html)
	
### Shielded Plug Library Specification

The **Shielded Plug Profile Specification** document contains detailed information on :

* the concepts underlying the Shielded Plug API
* how to map Java primitive types to portable C types for methods/functions parameters and return types
* how to create dedicated readers/writers that can translate Shielded Plug Blocks into Java Objects
* the reference API for both C and Java languages

The document can be downloaded from [www.e-s-r.net/specifications/index.html](www.e-s-r.net/specifications/index.html)
	
### Simulation

The **Simulation** section of the **Device Developer's Guide** document contains useful info for :

* creating Java mocks simulating native code using the same naming conventions as SNI
* using the Hardware In the Loop (HIL) engine as a replacement for the SNI/GreenThread interface used on target environments

To access the document from MicroEJ:

* Select **Help > MicroEJ Resource Center** menu item
	* Select **Manual > Device Developer's Guide**
	
# Requirements

* MicroEJ SDK 4.0.0 or later
* Keil MicroVision v5.0 or later