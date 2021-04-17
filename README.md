# PSLab Firmware

Repository for the firmware of the [Pocket Science Lab (PSLab)](https://pslab.io) open hardware platform.

![Build Status](https://github.com/fossasia/pslab-firmware/actions/workflows/main-builder.yml/badge.svg)
[![Gitter](https://badges.gitter.im/fossasia/pslab.svg)](https://gitter.im/fossasia/pslab?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge)
[![Mailing List](https://img.shields.io/badge/Mailing%20List-FOSSASIA-blue.svg)](https://groups.google.com/forum/#!forum/pslab-fossasia)
[![Twitter Follow](https://img.shields.io/twitter/follow/pslabio.svg?style=social&label=Follow&maxAge=2592000?style=flat-square)](https://twitter.com/pslabio)

PSLab is a tiny pocket science lab that provides an array of test and measurement instruments for doing science and engineering experiments. It comes with function or instruments like an oscilloscope, waveform generator, frequency counter, programmable voltage and current source and also as a data logger. Also available are the [hardware schematics](https://github.com/fossasia/pslab-hardware/), the [desktop application](https://github.com/fossasia/pslab-desktop) and an [Android app](https://github.com/fossasia/pslab-android). The website is at https://pslab.io.

## Buy

* You can get a Pocket Science Lab device from the [FOSSASIA Shop](https://fossasia.com).
* More resellers are listed on the [PSLab website](https://pslab.io/shop/).

## Communication

* The PSLab [chat channel is on Gitter](https://gitter.im/fossasia/pslab).
* Please also join us on the [PSLab Mailing List](https://groups.google.com/forum/#!forum/pslab-fossasia).

## Requirements 

### Development Environment
The source files in this repository were generated and developed using MPLAB X IDE V5.45. The latest version of this IDE can be downloaded from [MPLAB-X-IDE](https://www.microchip.com/en-us/development-tools-tools-and-software/mplab-x-ide#tabs) page.

### Compiler
The 16-bit microcontroller architecture uses XC-16 compiler and the source in this repository is compiled using XC16 compiler V1.70 and this can be downloaded from [XC16 Compiler](https://www.microchip.com/en-us/development-tools-tools-and-software/mplab-xc-compilers#Downloads) page.

### Code Configurator
Bootloader configurations based on EZBL library and SD card functionalities based on FatFS library are configured using Microchip Code Configurator (MCC). The version used in this repository is V4.0.2 and it can be downloaded from [MCC](https://www.microchip.com/en-us/development-tools-tools-and-software/embedded-software-center/mplab-code-configurator#Downloads) page.

### Programmer
The compiled source will generate a `.hex` file that can be uploaded into the microcontroller using a compatible programmer. PiCKit3 In-Circuit Debugger is used during development and it is available to purchase in many online stores

### Unified Bootloader
Compiled `.hex` files can be uploaded into the microcontroller using a direct USB connection using a special GUI application; Unified Bootloader. This standalone application can be downloaded from [Unified Bootloader](https://www.microchip.com/SWLibraryWeb/product.aspx?product=16-bit-Bootloader) page.

It might not be straightforward to run this application. Java and JavaFX libraries are required as pre-requisites and they can be downloaded from [Gluonhq](https://gluonhq.com/products/javafx/) page and installation instructions are available at [OpenJFX Docs](https://openjfx.io/openjfx-docs/#install-javafx) page. Extract the `lib` folder into `/usr/local/openjfx/` directory to use the supplimentary [shell file](bootloader-host-application/run-host.sh) to execute the GUI application.

More resources on ezbl integration can be found from [this video](https://www.youtube.com/watch?v=2LhW11LbNhY).

## Branch Policy

* The **bootloader** branch contains the new version of firmware that enables on-site firmware updates over a USB connection.
* The **development** branch holds is the standard branch of the project. PRs are merged to this branch and tested on an ongoing basis.
* The **master** branch holds the stable version of the project and merges the development branch regularly after it is tested thouroughly.

## Available Software

Either use the hardware with the [PSLab desktop app](https://github.com/fossasia/pslab-desktop) or the [PSLab Android app](https://github.com/fossasia/pslab-android).

## License

The library is free and open source software licensed under the [Apache License 2.0](LICENSE). The copyright is owned by FOSSASIA.
