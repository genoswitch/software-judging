set(MAIN_FL_LINKED intermediate_main_flashloader_linked)

add_executable(${MAIN_FL_LINKED} core/main.c)
target_link_libraries(${MAIN_FL_LINKED} ${MAIN})
# Based on lib/pico-flashloader/CMakeLists.txt#L86
# Use a separate linker script for the application to make sure it is built
# to run at the right location (after the flashloader).
set_linker_script(${MAIN_FL_LINKED} flashloader/application.ld)