set (SRCS
    stb_image.h
)

add_library(stb ${SRCS})

target_include_directories(stb PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
