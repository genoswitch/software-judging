# Pico SDK must be imported before project()
set(PICO_SDK_PATH ../lib/pico-sdk)
set(PICO_TINYUSB_PATH ${CMAKE_CURRENT_LIST_DIR}/../lib/tinyusb)


# Project name
set (PROJECT pico)

# Min CMake version
cmake_minimum_required(VERSION 3.13)


# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

# Project definitions
# Src: https://github.com/raspberrypi/pico-examples/blob/f800a7e3031e278062639e070c5764adb4a8a0fc/CMakeLists.txt#L6
project(${PROJECT} C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Define the 'set_linker_script' function
include(linker/helper.cmake)

# Main application
include(main.cmake)

# Flashloader scripts
include(flashloader.cmake)

# Scripts to combine the flashloader and main app together into a single bootable uf2.
include(combined.cmake)