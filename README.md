Bare Metal ESP32-S3 Project
============================

[![Build Status](https://github.com/chalandi/Baremetal_esp32s3_nosdk/actions/workflows/Baremetal_esp32s3_nosdk.yml/badge.svg)](https://github.com/chalandi/Baremetal_esp32s3_nosdk/actions)

This repository implements a fully manually-written bare-metal project for the ESP32-S3 dual-core Xtensa LX7 SoC, without using Espressif's SDK.

Features :
  - Dual-core Xtensa LX7 booting support
  - ULP-RISC-V coprocessor booting support
  - Clock configuration: APB clock set to 80 MHz, SoC clock set to 240 MHz
  - Interrupt vector tables for both cores
  - 1 Hz interrupt generated from the Xtensa LX7 private timer (Timer1 interrupt via IRQ6)
  - LED blinking from both cores
  - WS2812 switching color from core 1 interrupt
  - Multicore Debug environment configuration for VSCode (using the built-in JTAG interface, GDB and OpenOCD)


A clear and easy-to-understand implementation in C11 and assembly with a build system based on GNU Make makes this project both fun and educational.


This repository provides valuable insight into starting a bare-metal ESP32-S3 project.


## Details on the Application

The ESP32-S3's bootRom loads this software from flash memory into internal SRAM during a cold boot.

The low-level startup process begins on core 0, initializing the C/C++ environment and configuring the clock before starting core 1 and the coprocessor (ULP-RISC-V).

Both cores then enable interrupts and enter an idle loop. Each core toggles an LED at a 1 Hz frequency using its private timer interrupt.

## Building the Application

To build the project, you need an installed Xtensa GCC compiler (xtensa-esp32s3-elf).

Run the following commands :

```sh
cd ./Build
Rebuild.sh
```

The build process generates the following outputs in the `Output` directory :

  - ELF file
  - HEX mask
  - Assembly listing
  - MAP file
  - Binary file for flashing to ESP32-S3 QSPI memory

## Tools

This tools is needed to build, flash and debug this project:

  - Xtensa GCC compiler (https://github.com/espressif/crosstool-NG/releases)
  - OpenOCD for ESP32   (https://github.com/espressif/openocd-esp32/releases)
  - GDB for ESP32       (https://github.com/espressif/binutils-gdb/releases)
  - esptool             (run this command: pip install esptool)


## Continuous Integration

CI runs on pushes and pull-requests with a simple build and result verification on `ubuntu-latest` using GitHub Actions.
