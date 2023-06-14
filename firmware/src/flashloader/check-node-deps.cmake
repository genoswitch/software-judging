message("Checking for lib/universal-hex dependencies...")

execute_process(
    WORKING_DIRECTORY ../lib/universal-hex
    RESULT_VARIABLE exit_code
    COMMAND ${NPM_EXECUTABLE} ls
)

if (NOT "${exit_code}" STREQUAL 0)
message("Dependencies not installed. Installing...")

execute_process(
    WORKING_DIRECTORY ../lib/universal-hex
    COMMAND ${NPM_EXECUTABLE} ci --no-scripts
)

else()
message("Dependencies already installed.")
endif()