# 🔬 PSLab Firmware

This repository contains firmware for the [Pocket Science Lab (PSLab)](https://pslab.io) open hardware platform. Hardware versions 5 and 6 are supported.

---

## 🌟 Overview

The PSLab provides an array of test and measurement instruments for science and engineering experiments.

Its built-in instruments include:

- Oscilloscope  
- Waveform Generator  
- Frequency Counter  
- Programmable Voltage & Current Sources  
- Logic Analyzer  

The PSLab also supports communication via:

- UART  
- I2C  
- SPI  

---

## 🧩 Open Source Ecosystem

The PSLab is a fully open device, and FOSSASIA provides a complete hardware and software stack:

- Hardware  
- Bootloader  
- Firmware (this repository)  
- Python library  
- Graphical desktop application  
- Android app  

---

## 🛒 Get a PSLab Device

- FOSSASIA Shop → https://fossasia.com/shop  
- Official Website → https://pslab.io  

---

## 🔔 Stay Updated

- Gitter Chat → https://gitter.im/fossasia/pslab  
- Mailing List → https://groups.google.com/g/pslab  

---

## ⚙️ Dependencies

The following tools are required to build the firmware:

- xc16 compiler  
- cmake  

---

## 🚀 Building

This project is built with CMake.

### 1. Clone Repository

```bash
git clone https://github.com/fossasia/pslab-firmware.git
cd pslab-firmware
```

### 2. Initialize and Update Submodules

```bash
git submodule init
git submodule update
```

### 3. Build Firmware

```bash
mkdir build
cd build
cmake ..
make
```

This will create:

```bash
pslab-firmware.hex
```

### Build for PSLab v5

```bash
cmake -DLEGACY_HARDWARE=true ..
```

---

## 🔥 Flashing Firmware

The firmware can be flashed over USB or by using a programmer such as the PICkit3.

### 🔌 USB Method

Firmware can be flashed over USB if the device already has the bootloader installed.

Flashing requires `mcbootflash`, which can be installed via:

- https://github.com/fossasia/mcbootflash  
- https://github.com/fossasia/pslab-python  

### Steps:

1. Press and hold the **BOOT** button  
2. Press the **RESET** button  
3. The status LED will start blinking  
4. Release the BOOT button  
5. Run:

```bash
mcbootflash --port <PORT> -b 460800 pslab-firmware.hex
```

6. Reset or power cycle the device  

---

### ⚠️ Using a Programmer

If flashing using a programmer:

> ⚠️ Warning: Flashing firmware directly may overwrite the bootloader.

It is recommended to create a combined HEX file containing both bootloader and firmware.

Steps:

1. Disconnect device  
2. Connect programmer to ICSP header  
3. Power device via USB  
4. Run:

```bash
mdb.sh flash.mdbscript
```

5. Disconnect programmer  

---

## 🔁 Bootloader Mode (PSLab v5)

PSLab v5 does not have a BOOT button.

To enter bootloader mode:

- Bridge BOOT pin and GND  
- Reset or power cycle device  
- LED should start blinking  

---

## 🐳 Dev Container Usage

This repository supports development using VS Code Dev Containers and GitHub Codespaces.

Steps:

1. Open in VS Code  
2. Click "Reopen in Container"  
3. Start development  

---

## 📦 Repository Structure

```
pslab-firmware/
├── src/
├── external/
├── CMakeLists.txt
├── flash.mdbscript
└── README.md
```

---

## 🤝 Contributing

1. Fork the repository  
2. Create a branch  
3. Commit changes  
4. Submit a Pull Request  

---

## 📄 License

Apache 2.0  

---

# ❤️ Built by FOSSASIA
