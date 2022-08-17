# PSLab Firmware

This repository contains firmware for the
[Pocket Science Lab (PSLab)](https://pslab.io) open hardware platform.

![Build Status](https://github.com/fossasia/pslab-firmware/actions/workflows/main-builder.yml/badge.svg)
[![Gitter](https://badges.gitter.im/fossasia/pslab.svg)](https://gitter.im/fossasia/pslab?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge)
[![Mailing List](https://img.shields.io/badge/Mailing%20List-FOSSASIA-blue.svg)](https://groups.google.com/forum/#!forum/pslab-fossasia)
![Twitter Follow](https://img.shields.io/twitter/follow/pslabio.svg?style=social&label=Follow&maxAge=259)

## Pocket Science Lab

The PSLab provides an array of test and measurement instruments for doing
science and engineering experiments. Its built-in instruments include an
oscilloscope, a waveform generator, a frequency counter, programmable voltage
and current sources, and a logic analyzer. The PSLab also has UART, I2C, and SPI
buses, via which external devices can be connected and controlled.

The PSLab is a fully open device, and FOSSASIA provides a complete hardware
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

The following tools are required to build the firmware:

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
firmware can be built:

```bash
mkdir build
cd build
cmake ..
make
```

This will create a build artifact in the `build` directory:
`pslab-firmware.hex`.

## Flashing

The firmware can be flashed over USB or by using a programmer such as the
PICkit3.

### Over USB

Firmware can be flashed over USB if the device already has the
[bootloader](https://github.com/fossasia/pslab-bootloader) installed.

Flashing the firmware requires the pslab-python library. See
[pslab-python](https://github.com/fossasia/pslab-python) for installation
instructions.

Follow these steps to flash new firmware:

1.  Ground the BOOT pin
2.  Reset or power cycle the device
3.  Run `pslab flash --port <portname> firmware.hex`
4.  Unground the BOOT pin
5.  Reset or power cycle the device

### Using a programmer

> **Warning**  
> If your device contains a bootloader, flashing just the firmware HEX with a
> programmer will OVERWRITE the bootloader. If for some reason you are unable
> to flash over USB, it is a better idea to first create a combined HEX file
> containing both the bootloader and the firmware and flash that, rather than
> flashing the pure firmare HEX. See the
> [bootloader repository](https://github.com/fossasia/pslab-bootloader/#creating-a-combined-hex-file)
> for instructions on how to create a combined HEX.

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
📦pslab-firmware
 ┣ 📂src                        # PSLab firmware source code
 ┃ ┣ 📂bus                      # Communication specific source files
 ┃ ┃ ┣ 📜 ...
 ┃ ┃ ┗ 📜i2c.c
 ┃ ┣ 📂helpers                  # Supplementary functions
 ┃ ┃ ┣ 📜 ...
 ┃ ┃ ┗ 📜version.c
 ┃ ┣ 📂instruments              # Instrument specific source files
 ┃ ┃ ┣ 📜 ...
 ┃ ┃ ┗ 📜multimeter.c
 ┃ ┣ 📂registers                # PIC specific register entry files
 ┃ ┃ ┣ 📂comparators
 ┃ ┃ ┃ ┣ 📜 ...
 ┃ ┃ ┃ ┗ 📜ic1.c
 ┃ ┃ ┣ 📂 ...                   # includes converters, memory, system
 ┃ ┃ ┣ 📂timers
 ┃ ┃ ┃ ┣ 📜 ...
 ┃ ┃ ┃ ┗ 📜tmr1.c
 ┃ ┣ 📂sdcard                   # SD Card specific file handling source files
 ┃ ┣ 📜 ...
 ┃ ┣ 📜main.c                   # Entry point to PSLab Core
 ┃ ┣ 📜commands.c               # Entry point to function implementations
 ┣ 📂external
 ┃ ┣ 📂cmake-microchip          # Toolchain submodule
 ┣ 📜CMakeLists.txt
 ┣ 📜flash.mdbscript
 ┣ 📜LICENSE
 ┗ 📜README.md
```
