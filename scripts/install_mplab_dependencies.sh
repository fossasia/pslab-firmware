#!/bin/bash

# Install pre-requisites
sudo apt-get install libc6:i386 libx11-6:i386 libxext6:i386 libstdc++6:i386 libexpat1:i386

# Create a folder for the installation files.
mkdir ../install-dependencies
cd ../install-dependencies

# Install MPLAB X IDE v3.15
echo "Downloading MPLAB X IDE"
wget https://www.microchip.com/content/dam/mchp/documents/DEV/ProductDocuments/SoftwareTools/mplabx-v5.45-linux-installer.tar

echo "Extracting MPLAB X IDE"
tar -xvf mplabx-v5.45-linux-installer.tar

echo "Installing MPLAB X IDE"
./MPLABX-v5.45-linux-installer.sh -- --mode unattended --unattendedmodeui minimal --debuglevel 0 --8bitmcu 0 --32bitmcu 0 --othermcu 0

echo "MPLAB IDE installed successfully"

echo "Downloading MPLAB XC 16 Compiler"
wget https://ww1.microchip.com/downloads/en/DeviceDoc/xc16-v1.70-full-install-linux64-installer.run

echo "Installing MPLAB XC 16 Compiler"
chmod +x xc16-v1.70-full-install-linux64-installer.run
./xc16-v1.70-full-install-linux64-installer.run --mode unattended --netservername dontknow

echo "Compiler setup successfully"
