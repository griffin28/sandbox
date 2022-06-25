set (DIALOG_SRCS
    SphereDialog.cpp
    SphereDialog.h)

INCLUDE_DIRECTORIES(
    ${Qt5Widgets_INCLUDE_DIRS}
)

add_library(dialogs OBJECT ${DIALOG_SRCS})

target_compile_options(dialogs PRIVATE "-fPIC")
target_include_directories(dialogs PUBLIC .)
