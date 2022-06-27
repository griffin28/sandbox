set (SHAPE_SRCS
    shape.cpp
    sphere.cpp)

INCLUDE_DIRECTORIES(
    ${GLM_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../shading
)

add_library(shapes OBJECT ${SHAPE_SRCS})

target_include_directories(shapes PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
