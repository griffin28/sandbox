set (SHAPE_SRCS
    shape.cpp
    sphere.cpp)

INCLUDE_DIRECTORIES(
    ${GLM_INCLUDE_DIRS}
)

add_library(shapes OBJECT ${SHAPE_SRCS})

target_include_directories(shapes PUBLIC .)
