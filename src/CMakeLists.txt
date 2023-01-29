set(PICO_SDK_PATH ../lib/pico-sdk)
set(FREERTOS_KERNEL_PATH ../lib/freertos-smp)
set(PICO_TINYUSB_PATH ${CMAKE_CURRENT_LIST_DIR}/../lib/tinyusb)

# Min CMake version
cmake_minimum_required(VERSION 3.13)

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

# Pull in FreeRTOS
include(FreeRTOS_Kernel_import.cmake)

# Project definitions
# Src: https://github.com/raspberrypi/pico-examples/blob/f800a7e3031e278062639e070c5764adb4a8a0fc/CMakeLists.txt#L6
project(pico C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Init SDK
pico_sdk_init()


# Add executable targets
add_executable(main main.c)


# RTOS config "FreeRTOSConfig.h"
target_include_directories(main PRIVATE ${CMAKE_CURRENT_LIST_DIR}/rtos-config/)

# Link to libraries  (after sdk init)
# pico_stdlib needed as FreeRTOS uses panic_unsupported
# memory management: FreeRTOS-Kernel-Heap# required for pvPortMalloc
target_link_libraries(main pico_stdlib FreeRTOS-Kernel FreeRTOS-Kernel-Heap4)

# stdio only on UART (UART0 by default, pins 1 and 2)
pico_enable_stdio_usb(main 0)
pico_enable_stdio_uart(main 1)

# Create extra files, such as .uf2
pico_add_extra_outputs(main)