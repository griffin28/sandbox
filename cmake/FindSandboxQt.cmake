#[=======================================================================[.rst:
FindSandboxQt
-------------

Finds the Qt5 module libraries.

Imported Targets
^^^^^^^^^^^^^^^^

This module provides the following imported targets, if found:

``Qt5::Core``
  Qt core functionalities
``Qt5::Gui`` 
``Qt5::Widgets`` 
``Qt5::OpenGL`` 
``Qt5::Network`` 
``Qt5::PrintSupport`` 
``Qt5::Qml`` 
``Qt5::Svg`` 
``Qt5::Xml`` 
``Qt5::UiTools`` 
``Qt5::Concurrent`` 
``Qt5::X11Extras``

Result Variables
^^^^^^^^^^^^^^^^

This will define the following variables:

``SandboxQt_FOUND``
  True if the system has the Qt5 library.

Cache Variables
^^^^^^^^^^^^^^^

The following cache variables may also be set:

``Qt5_DIR``
  The directory containing the Qt5 config files.

#]=======================================================================]

if(NOT DEFINED Qt5_DIR)
    file(TO_CMAKE_PATH "$ENV{Qt5_DIR}" _Qt5_DIR)
    set(Qt5_DIR ${_Qt5_DIR} CACHE PATH "Install location of Qt5" FORCE)
    mark_as_advanced(Qt5_DIR)
endif()

message(STATUS "Qt5_DIR: ${Qt5_DIR}")

# set(Qt5_DIR "/home/kgriffin/VisIt/third_party/3.1.4/qt/5.10.1/linux-x86_64_gcc-9.3/lib/cmake/Qt5"

set(Qt_MODULES 
    Core
    Gui
    Widgets 
    OpenGL 
    Network 
    PrintSupport 
    Qml 
    Svg 
    Xml 
    UiTools 
    Concurrent 
    X11Extras
)

create_find_package("Qt5" "${SandboxQt_FIND_REQUIRED}" "${SandboxQt_FIND_QUIETLY}" "${Qt_MODULES}")

