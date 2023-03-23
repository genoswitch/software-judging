set (PROJECT pico)


# Min CMake version
cmake_minimum_required(VERSION 3.13)

# Project definitions
# Src: https://github.com/raspberrypi/pico-examples/blob/f800a7e3031e278062639e070c5764adb4a8a0fc/CMakeLists.txt#L6
project(${PROJECT} C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Set the flashloader submodule path and define the 'set_linker_script' function
include(flashloader-helper.cmake)

# Main application, including TinyUSB and Pico SDK
# TODO: move pico SDK out of that, put it here?
include(main.cmake)

# Flashloader scripts
include(flashloader.cmake)

# Scripts to combine the flashloader and main app together into a single bootable uf2.
include(combined.cmake)