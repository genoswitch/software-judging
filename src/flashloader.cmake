# Submodule path set by flashloader-helper.
set (FLASHLOADER_MODULE_PATH ../lib/pico-flashloader)

# 'Subproject' name
set(FLASHLOADER pico-flashloader)

add_executable(${FLASHLOADER})

target_sources(${FLASHLOADER} PUBLIC
        ${FLASHLOADER_MODULE_PATH}/flashloader.c
        )

        target_link_libraries(${FLASHLOADER} PRIVATE
        hardware_structs
        hardware_sync
        hardware_flash
        hardware_watchdog
        hardware_resets
        hardware_xosc
        hardware_clocks
        hardware_pll
        hardware_dma
        pico_platform
        pico_standard_link
        pico_divider
        )

pico_add_bin_output(${FLASHLOADER})
pico_set_program_name(${FLASHLOADER} ${FLASHLOADER})
target_compile_options(${FLASHLOADER} PRIVATE -Wall -Wextra -Wno-ignored-qualifiers -Os)


# Use a separate linker script for the flashloader to make sure it is built
# to run at the right location and cannot overflow into the applications's
# address space
set_linker_script(${FLASHLOADER} flashloader/flashloader.ld)

set(FLASHLOADER_UF2 ${CMAKE_CURRENT_BINARY_DIR}/${FLASHLOADER}.uf2)