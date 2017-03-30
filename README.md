# pslab-firmware
Repository for PSLab Firmware

uC Platform : PIC24EP256GP204


Implemented feature list
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
PSLab hardware/PIC24EP256GP204 breakout board

Parts list
----------
PIC24EP256GP204 - Main uC
MCP6S21 - Programmable Gain Amplifier
MCP4728 - 4 channel DAC
TC7660  - Charge Pump voltage invertor
TC1240A - Charge Pump voltage doubler
TL082   - 2 channel Op-Amp
LM324   - 4 channel Op-Amp
LM1117 3.3 - 3.3Volt regulator

MCP2200 - USB-UART Bridge
or ESP8266 (ESP-12E) - UART-TCP bridge

0.5A Fuse
100K Potentiometer
Assorted resistors, capacitors, & diodes.
