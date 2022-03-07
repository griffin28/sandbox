# Created by and for Qt Creator This file was created for editing the project sources only.
# You may attempt to use it for building too, by modifying this file here.

cmake_minimum_required(VERSION 3.5)
project(myopengl VERSION 0.0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
# Resource Compiler
set(CMAKE_AUTORCC ON)

set(Qt5_DIR "/home/kgriffin/VisIt/third_party/3.1.4/qt/5.10.1/linux-x86_64_gcc-9.3/lib/cmake/Qt5")
find_package(Qt5 COMPONENTS Core Gui Widgets OpenGL Network PrintSupport Qml Svg Xml UiTools Concurrent X11Extras REQUIRED)

set(glm_DIR "${CMAKE_SOURCE_DIR}/glm/cmake/glm")
find_package(glm REQUIRED)

set (SRCS
    AbstractGLScene.cpp
    AbstractGLScene.h
    Canvas.cpp
    Canvas.h
    main.cpp
    MainWidget.cpp
    MainWidget.h
    MainWindow.cpp
    MainWindow.h
    SimpleScene.cpp
    SimpleScene.h
    HUD.cpp
    HUD.h
    shapes/shape.cpp
    shapes/shape.h
    shapes/sphere.cpp
    shapes/sphere.h
    resources/resources.qrc
)

# add_subdirectory(<sourcedir> [<binarydir>])
# vmath library
# add_subdirectory(math)

# Shapes
# add_subdirectory(shapes)

# stb
# mainly for image loading/decoding
add_subdirectory(stb)

add_executable(${CMAKE_PROJECT_NAME} ${SRCS})

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets Qt5::OpenGL Qt5::Network Qt5::PrintSupport Qt5::Qml Qt5::Svg Qt5::Xml Qt5::UiTools Qt5::Concurrent Qt5::X11Extras stb glm::glm)
