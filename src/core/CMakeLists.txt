set (CORE_SRCS
    AABB.cpp
    BVH.cpp
    Ray.h)

add_library(core OBJECT ${CORE_SRCS})

target_compile_options(core
    PRIVATE
        $<$<COMPILE_LANGUAGE:CXX>:-fPIC>)

target_include_directories(core
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    PRIVATE
        ${Qt5Widgets_INCLUDE_DIRS}
        ${GLM_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/../shapes
        ${CMAKE_CURRENT_SOURCE_DIR}/../shading
        ${CMAKE_CURRENT_SOURCE_DIR}/../ui/rasterization)
