set (CORE_SRCS
    Ray.h)

INCLUDE_DIRECTORIES(
    ${GLM_INCLUDE_DIRS}
)

add_library(core OBJECT ${CORE_SRCS})

target_include_directories(core PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
