# Combine the flashloader and main binaries to one bootable UF2.

set(APP_BIN ${CMAKE_CURRENT_BINARY_DIR}/main.bin)
set(COMBINED combined_new)
add_custom_target(${COMBINED} ALL)
add_dependencies(${COMBINED} main pico-flashloader)
add_custom_command(TARGET ${COMBINED} DEPENDS ${APP_BIN}
	COMMAND ${CMAKE_OBJCOPY}
		--update-section .app=${APP_BIN} ${CMAKE_CURRENT_BINARY_DIR}/pico-flashloader.elf ${COMBINED}.elf
)

add_custom_command(TARGET ${COMBINED} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -Obinary ${COMBINED}.elf ${COMBINED}.bin
)

add_custom_command(TARGET ${COMBINED} POST_BUILD 
    COMMAND ${CMAKE_OBJCOPY} -Oihex  ${COMBINED}.elf ${COMBINED}.hex
)