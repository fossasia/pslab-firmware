# PSLab Firmware - Bootloader

This directory contains the bootloader for the PSLab firmware. The bootloader normally just loads the main firmware application, but by grounding the BOOT pin the device can be made to stay in bootloader mode. In bootloader mode, new application level firmware can be flashed via USB.

## Dependencies

-   xc16 compiler
-   cmake

## Building

This project is built with CMake. After cloning this repository, you must first initialize and update the toolchain submodule:

```bash
git submodule init
git submodule update
```

This will populate the `external/cmake-microchip` directory, after which the firmware can be built:

```bash
mkdir build
cd build
cmake ..
make
```

This will create a build artifact in the `build` directory: `pslab-bootloader.hex`. This file can be flashed to the PSLab using a programmer such as the PICKit3.
