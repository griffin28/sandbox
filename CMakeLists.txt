# Created by and for Qt Creator This file was created for editing the project sources only.
# You may attempt to use it for building too, by modifying this file here.

cmake_minimum_required(VERSION 3.20)
project(sandbox 
        VERSION 0.0.1 
        DESCRIPTION "Rendering Sandbox"
        HOMEPAGE_URL "https://github.com/griffin28/sandbox"
        LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
# Resource Compiler
set(CMAKE_AUTORCC ON)

set(Qt5_DIR "/home/kgriffin/VisIt/third_party/3.1.4/qt/5.10.1/linux-x86_64_gcc-9.3/lib/cmake/Qt5" 
    CACHE PATH 
    "QT install location")
mark_as_advanced(Qt5_DIR)
find_package(Qt5 COMPONENTS Core Gui Widgets OpenGL Network PrintSupport Qml Svg Xml UiTools Concurrent X11Extras REQUIRED)

set(glm_DIR "${CMAKE_SOURCE_DIR}/glm/cmake/glm" CACHE PATH "GLM install location")
find_package(glm REQUIRED)

set (SRCS
    AbstractGLScene.cpp
    Canvas.cpp
    main.cpp
    MainWidget.cpp
    MainWindow.cpp
    SimpleScene.cpp
    HUD.cpp
    resources/resources.qrc
)

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# add_subdirectory(<sourcedir> [<binarydir>])
# Shading Models
add_subdirectory(shading)

# Shapes
add_subdirectory(shapes)

# Dialogs
add_subdirectory(dialogs)

# stb
# mainly for image loading/decoding
add_subdirectory(stb)

add_executable(${CMAKE_PROJECT_NAME} ${SRCS})

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE stb shapes dialogs shading Qt5::Core Qt5::Gui Qt5::Widgets Qt5::OpenGL Qt5::Network Qt5::PrintSupport Qt5::Qml Qt5::Svg Qt5::Xml Qt5::UiTools Qt5::Concurrent Qt5::X11Extras glm::glm)
