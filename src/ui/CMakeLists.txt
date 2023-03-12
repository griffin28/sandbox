set (UI_SRCS
    # Rasterization
    rasterization/AbstractGLScene.cpp
    rasterization/AbstractGLScene.h
    rasterization/GLCanvas.cpp
    rasterization/GLCanvas.h
    rasterization/RasterizationScene.cpp
    rasterization/RasterizationScene.h
    rasterization/RasterizationWidget.cpp
    rasterization/RasterizationWidget.h
    # TODO: Path Tracing
    HUD.cpp
    HUD.h
    MainWindow.cpp
    MainWindow.h)

add_library(ui OBJECT ${UI_SRCS})

target_compile_options(ui
    PRIVATE
        $<$<COMPILE_LANGUAGE:CXX>:-fPIC>)

target_include_directories(ui
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/rasterization>
    PRIVATE
        ${Qt5Widgets_INCLUDE_DIRS}
        ${GLM_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/../cameras
        ${CMAKE_CURRENT_SOURCE_DIR}/../shapes
        ${CMAKE_CURRENT_SOURCE_DIR}/../shading
        ${CMAKE_CURRENT_SOURCE_DIR}/../dialogs)
