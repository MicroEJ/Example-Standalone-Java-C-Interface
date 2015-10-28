# Overview

This repository contains examples showing how to enable communication between programs written in C and programs written in Java using MicroEJ APIs

## Importing projects from Git

Those examples are MicroEJ projects that can be directly imported from github.

From a MicroEJ workspace :
* File > Import > Git > Projects from Git
* Select "Clone URI"
* Click on "Next"
* Set the "URI" field to http://github.com/MicroEJ/Example-Java-SNI-ShieldedPlug.git
* Click on "Next"
* Select the "master" branch
* Click on "Next"
* Set up the destination directory
* Click on "Next"
* Select "Import existing projects"
* Click on "Next"
* Click on "Finish"

## Available examples

Although MicroEJ lets you leverage Java in an embedded context, there are still some cases where your Java threads need to synchronize and exchange data with OS tasks written in C/C++.

For tasks/threads synchronisation via function calls and callbacks, MicroEJ provides the SNI API.
For data exchange issues between tasks/threads, MicroEJ provides a few options.
* Shielded Plug API (suitable for read/write accesses to structured data, with built-in wait/release synchronisation APIs)
* Immortals API and SNI API (suitable for sharing access to raw data, but you need to provide your own synchronisation mechanism)

The following examples illustrate how to use those APIs :
* [Making synchronous C functions calls from Java](/CallingCFromJava) using SNI
* [Producer Consumer problem with ShieldedPlug](/ProducerConsumerUsingShieldedPlug) using Shielded Plug API as the delivery mechanism
* [Producer Consumer problem with SNI](/ProducerConsumerUsingSNIAndImmortals) using SNI and Immortals APIs

## API Selection Criterion

* Need to share big memory area ?
* Resource access contention ?
* ...

<!--

|APIs|Example
|--|--|
|SNI|[Making synchronous C functions calls from Java](/CallingCFromJava)|
|Shielded Plug|[Producer Consumer problem](/ProducerConsumerUsingShieldedPlug) using Shielded Plug API as the delivery|
|SNI,Shielded Plug|(TODO)|

-->

## Additional references

Additional examples can be found at the following locations :
* Shared memory areas via the use of the Immutables API (TODO)

<!--

|APIs|Example
|--|--|
|Immutables|[Shared memory areas via the use of the Immutables API(TODO)|

-->

### SNI Library Specification

The **Simple Native Interface for GreenThread Context Profile Specification** document contains detailed information on :

* how to map Java primitive types to portable C types for methods/functions parameters and return types
* naming conventions for C functions implementing Java SNI native implementations
	* these have to be followed to generate valid linker configuration files
* sharing memory areas between Java and C/C++
* controlling (as in suspend and resume) Java Thread execution from C code

To access the document from MicroEJ :

* Select **Help > MicroEJ Resource Center** menu item
	* Select **Library Specification > SNI_GT 1.2**

### B-ON Library Specification

Among other things, the **Beyond Profile Specification** document contains detailed information on :

* sharing memory areas between Java and C/C++ using **immortal objects**

**immortal objects** such objects are never garbage collected and are useful for specifying shared memory areas between 

To access the document from MicroEJ :

* Select **Help > MicroEJ Resource Center** menu item
	* Select **Library Specification > BON 1.2**
	
	
### Shielded Plug Library Specification

The **Shielded Plug Profile Specification** document contains detailed information on :

* the concepts underlying the Shielded Plug API
* how to map Java primitive types to portable C types for methods/functions parameters and return types
* how to create dedicated readers/writers that can translate Shielded Plug Blocks into Java Objects
* the reference API for both C and Java languages

To access the document from MicroEJ :

* Select **Help > MicroEJ Resource Center** menu item
	* Select **Library Specification > SP 1.0**