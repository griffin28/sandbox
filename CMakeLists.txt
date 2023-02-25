# Created by and for Qt Creator This file was created for editing the project sources only.
# You may attempt to use it for building too, by modifying this file here.

cmake_minimum_required(VERSION 3.20)
project(sandbox
        VERSION 0.0.1 
        DESCRIPTION "Rendering Sandbox"
        HOMEPAGE_URL "https://github.com/griffin28/sandbox"
        LANGUAGES C CXX)

# set(CMAKE_DIABLE_SOURCE_CHANGES ON) and
# set(CMAKE_DIABLE_IN_SOURCE_BUILD ON) - creates an error, however,
# files can still be created in the source directory.
# This is an older alternative:
if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(FATAL_ERROR "In-source builds are not allowed.")
endif()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
# Resource Compiler
set(CMAKE_AUTORCC ON)

# Peform optimization after linking so that all compilation
# units can be optimized as a single module
#include(CheckIPOSupported)
#check_ipo_supported(RESULT ipo_supported)
#if(ipo_supported)
#    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION true)
#endif()

# Add custom find module path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/")

include(SandboxUtils)

# Find packages
find_package(SandboxQt REQUIRED)
find_package(GLM REQUIRED)

# Options
option(BUILD_TESTS "Build unit tests" OFF)
option(BUILD_DOCS "Build documentation" OFF)

# Set global vars here
# set(SANDBOX_INSTALL_DOC_DIR doc/sandbox-${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})

# add_subdirectory(<sourcedir> [<binarydir>])

# External projects 
add_subdirectory(extern)

# Source directory
add_subdirectory(src bin)

# Tests 
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Docs
if(BUILD_DOCS)
    add_subdirectory(docs)
endif()

# add_dependencies(doc src)
