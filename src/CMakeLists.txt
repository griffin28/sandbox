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
    resources/resources.qrc
)

# add_subdirectory(<sourcedir> [<binarydir>])
# Shading Models
add_subdirectory(shading)

# Shapes
add_subdirectory(shapes)

# Dialogs
add_subdirectory(dialogs)

add_executable(${CMAKE_PROJECT_NAME} ${SRCS})
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE 
    stb shapes dialogs shading Qt5::Core Qt5::Gui Qt5::Widgets 
    Qt5::OpenGL Qt5::Network Qt5::PrintSupport Qt5::Qml Qt5::Svg 
    Qt5::Xml Qt5::UiTools Qt5::Concurrent Qt5::X11Extras glm::glm
    warning_props)

# Generate MD5 hash of the executable
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
                   COMMAND md5sum
                   ARGS -b "$<TARGET_FILE:${CMAKE_PROJECT_NAME}>" > "${CMAKE_PROJECT_NAME}.md5")
