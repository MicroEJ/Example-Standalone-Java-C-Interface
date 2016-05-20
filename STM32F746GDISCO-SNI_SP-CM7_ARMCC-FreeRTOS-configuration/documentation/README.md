<!--
	Copyright 2015-2016 IS2T. All rights reserved.
	For demonstration purpose only
	IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
-->

# Build MicroEJ Platform Reference Implementation for STM32F746G-DISCO

## Introduction

This document describes how to build the MicroEJ Platform Reference Implementation for STM32F746G-DISCO and how to launch a MicroEJ application on this platform.
	
## Prerequisites

 * Java7 or higher
 * MicroEJ SDK 4.0 or later
 * Keil uVision 5.18
 
## Projects Description

This platform source files are split into multiple projects:
	
`STM32F746GDISCO-[name]-CM7hardfp_ARMCC5-configuration` :
> Contains the platform reference implementation configuration description. Some modules are described in a specific sub-folder /<module> with some optional configuration files (<module>.properties and / or <module>.xml).
	
`STM32F746GDISCO-[name]-CM7hardfp_ARMCC5-bsp` :
> Contains a ready-to-use BSP software project for the STM32F746G-DISCO board, including a Keil uVision project, an implementation of MicroEJ core engine (and extensions) port on FreeRTOS OS and the STM32F746G-DISCO board support package.
		
`STM32F746GDISCO-[name]-CM7hardfp_ARMCC5-[version]` :
> This project is created once the platform is built. It contains the platform for STM32F746G-DISCO board, ready to be linked into the Keil uVision project.
		
`STM32F746GDISCO-[name]-CM7hardfp_ARMCC5-fp` :
> This front panel project is updated once the platform is built. It contains the board description and images for the MicroEJ simulator.
				
## Platform Reference Implementation Build

First step is to build a platform reference implementation for the STM32F746G-DISCO board from platform architecture STMicroelectronics STM32F7J. 
		
Build may be less or more complex, depending on the set	of foundation libraries that will be provided by the platform. Typical build steps are:
 * platform instantiation,
 * modules selection (native stacks, MicroEJ foundation libraries etc.),
 * Keil uVision project generation. 
		
Procedure:
 * Open the file `STM32F746GDISCO-[name]-CM7hardfp_ARMCC5-configuration/full.platform`. 
 * Launch the platform build clicking on the link `Build Platform` in the `Overview > Build` section.
 * Once the platform is built, the `STM32F746GDISCO-[name]-CM7hardfp_ARMCC5-[version]` project is created in the workspace.
		
## MicroEJ Application Build 
	
Second step is to write and generate a MicroEJ application on top of the newly created platform. This platform has built-in MicroEJ examples. Some additional MicroEJ examples are available on <http://developer.microej.com>
		
Procedure:
 * Click on `File > New > MicroEJ Standalone Example Project`
 * Select `STM32F746GDISCO-[name]-CM7hardfp_ARMCC5` platform
 * Select one of the available example
 * Click on `Next`
 * Click on `Finish`
 * In the Package Explorer, right-click on `[example]` project
 * Select `Run As > MicroEJ Application` to run the application on the MicroEJ simulator
 * Click on menu `Run > Run Configurations...`
 * The launcher is automatically selected on left menu, if not, select it
 * Open `Execution` tab
 * Select `Execute on Device`
 * Click on `Run`
 * Once MicroEJ launch is executed, the generated object file is available in `[workspace]/.microej/CM7hardfp_ARMCC5/application/microejapp.o`.
		
## Final Application build
	
Last step is to open the STM32F746G-DISCO board support project for Keil uVision, compile it and link it with the MicroEJ application, and program the final application binary on the board.

Procedure:
 * In Package Explorer, right-click on `STM32F746GDISCO-[name]-CM7hardfp_ARMCC5-bsp/Projects/STM32746G-Discovery/Applications/MicroEJ/MDK-ARM/Project.uvprojx > Open With > System Editor`.
 * In Keil uVision, press `F7`. The C application is compiled and linked with platform libraries and MicroEJ application.
 * Press `Flash > Download` (By default, project is configured for programming with STLINK/V2 probe).	  
 * Once application is loaded, it starts. MicroEJ application `System.out` print stream is redirected to C `printf`, which is connected to CN14 USB connector (baudrate=115200,stopbits=1,parity=none,databits=8) 
			  
			  