set (UI_SRCS
    AbstractGLScene.cpp
    AbstractGLScene.h
    Canvas.cpp
    Canvas.h
    HUD.cpp 
    HUD.h
    MainWidget.cpp 
    MainWidget.h
    MainWindow.cpp 
    MainWindow.h
    SimpleScene.cpp
    SimpleScene.h)

add_library(ui OBJECT ${UI_SRCS})

target_compile_options(ui 
    PRIVATE 
        $<$<COMPILE_LANGUAGE:CXX>:-fPIC>)

target_include_directories(ui 
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    PRIVATE
        ${Qt5Widgets_INCLUDE_DIRS}
        ${GLM_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/../shapes
        ${CMAKE_CURRENT_SOURCE_DIR}/../shading
        ${CMAKE_CURRENT_SOURCE_DIR}/../dialogs)
