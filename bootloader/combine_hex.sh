#!/bin/sh
hexmate r0-2FFF,build/pslab-bootloader.hex r3000-54FFF,build/pslab-firmware.hex r55000-FFFFFFFF,build/pslab-bootloader.hex -Obuild/combined.hex
