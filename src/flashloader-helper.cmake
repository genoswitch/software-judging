# Submodule path
set (FLASHLOADER_MODULE_PATH ../lib/pico-flashloader)

# Helper function (based on lib/pico-flashloader/CMakeLists.txt#L22)
function(set_linker_script TARGET script)
    target_link_directories(${TARGET} PRIVATE ${FLASHLOADER_MODULE_PATH})
    pico_set_linker_script(${TARGET} ${FLASHLOADER_MODULE_PATH}/${script})

    # Add dependencies on the 'included' linker scripts so that the target gets
    # rebuilt if they are changed
    pico_add_link_depend(${TARGET} ${FLASHLOADER_MODULE_PATH}/memmap_defines.ld)
    pico_add_link_depend(${TARGET} ${FLASHLOADER_MODULE_PATH}/memmap_default.ld)
endfunction()