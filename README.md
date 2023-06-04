# pico-beacon-c

## About
- ble beacon program on raspberry pi pico w.

## Dependency
- cmake
- arm-none-eabi-gcc
- pico-sdk
- pico-extras

## Setup
- https://github.com/raspberrypi/pico-sdk/tree/master/.github/workflows

## Usage
- git clone with submodules(pico-sdk, pico-extras)
```
$ git clone ... --recursive

# or after clone
$ git submodule update --init --recursive
```

- build uf2 file for firmware in pico w.
```
$ make
[  0%] Performing build step for 'PioasmBuild'
[  0%] Performing build step for 'ELF2UF2Build'
[  1%] Built target bs2_default
[  2%] Built target bs2_default_padded_checksummed_asm
[100%] Built target elf2uf2
[100%] Built target pioasm
[  2%] No install step for 'ELF2UF2Build'
[  2%] No install step for 'PioasmBuild'
[  2%] Completed 'ELF2UF2Build'
[  3%] Completed 'PioasmBuild'
[  5%] Built target ELF2UF2Build
[  7%] Built target PioasmBuild
[  7%] Built target cyw43_driver_picow_cyw43_bus_pio_spi_pio_h
[  8%] Building C object CMakeFiles/le_advertise.dir/src/le_advertise.c.obj
[  9%] Linking CXX executable le_advertise.elf
[100%] Built target le_advertise
```
