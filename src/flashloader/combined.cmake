# Combine the flashloader and main binaries to one bootable UF2.

set(APP_BIN ${CMAKE_CURRENT_BINARY_DIR}/main.elf)
set(FLASHLOADER_BIN ${CMAKE_CURRENT_BINARY_DIR}/pico-flashloader.bin)
set(COMBINED combined_new)
add_executable(${COMBINED} ${CMAKE_CURRENT_LIST_DIR}/null.c)
target_link_libraries(${COMBINED} pico_stdlib) # annoying hh
add_dependencies(${COMBINED} main pico-flashloader)

# Add a dependency on the output (target) files from main.
# This will trigger a rebuild of the combined target when the main project is changed.
# Src: https://stackoverflow.com/a/69712159
set_target_properties(${COMBINED} PROPERTIES
    LINK_DEPENDS $<TARGET_FILE:main>
)

add_custom_command(TARGET ${COMBINED} DEPENDS ${FLASHLOADER_BIN}
    COMMENT "Injecting flashloader into main to create ${COMBINED}.elf"
	COMMAND ${CMAKE_OBJCOPY}
		--update-section .flashloader=${FLASHLOADER_BIN} ${APP_BIN} ${COMBINED}.elf
)

add_custom_command(TARGET ${COMBINED} POST_BUILD
    COMMENT "Creating ${COMBINED}.bin"
    COMMAND ${CMAKE_OBJCOPY} -Obinary ${COMBINED}.elf ${COMBINED}.bin
)

add_custom_command(TARGET ${COMBINED} POST_BUILD 
    COMMENT "Creating ${COMBINED}.hex"
    COMMAND ${CMAKE_OBJCOPY} -Oihex  ${COMBINED}.elf ${COMBINED}.hex
)

# Create a UF2.
# pico-flashloader uses custom python script to both combine and create the final uf2.
# However, we now use another method using objcopy similar to what picowota does.
# <https://github.com/usedbytes/picowota>
# Usedbytes demonstrates that the pico sdk has a tool for creating uf2 files.
# We can simply call the same tool to generate our combined uf2.

# 1. Find ELF2UF2 if it is not already available/found
if (NOT ELFUF2_FOUND)
    # Append the Pico SDK tools directory to the cmake PATH.
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PICO_SDK_PATH}/tools)
    # Call CMake's find_package function.
    find_package(ELF2UF2)
endif()

# 2. ELF2UF2 should now be available.
if (ELF2UF2_FOUND)
    add_custom_command(TARGET ${COMBINED} POST_BUILD
        COMMENT "Creating ${COMBINED}.uf2"
        COMMAND ELF2UF2 ${COMBINED}.elf ${COMBINED}.uf2
    )
else()
    message(FATAL_ERROR "ELF2UF2 not found in the pico sdk. Unable to generate ${COMBINED}.uf2")
endif()
