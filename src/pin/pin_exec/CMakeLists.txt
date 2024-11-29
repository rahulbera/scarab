cmake_minimum_required(VERSION 3.10)

# Add a custom target to invoke the existing Makefile
add_custom_target(run_makefile ALL
    COMMAND make
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Invoking the existing Makefile"
)