set(PICO_SDK_PATH ../../lib/pico-sdk)
set(FREERTOS_KERNEL_PATH ../../lib/freertos-smp)

# Min CMake version
cmake_minimum_required(VERSION 3.13)

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

# Pull in FreeRTOS
#include(FreeRTOS_Kernel_import.cmake)

# Project definitions
# Src: https://github.com/raspberrypi/pico-examples/blob/f800a7e3031e278062639e070c5764adb4a8a0fc/CMakeLists.txt#L6
project(pico C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Init SDK
pico_sdk_init()


# Add executable targets
#add_executable(main main.c)

# Link to libraries  (after sdk init)
target_link_libraries(main pico_stdlib)

# Create extra files, such as .uf2
#pico_add_extra_outputs(main)