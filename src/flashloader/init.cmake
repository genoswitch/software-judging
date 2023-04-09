# Define an executable target linked against core, linked with space for the flashloader.
include(${CMAKE_CURRENT_LIST_DIR}/main.cmake)

# Define an exectuable target for the flashloader itself.
include(${CMAKE_CURRENT_LIST_DIR}/flashloader.cmake)

# Define an executable target that combines the two targets into one executable.
# (Adds the flashloader to the space assigned by the custom linker script)
# (Scripts to combine the flashloader and main app together into a single bootable uf2)
include(${CMAKE_CURRENT_LIST_DIR}/combined.cmake)