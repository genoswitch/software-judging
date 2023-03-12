set(PICO_SDK_PATH ../lib/pico-sdk)
set(FREERTOS_KERNEL_PATH ../lib/freertos-smp)
set(PICO_TINYUSB_PATH ${CMAKE_CURRENT_LIST_DIR}/../lib/tinyusb)

# Min CMake version
cmake_minimum_required(VERSION 3.13)

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

# Pull in FreeRTOS
include(FreeRTOS_Kernel_import.cmake)

# Include git version tracking submodule
add_subdirectory(../lib/cmake-git-version-tracking git) # copy/compile to "git" folder in the build directory *^* build/git

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
target_include_directories(main PRIVATE 
    ${CMAKE_CURRENT_LIST_DIR}/rtos-config/
    ${CMAKE_CURRENT_LIST_DIR}/tinyusb-config/

)

target_sources(main PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/main.c
    ${CMAKE_CURRENT_LIST_DIR}/usb/tasks.c
    ${CMAKE_CURRENT_LIST_DIR}/usb/vendor_request.c
    # TinyUSB functions not picked up unless we include it here (tud_vendor_control_xfer_cb)
    ${CMAKE_CURRENT_LIST_DIR}/usb/webusb.c
    ${CMAKE_CURRENT_LIST_DIR}/tinyusb-config/usb_descriptors.c
    ${CMAKE_CURRENT_LIST_DIR}/tasks/bulk.c
    ${CMAKE_CURRENT_LIST_DIR}/tasks/mcu_temperature.c
    ${CMAKE_CURRENT_LIST_DIR}/config.h
    )

# Link to libraries  (after sdk init)
# pico_stdlib needed as FreeRTOS uses panic_unsupported
# memory management: FreeRTOS-Kernel-Heap# required for pvPortMalloc
# tinyusb_device tinyusb_board (https://github.com/raspberrypi/pico-examples/blob/master/usb/device/dev_hid_composite/CMakeLists.txt)
target_link_libraries(main pico_stdlib hardware_adc pico_unique_id FreeRTOS-Kernel FreeRTOS-Kernel-Heap4 tinyusb_device tinyusb_board cmake_git_version_tracking)

# stdio only on UART (UART0 by default, pins 1 and 2)
pico_enable_stdio_usb(main 0)
pico_enable_stdio_uart(main 1)

# Create extra files, such as .uf2
pico_add_extra_outputs(main)