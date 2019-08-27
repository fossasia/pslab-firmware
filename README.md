# PSLab Firmware

[![Build Status](https://travis-ci.org/fossasia/pslab-firmware.svg?branch=master)](https://travis-ci.org/fossasia/pslab-firmware)
[![Gitter](https://badges.gitter.im/fossasia/pslab.svg)](https://gitter.im/fossasia/pslab?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge)
[![Mailing List](https://img.shields.io/badge/Mailing%20List-FOSSASIA-blue.svg)](https://groups.google.com/forum/#!forum/pslab-fossasia)
[![Twitter Follow](https://img.shields.io/twitter/follow/pslabio.svg?style=social&label=Follow&maxAge=2592000?style=flat-square)](https://twitter.com/pslabio)

Repository for PSLab Firmware.

PSLab is a tiny pocket science lab that provides an array of test and measurement instruments for doing science and engineering experiments. It comes with function or instruments like an oscilloscope, waveform generator, frequency counter, programmable voltage and current source and also as a data logger. Also available are the [hardware schematics](https://github.com/fossasia/pslab-hardware/), [Desktop applications](https://github.com/fossasia/pslab-desktop-apps) and Android [application](https://github.com/fossasia/pslab-android). The website is at https://pslab.io.

## Communication

Please join us on the following channels:
* Pocket Science Channel https://gitter.im/fossasia/pslab
* Mailing List https://groups.google.com/forum/#!forum/pslab-fossasia

## Basic requirements 

- PSLab Hardware - https://github.com/fossasia/pslab-hardware) or PIC24EP256GP204 breakout board - http://www.microchip.com/wwwproducts/en/PIC24EP256GP204
- IDE: MPLABX IDE v3.35 Supported on Linux/Windows/Mac - http://www.microchip.com/mplab/mplab-x-ide
- Compiler: MPLAB® XC16 Compiler - http://www.microchip.com/mplab/compilers
- Programming Tool: PICkit™ 3 In-Circuit Debugger/PICkit2 - http://www.microchip.com/Developmenttools/ProductDetails.aspx?PartNO=PG164130

## Firmware

Implemented features:
-Byte Headers 
+ Flash R/W . One page at a time (2048 bytes)
+ I2C Sensors :
  + Start , Stop, BulkTransfers, config
+ ADC :
  + Read buffer , clear buffer
+ Sine wave :
  + Wave tables 
+ Sine wave :
  + Wave tables 

This code is elaborately written , and special function register(SFR) bit settings are done in separate lines of code for easily tweaking. The code can be shrunk if firmware efficiency becomes a concern at any point. 

The library and code for PSLab version 1 are located in directories 'PSLab_V1'. Please note that they must be located in the same directory during compilation via MPLABX because the former relatively imports the latter.


### Compilation and testing instructions

1. Download and install the MPLab IDE & XC16 compiler from the links given above
2. From the IDE , navigate to the folder containing `PSLab_Original` & `PSLab_Original_library` , and open `PSLab_Original` .
3. Connect the Pickit3 to the 5 pin programming header on the bottom side of the PSLab hardware (attach reference image here).
4. Also ensure that the PSLab is powered on by connecting it via USB . The PICkit has a software controllable option to supply power (3.3V) to the uC while programming, but this may be avoided since many other peripherals also rely on the same power source
5. Build and Download the code ( menu->run->build and program , or the Downward facing green arrow in the toolbar)

### Preliminary tests after program download

- Connect the device via USB , and issue an `lsusb command via the terminal (On Linux)`
- The output should have an entry `Bus 003 Device 003: ID 04d8:00df Microchip Technology, Inc.` . This indicates that the USB-UART chip was detected 
- To locate its handler , check /dev for ttyACM devices. The desktop app automatically locates and connects to the handler.
- In case either step failed , issue `dmesg` command. Output should resemble :
```
[128189.796088] usb 3-2: new full-speed USB device number 3 using xhci_hcd
[128189.928459] usb 3-2: New USB device found, idVendor=04d8, idProduct=00df
[128189.928469] usb 3-2: New USB device strings: Mfr=1, Product=2, SerialNumber=3
[128189.928475] usb 3-2: Product: MCP2200 USB Serial Port Emulator
[128189.928479] usb 3-2: Manufacturer: Microchip Technology Inc.
[128189.928483] usb 3-2: SerialNumber: 0001463239
[128189.929873] cdc_acm 3-2:1.0: ttyACM0: USB ACM device
[128189.933262] hid-generic 0003:04D8:00DF.0032: hiddev0,hidraw0: USB HID v1.11 Device [Microchip Technology Inc. MCP2200 USB Serial Port Emulator] on usb-0000:00:14.0-2/input2
```
- Install and launch the [PSLab desktop app](https://github.com/fossasia/pslab-desktop-apps) by following its README

## License
This project is Free and Open Source software. The project us licensed under the [Apache License 2.0](LICENSE). Copyright is owned by FOSSASIA. More details in the license files.
