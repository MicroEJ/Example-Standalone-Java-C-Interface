# Overview

This repository contains examples showing how to communicate between programs written in C and programs written in Java.

For tasks/threads synchronisation via function calls and callbacks, MicroEJ provides the SNI API.
The following examples illustrate some SNI use cases :
* [Making synchronous C functions calls from Java](/CallingCFromJava)
* ...

For data exchange issues between tasks/threads, MicroEJ provides a few options.
The following examples illustrate some of the options :
* Shared memory areas via the use of the Immutables API (TODO)
* [Producer Consumer problem](/ProducerConsumerUsingShieldedPlug) using Shielded Plug API as the delivery mechanism