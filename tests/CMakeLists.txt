include(FetchContent)
FetchContent_Declare(googletest 
    GIT_REPOSITORY https://github.com/google/googletest.git 
    GIT_TAG release-1.12.1)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(TEST_SRCS
    shapes/SphereTest.cpp)

add_executable(unit_tests ${TEST_SRCS})

target_include_directories(unit_tests 
    PRIVATE
        ${GLM_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/shading)

target_link_libraries(unit_tests PRIVATE shapes gtest_main)

include(GoogleTest)
gtest_discover_tests(unit_tests)