#======== Interface libraries
# Compiler Warnings
add_library(warning_props INTERFACE)
target_compile_options(warning_props INTERFACE -Wall -Wextra -Wpedantic)

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
