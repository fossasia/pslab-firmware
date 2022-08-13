# PSLab Bootloader

This repository contains the bootloader of the
[Pocket Science Lab (PSLab)](https://pslab.io) open hardware platform.

![Build Status](https://github.com/fossasia/pslab-bootloader/actions/workflows/main-builder.yml/badge.svg)
[![Gitter](https://badges.gitter.im/fossasia/pslab.svg)](https://gitter.im/fossasia/pslab?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge)
[![Mailing List](https://img.shields.io/badge/Mailing%20List-FOSSASIA-blue.svg)](https://groups.google.com/forum/#!forum/pslab-fossasia)
[![Twitter Follow](https://img.shields.io/twitter/follow/pslabio.svg?style=social&label=Follow&maxAge=259)

The bootloader makes it possible to flash new firmware to the device over a USB
connection, whereas without it a special programming tool is required.

## Pocket Science Lab

The PSLab provides an array of test and measurement instruments for doing
science and engineering experiments. Its built-in instruments include an
oscilloscope, a waveform generator, a frequency counter, programmable voltage
and current sources, and a logic analyzer. The PSLab also has UART, I2C, and SPI
buses, via which external devices can be connected and controlled.

The PSLab is a fully open device, and a FOSSASIA provices a complete hardware
and software stack under open source licenses:

-   [Hardware](https://github.com/fossasia/pslab-hardware)
-   [Bootloader](https://github.com/fossasia/pslab-bootloader)
-   [Firmware](https://github.com/fossasia/pslab-firmware)
-   [Python library](https://github.com/fossasia/pslab-python)
-   [Graphical desktop application](https://github.com/fossasia/pslab-desktop)
-   [Android app](https://github.com/fossasia/pslab-android)

### Buy

-   You can get a Pocket Science Lab device from the
    [FOSSASIA shop](https://fossasia.com/)

-   More resellers are listed on the [PSLab website](https://pslab.io/shop/)

### Get in touch

-   The PSLab [chat channel is on Gitter](https://gitter.im/fossasia/pslab)

-   Please also join us on the
    [PSLab Mailing List](https://groups.google.com/forum/#!forum/pslab-fossasia)

## Dependencies

The following tools are required to build the bootloader:

-   xc16 compiler
-   cmake

## Building

This project is built with CMake. After cloning this repository, you must first
initialize and update the toolchain submodule:

```bash
git submodule init
git submodule update
```

This will populate the `external/cmake-microchip` directory, after which the
bootloader can be built:

```bash
mkdir build
cd build
cmake ..
make
```

This will create a build artifact in the `build` directory:
`pslab-bootloader.hex`.

### Creating a combined HEX file

It is possible to create a HEX file containing both the bootloader and the
firmware. To do this, follow these steps:

1.  Build the bootloader

2.  Build the firmware

3.  Move firmware.hex into the `build/` directory of the bootloader repository

4.  Run `combine_hex.sh`. The `hexmate` program, distributed as part of
    MPLAB-X, needs to be in your $PATH. Alternatively, modify `combine_hex.sh`
    to include the full pathname to `hexmate`.

## Flashing

Flashing the bootloader requires a programmer such as the PICkit3.

Flashing with a programmer requires the mdb.sh script, which is distributed as
part of Microchip's MPLAB-X software suite. On Linux, the default installation
path for mdb.sh is `/opt/microchip/mplabx/<version>/mplab_platform/bin/mdb.sh`.
This script is used to run the file flash.mdbscript, located in the repository
root. Before following the below steps, you may need to modify flash.mdbscript
depending on which programmer you are using and the location of the firmware
HEX.

1.  Disconnect the device from any power source
2.  Connect the programmer to the device's ICSP header
3.  Power on the device via USB
4.  Run `mdb.sh flash.mdbscript`
5.  Disconnect the programmer

## Repository structure

```shell
📦pslab-bootloader
 ┣ 📂src                        # PSLab bootloader source code
 ┃ ┣ 📂mcc_generated_files      # Source C files
 ┃ ┣ 📜 ...
 ┃ ┣ 📜main.c                   # Entry point to PSLab Bootloader
 ┣ 📂external
 ┃ ┣ 📂cmake-microchip          # Toolchain submodule
 ┣ 📜CMakeLists.txt
 ┣ 📜flash.mdbscript
 ┣ 📜LICENSE
 ┗ 📜README.md
```
