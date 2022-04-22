# PSLab Firmware - Application Layer

This directory contains the application layer of the PSLab firmware. This application runs during normal operation, i.e. unless the device stays in bootloader mode for some reason.

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

This will create a build artifact in the `build` directory: `pslab-firmware.hex`. After putting the PSLab in bootloader mode (by grounding the BOOT pin and power cycling the device), this new firmware can be flashed to the device with the Unified Bootloader application.
