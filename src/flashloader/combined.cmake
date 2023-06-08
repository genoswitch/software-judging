# Combine the flashloader and main binaries to one bootable UF2.

set(APP_BIN ${CMAKE_CURRENT_BINARY_DIR}/${MAIN_FL_LINKED}.elf)
set(FLASHLOADER_BIN ${CMAKE_CURRENT_BINARY_DIR}/${FLASHLOADER}.bin)
set(COMBINED main_flashloader)
add_executable(${COMBINED} ${CMAKE_CURRENT_LIST_DIR}/null.c)
target_link_libraries(${COMBINED} pico_stdlib) # annoying hh
add_dependencies(${COMBINED} ${MAIN_FL_LINKED} ${FLASHLOADER})

# Add a dependency on the output (target) files from main.
# This will trigger a rebuild of the combined target when the main project is changed.
# Src: https://stackoverflow.com/a/69712159
set_target_properties(${COMBINED} PROPERTIES
    LINK_DEPENDS $<TARGET_FILE:${MAIN_FL_LINKED}>
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

set(SECTIONED_DIR hex_sectioned)

add_custom_command(TARGET ${COMBINED} POST_BUILD
    COMMENT "Add temporary directory '${SECTIONED_DIR}' for sectioned hex files"
    COMMAND ${CMAKE_COMMAND} -E make_directory ${SECTIONED_DIR}
)
# Create a hex file WITHOUT the flashloader section
add_custom_command(TARGET ${COMBINED} POST_BUILD 
    COMMENT "Creating (${COMBINED}) ${SECTIONED_DIR}/app.hex"
    # Adjust the offset by 4k (0x1000) to match where the program is in the normal .hex
    # Otherwise the flashloader section is removed but the remaining contents are shifted to start at the flashloader start address.
    COMMAND ${CMAKE_OBJCOPY} --remove-section .flashloader --change-start 0x1000 -Oihex  ${COMBINED}.elf ${SECTIONED_DIR}/app.hex
)

# Create a hex file WITH ONLY the flashloader section
add_custom_command(TARGET ${COMBINED} POST_BUILD 
    COMMENT "Creating (${COMBINED}) ${SECTIONED_DIR}/flashloader.hex"
    COMMAND ${CMAKE_OBJCOPY} --only-section .flashloader -Oihex  ${COMBINED}.elf ${SECTIONED_DIR}/flashloader.hex
)

# ----- Start
# Find nodejs executable script.
# Src: https://github.com/eclipse/upm/blob/d6f76ff8c231417666594214679c49399513112e/cmake/modules/FindNode.cmake#L8-L12
find_program (NODEJS_EXECUTABLE NAMES node nodejs
    HINTS
    $ENV{NODE_DIR}
    PATH_SUFFIXES bin
    DOC "Node.js interpreter")

# Src: https://github.com/eclipse/upm/blob/d6f76ff8c231417666594214679c49399513112e/cmake/modules/FindNpm.cmake#L14
find_program (NPM_EXECUTABLE NAMES npm
    HINTS
    /usr
    DOC "NPM (Node Package Manager)"
)
# ----- End

if(NPM_EXECUTABLE)
    message("Found npm executable at '${NPM_EXECUTABLE}'")
    # Runs before other rules
    # TODO: Would be better if we could run this at configure time.
    add_custom_command(TARGET ${COMBINED} PRE_BUILD
        COMMENT "Installing npm dependencies for universal-hex"
        WORKING_DIRECTORY ../lib/universal-hex
        COMMAND ${NPM_EXECUTABLE} ci --no-scripts
    )
else()
    message(FATAL_ERROR "NPM executable not found. Unable to create universal hex.")
endif()

if (NODEJS_EXECUTABLE)
    message("Found Node.js executable at '${NODEJS_EXECUTABLE}'")

    add_custom_command(TARGET ${COMBINED} POST_BUILD
        COMMENT "Creating (${COMBINED}) ${SECTIONED_DIR}/universal.hex"
        COMMAND ${NODEJS_EXECUTABLE} ../lib/universal-hex/main.js ${SECTIONED_DIR}/flashloader.hex ${SECTIONED_DIR}/app.hex ${SECTIONED_DIR}/universal.hex
    )
else()
    message(FATAL_ERROR "Node.js executable not found. Unable to create universal hex.")
endif()


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
