# Combine the flashloader and main binaries to one bootable UF2.

set(APP_BIN ${CMAKE_CURRENT_BINARY_DIR}/main.elf)
set(FLASHLOADER_BIN ${CMAKE_CURRENT_BINARY_DIR}/pico-flashloader.bin)
set(COMBINED combined_new)
add_executable(${COMBINED} linker/null.c)
target_link_libraries(${COMBINED} pico_stdlib) # annoying hh
add_dependencies(${COMBINED} main pico-flashloader)
add_custom_command(TARGET ${COMBINED} DEPENDS ${FLASHLOADER_BIN}
	COMMAND ${CMAKE_OBJCOPY}
		--update-section .flashloader=${FLASHLOADER_BIN} ${APP_BIN} ${COMBINED}.elf
)

add_custom_command(TARGET ${COMBINED} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -Obinary ${COMBINED}.elf ${COMBINED}.bin
)

add_custom_command(TARGET ${COMBINED} POST_BUILD 
    COMMAND ${CMAKE_OBJCOPY} -Oihex  ${COMBINED}.elf ${COMBINED}.hex
)