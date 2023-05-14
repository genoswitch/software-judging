set(MAIN_FL_LINKED intermediate_main_flashloader_linked)

add_executable(${MAIN_FL_LINKED} core/main.c)

target_link_libraries(${MAIN_FL_LINKED} ${MAIN} hardware_dma)

target_compile_definitions(${MAIN_FL_LINKED} PRIVATE INCLUDES_FLASHLOADER)

target_sources(${MAIN_FL_LINKED} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/crc32.c
    ${CMAKE_CURRENT_LIST_DIR}/flashloader.c
    ${CMAKE_CURRENT_LIST_DIR}/dfu.c
)

# Based on lib/pico-flashloader/CMakeLists.txt#L86
# Use a separate linker script for the application to make sure it is built
# to run at the right location (after the flashloader).
set_linker_script(${MAIN_FL_LINKED} flashloader/application.ld)