# Copyright 2012-2015 IS2T. All rights reserved.
# For demonstration purpose only
# IS2T PROPRIETARY. Use is subject to license terms.

I) Introduction
===============

	This document describes how to build the STM32F429IDISCO-[example]-CM4_ARMCC Java Platform 
	(JPF) example and how to launch a Java application on this JPF. 
	
II) Content Description
=======================

	This JPF example is split into multiple MicroEJ projects:
	
	- STM32F429IDISCO-[example]-CM4_ARMCC-configuration:
		Contains the JPF configuration description. Some modules are described in 
		a specific sub-folder /<module> with some optional configuration files 
		(<module>.properties and / or <module>.xml).
	
	- STM32F429IDISCO-[example]-CM4_ARMCC-bsp:
		Contains a ready-to-use BSP software project for the STM32F429I-DISCO 
		board, including:
		- Keil MDK-ARM 4.5 project (compatible Keil MDK-ARM 4.6),
		- implementation of MicroJvm port on FreeRTOS OS,
		- STM32F429I-DISCO board support package.
		
	- STM32F429IDISCO-[example]-CM4_ARMCC-mock (optional)
		A Java project that contains the code for a mock implementation. Used by
		the SimJPF to simulate the Java application native (C) method(s).
		
	- STM32F429IDISCO-[example]-CM4_ARMCC-[version]
		This project is created once the JPF is built. 
		It contains a JPF for STM32F429I-DISCO board, ready to be 
		linked into the Keil MDK-ARM project.
		
	- STM32F429IDISCO-[example]-CM4_ARMCC-fp (optional)
		This front panel project is updated once the JPF is	built. It contains 
		the board description and images for the simulator.
		
	- STM32F429IDISCO-[example]-CM4_ARMCC-microuiGeneratorExtension (optional)
		Contains some files to extend MicroUI Image Generator tool. 
		
	- STM32F429IDISCO-[example]-CM4_ARMCC-javaExample
		Contains Java code example that will be run on the newly created JPF.
		
III) Launching Example
======================

	1) JPF build
	
		First step is to build a JPF for the STM32F429I-DISCO board from JPF 
		architecture CM4. 
		
		Build may be less or more complex, depending on the set	of libraries 
		that will be provided by the JPF. Typical build steps are:
			- JPF instantiation,
			- modules selection (native stacks, Java libraries etc.),
			- Keil MDK-ARM project generation. 
		
		Procedure:
			- Open the file [example]-configuration/example.platform. 
			- Launch the platform build clicking on the link "Build Platform" in
			the overview -> Build section.
			- Once the platform is built, the STM32F429IDISCO-[example]-CM4_ARMCC-[version] project is created 
			  in the workspace.
		
	2) Java Application build 
	
		Second step is to write and generate a Java application on top of the 
		newly created JPF.
		This platform has built-in Java examples.
					
		Procedure:
			- Click on File > New > Java Example
			- Select STM32F429IDISCO-[example]-CM4_ARMCC JPF
			- Select one of the available example
			- Click on Next > Finish
			- In the Package Explorer, open the folder Java example project > launches.
			- Right-click on [example] EmbJPF.launch file 
			- Select Run As > [example] EmbJPF.
			- Once MicroEJ launch is executed, the generated object file is available 
			  in [workspace]/.microej/[CPU architecture and compiler]/application/javaapp.o.
		
	3) Final Application build
	
		Last step is to open the STM32F429I-DISCO board support project, compile 
		it and link it with the Java application, and program the final application 
		binary on the board.
						
		Procedure:
			- In Package Explorer, right-click on [example]-bsp/Project/MicroEJ/
			  MDK-ARM/Project.uvproj > Open With > System Editor.
			- In Keil MDK-ARM Workbench, press F7. The C application is compiled 
			  and linked with MicroJvm libraries and Java application.
			- Press Debug > Start/Stop Debug Session. (By default, project is 
			  configured for programming with STLINK/V2 probe).	  
			- Once application is loaded, press F5 for resuming. Java System.out 
			  print stream is redirected to C printf, which is connected to P1 
			  connector at the pin PD5 (baudrate=115200,stopbits=1,parity=none,databits=8)
		
