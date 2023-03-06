set (CAMERA_SRCS
    Camera.cpp
    PerspectiveCamera.cpp)

INCLUDE_DIRECTORIES(
    ${GLM_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../core
)

add_library(camera OBJECT ${CAMERA_SRCS})

target_include_directories(camera PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
