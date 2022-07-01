#======== Interface libraries
# Compiler Settings
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
