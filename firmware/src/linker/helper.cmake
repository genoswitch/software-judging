# Submodule path
set (LINKER_FOLDER ${CMAKE_CURRENT_LIST_DIR})

# Helper function (based on lib/pico-flashloader/CMakeLists.txt#L22)
function(set_linker_script TARGET script)
    target_link_directories(${TARGET} PRIVATE ${LINKER_FOLDER})
    target_link_directories(${TARGET} PRIVATE ${LINKER_FOLDER}/flashloader)
    # Add dependencies on the 'included' linker scripts so that the target gets
    # rebuilt if they are changed
    pico_set_linker_script(${TARGET} ${LINKER_FOLDER}/${script})
endfunction()