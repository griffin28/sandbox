#[=======================================================================[.rst:
FindGLM
-------

Finds the GLM library.

Imported Targets
^^^^^^^^^^^^^^^^

This module provides the following imported targets, if found:


Result Variables
^^^^^^^^^^^^^^^^

This will define the following variables:

``glm_FOUND``
  True if the system has the Qt5 library.

Cache Variables
^^^^^^^^^^^^^^^

The following cache variables may also be set:

``glm_DIR``
  The directory containing the glm config files.

#]=======================================================================]

if(NOT DEFINED glm_DIR)
    if(NOT DEFINED $ENV{glm_DIR})
        set(glm_DIR "${CMAKE_SOURCE_DIR}/extern/glm/cmake/glm" CACHE PATH "GLM install location" FORCE)
        mark_as_advanced(glm_DIR)
    else()
        file(TO_CMAKE_PATH "$ENV{glm_DIR}" _glm_DIR)
        set(glm_DIR ${_glm_DIR} CACHE PATH "GLM install location" FORCE)
        mark_as_advanced(glm_DIR)
    endif()
endif()

message(STATUS "glm DIR: ${glm_DIR}")

create_find_package("glm" "${GLM_FIND_REQUIRED}" "${GLM_FIND_QUIETLY}" "")
# if(("${GLM_FIND_REQUIRED}" STREQUAL "1") AND ("${GLM_FIND_QUIETLY}" STREQUAL "1"))
#     find_package(glm QUIET REQUIRED)
# elseif(${GLM_FIND_REQUIRED})
#     find_package(glm REQUIRED)
# elseif(${GLM_FIND_QUIETLY})
#     find_package(glm QUIET)
# endif()
