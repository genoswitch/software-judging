set(MAIN_FL_LINKED intermediate_main_flashloader_linked)

set (RINGBUF_MODULE_PATH ../lib/ringbuf)
set (FLASHLOADER_MODULE_PATH ../lib/pico-flashloader)
include_directories(${RINGBUF_MODULE_PATH})
include_directories(${FLASHLOADER_MODULE_PATH})

add_executable(${MAIN_FL_LINKED} core/main.c)

target_link_libraries(${MAIN_FL_LINKED} ${MAIN} hardware_dma pico_mem_ops)

target_compile_definitions(${MAIN_FL_LINKED} PRIVATE INCLUDES_FLASHLOADER)

target_sources(${MAIN_FL_LINKED} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/crc32.c
    ${CMAKE_CURRENT_LIST_DIR}/main.c
    ${CMAKE_CURRENT_LIST_DIR}/dfu.c
    ${CMAKE_CURRENT_LIST_DIR}/spinlock.c
    ${CMAKE_CURRENT_LIST_DIR}/ihex/record.c
    ${CMAKE_CURRENT_LIST_DIR}/ihex/util.c
    ${CMAKE_CURRENT_LIST_DIR}/ihex/process.c
    ${CMAKE_CURRENT_LIST_DIR}/ihex/flash.c
    ${RINGBUF_MODULE_PATH}/ringbuf.c
    ${RINGBUF_MODULE_PATH}/ringbuf.h
    ${FLASHLOADER_MODULE_PATH}/flashloader.h
)

# Based on lib/pico-flashloader/CMakeLists.txt#L86
# Use a separate linker script for the application to make sure it is built
# to run at the right location (after the flashloader).
set_linker_script(${MAIN_FL_LINKED} flashloader/application.ld)