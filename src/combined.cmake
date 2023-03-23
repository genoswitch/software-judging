# Based on lib/pico-flashloader/CMakeLists.txt#L110

# Combine the flashloader and main application into one flashable image!

set(COMPLETE_UF2 ${CMAKE_CURRENT_BINARY_DIR}/main_full.uf2)

find_package (Python3 REQUIRED COMPONENTS Interpreter)
add_custom_command(OUTPUT ${COMPLETE_UF2} DEPENDS ${FLASHLOADER} ${MAIN}
        COMMENT "Building combined UF2 image (${FLASHLOADER} + ${MAIN})"
        COMMAND ${Python3_EXECUTABLE}
                ${FLASHLOADER_MODULE_PATH}/uf2tool.py
                -o ${COMPLETE_UF2} ${FLASHLOADER_UF2} ${MAIN_UF2}
        )

# Make the entire project depend on the combine stage
add_custom_target(${PROJECT} ALL DEPENDS ${COMPLETE_UF2})

# Does not seem to be needed?
# install(FILES ${COMPLETE_UF2} DESTINATION ${CMAKE_INSTALL_PREFIX} )