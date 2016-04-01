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

## API Selection Criteria

Here are a few things to consider when choosing between Shielded Plug and an SNI and Immortals based solution

* Generally speaking, Shielded Plug will copy data from/to a database block and will therefore take more memory than an SNI-based solution relying on a shared memory area with less data copying.
* Besides, Shielded Plug requires that you describe your data in an xml based configuration file.
* However, the synchronization API is somewhat simpler
* Using SNI and Immortals, it is easier to pass data directly to/from C native functions

In summary, for simple use cases, Shielded Plug shall suffice, but for more intensive and more constrained environments, SNI and Immortals may be a better fit.

## Additional references

Additional examples can be found at the following locations:
* Java/C Data Sharing Using SNI application note on www.microej.com.
* Synchronizing a Java thread with C task(s) application note on www.microej.com.

### B-ON Library Specification

Among other things, the **Beyond Profile Specification** document contains detailed information on:

* sharing memory areas between Java and C/C++ using **immortal objects**

**immortal objects** : such objects are never garbage collected and keep the same physical memory location forever. This makes them useful for specifying shared memory areas between C and Java.

To access the document from MicroEJ:

* Select **Help > MicroEJ Resource Center** menu item
	* Select **Library Specification > BON 1.2**

### SNI Library Specification

The **Simple Native Interface for GreenThread Context Profile Specification** document contains detailed information on :

* how to map Java primitive types to portable C types for methods/functions parameters and return types
* naming conventions for C functions implementing Java SNI native implementations
	* these have to be followed to generate valid linker configuration files
* sharing memory areas between Java and C/C++ using **immortal objects**
* controlling (as in suspend and resume) Java Thread execution from C code

To access the document from MicroEJ:

* Select **Help > MicroEJ Resource Center** menu item
	* Select **Library Specification > SNI_GT 1.2**
	
### Shielded Plug Library Specification

The **Shielded Plug Profile Specification** document contains detailed information on :

* the concepts underlying the Shielded Plug API
* how to map Java primitive types to portable C types for methods/functions parameters and return types
* how to create dedicated readers/writers that can translate Shielded Plug Blocks into Java Objects
* the reference API for both C and Java languages

To access the document from MicroEJ:

* Select **Help > MicroEJ Resource Center** menu item
	* Select **Library Specification > SP 1.0**
	
### Simulation

The **Simulation** section of the **platform architecture manual** document contains useful info for :

* creating Java mocks simulating native code using the same naming conventions as SNI
* using the Hardware In the Loop (HIL) engine as a replacement for the SNI/GreenThread interface used on target environments

To access the document from MicroEJ:

* Select **Help > MicroEJ Resource Center** menu item
	* Select **Manual > Platform Architecture - User Manual**