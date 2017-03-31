# pslab-firmware
Repository for PSLab Firmware

uC Platform : PIC24EP256GP204

Existing Firmware
-----------------
The original library and main code for PSLab version 1 are located in directories 'PSLab_Original' and 'PSLab_Original_library'\\
Please note that they must be located in the same directory during compilation via MPLABX because the former relatively imports the latter.

This code is elaborately written , and SFR bit settings are done in separate lines of code for easily tweaking. The code can be shrunk if firmware efficiency becomes a concern at any point. 


New Firmware
------------
Implemented feature list in new project called 'MPLab Project'
-Byte Headers 

- Flash R/W . One page at a time (2048 bytes)
- I2C Sensors :
-- Start , Stop, BulkTransfers, config

- ADC :
-- Read buffer , clear buffer

- Sine wave :
-- Wave tables 

Programming Requirements
------------------------
MPLABX IDE (Tested on v3.35)
XC16 compiler
PICKIT programmer (Tested with Pickit3)

Hardware requirements
---------------------
PSLab hardware/PIC24EP256GP204 breakout board.

Kits and accessories are available with praveenkumar103 [at] gmail, or jithinbp [at] gmail.com . 

Parts list
----------
- PIC24EP256GP204 - Main uC
- MCP6S21 - Programmable Gain Amplifier
- MCP4728 - 4 channel DAC
- TC7660  - Charge Pump voltage invertor
- TC1240A - Charge Pump voltage doubler
- TL082   - 2 channel Op-Amp
- LM324   - 4 channel Op-Amp
- LM1117 3.3 - 3.3Volt regulator

- MCP2200 - USB-UART Bridge
- or ESP8266 (ESP-12E) - UART-TCP bridge

- 0.5A Fuse
- 100K Potentiometer
- Assorted resistors, capacitors, & diodes.

Complete parts list pdf is included in the repo
