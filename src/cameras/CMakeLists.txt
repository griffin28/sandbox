set (CAMERA_SRCS
    Camera.cpp
    PerspectiveCamera.cpp
    OrthographicCamera.cpp)

add_library(cameras OBJECT ${CAMERA_SRCS})

target_include_directories(cameras
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    PRIVATE
        ${GLM_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/../core)
