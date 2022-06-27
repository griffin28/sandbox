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
# set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
# Resource Compiler
set(CMAKE_AUTORCC ON)

set(Qt5_DIR "/home/kgriffin/VisIt/third_party/3.1.4/qt/5.10.1/linux-x86_64_gcc-9.3/lib/cmake/Qt5"
    CACHE PATH
    "QT install location")
mark_as_advanced(Qt5_DIR)
find_package(Qt5 COMPONENTS Core Gui Widgets OpenGL Network PrintSupport Qml Svg Xml UiTools Concurrent X11Extras REQUIRED)

set(glm_DIR "${CMAKE_SOURCE_DIR}/extern/glm/cmake/glm" CACHE PATH "GLM install location")
find_package(glm REQUIRED)

option(BUILD_TESTING "Build tests for CTest" OFF)
option(BUILD_DOCS "Build documentation" OFF)

# add_subdirectory(<sourcedir> [<binarydir>])

# External projects 
add_subdirectory(extern)

# Source directory
add_subdirectory(src bin)

# Tests 
if(BUILD_TESTING)
    add_subdirectory(test)
endif()

# Docs
if(BUILD_DOCS)
    add_subdirectory(doc)
endif()
