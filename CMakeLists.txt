cmake_minimum_required(VERSION 3.13)

set(PICO_BOARD pico_w)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)
#include(pico_extras_import.cmake)

project(pico-beacon-c)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project

add_executable(le_advertise
    src/le_advertise.c
)

target_link_libraries(le_advertise
  pico_cyw43_arch
  pico_stdlib
  pico_btstack_ble
  pico_btstack_classic
  pico_btstack_cyw43
  hardware_i2c
  pico_cyw43_arch_lwip_threadsafe_background
)

pico_add_extra_outputs(le_advertise)
add_compile_options(-Wall
        -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
        -Wno-unused-function # we have some for the docs that aren't called
        -Wno-maybe-uninitialized
        )

include_directories(${PICO_SDK_PATH}/lib/btstack/platform/lwip)
include_directories(src)

pico_enable_stdio_usb(le_advertise 1)
pico_enable_stdio_uart(le_advertise 0)

set(BTSTACK_ROOT ${PICO_SDK_PATH}/lib/btstack)

