#======== Interface libraries
# Compiler Settings
#
# Add -save-temps=obj to force output of each stage to be stored in a file instead
# of memory (GCC and Clang)
#
# Add -H to see which paths are being used to include a requested header
add_library(compiler_props INTERFACE)
target_compile_options(compiler_props
    INTERFACE
        $<$<CONFIG:DEBUG>:-Wall -Wextra -Wpedantic>
        $<$<AND:$<CXX_COMPILER_ID:GNU>,$<CONFIG:DEBUG>>:-gstatement-frontiers -ginline-points> # extended debug info for inline funcs
)

#======== Macros
macro(create_find_package pkg required quiet components)
    if(("${required}" STREQUAL "1") AND ("${quiet}" STREQUAL "1"))
        find_package(${pkg} QUIET REQUIRED COMPONENTS ${components})
    elseif(${required})
        find_package(${pkg} REQUIRED COMPONENTS ${components})
    elseif(${quiet})
        find_package(${pkg} QUIET COMPONENTS ${components})
    endif()
endmacro(create_find_package)

#======== Functions
function(AddValgrind target)
    find_program(VALGRIND_PATH valgrind)
    if(NOT VALGRIND_PATH)
        add_custom_target(valgrind COMMAND false
            COMMENT "valgrind not found")
        return()
    endif()
    add_custom_target(valgrind 
        COMMAND ${VALGRIND_PATH} --leak-check=full $<TARGET_FILE:${target}>
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
endfunction()
