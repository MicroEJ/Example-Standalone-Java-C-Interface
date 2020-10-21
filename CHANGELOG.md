# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## 1.2.1 - 2020-10-21

### Changed

  - Increase stack size of producer.

### Fixed

  - Fix all readme warnings.
  - Initialize variable before use.
  - Fix panic when a FreeRTOS task is created with a NULL name.
  - Replace empty snprintf with strcpy.

## 1.2.0 - 2019-11-29

### Changed

  - Use MMM and MicroEJ 5.


## 1.1.3 - 2017-08-16

### Fixed

  - Fix readme.md in CallingCFromJava

## 1.1.2 - 2017-02-06

### Fixed

  - fixed EasyAnt builds
  - minor rewording for a better separation between the CallingCFromJava example and the tasks/threads synchronisation examples

## 1.1.1 - 2017-02-03

### Fixed

  - ProducerConsumerUsingQueues revision 1.1.1

## 1.1.0 - 2017-01-16

### Added

  - Rely on the jpf downloadable from http://developer.microej.com instead of providing a custom one.
  - Use Ivy for dependency management

## 1.0.1 - 2016-08-02

### Fixed

  - ProducerConsumerUsingShieldedPlug revision 1.0.1

## 1.0.0 - 2016-05-19

### Added

  - Initial public revision.
  
---
_Markdown_
_Copyright 2016-2020 MicroEJ Corp. All rights reserved._
_Use of this source code is governed by a BSD-style license that can be found with this software._
