set (SHADING_SRCS
    lambert.cpp
)

add_library(shading OBJECT ${SHADING_SRCS})

target_include_directories(shading PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
