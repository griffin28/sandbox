set (DIALOG_SRCS
    SphereDialog.cpp
    SphereDialog.h
)

INCLUDE_DIRECTORIES(
    ${Qt5Widgets_INCLUDE_DIRS}
)

add_library(dialogs OBJECT ${DIALOG_SRCS})
# set_target_properties(sandbox::dialogs PROPERTIES 
#     POSITION_INDEPENDENT_CODE true)

target_compile_options(dialogs 
    PRIVATE 
        $<$<COMPILE_LANGUAGE:CXX>:-fPIC>    
)

target_include_directories(dialogs PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
