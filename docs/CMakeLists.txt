find_package(Doxygen)

if(NOT DOXYGEN_FOUND)
    add_custom_target(doxygen COMMAND false
        COMMENT "Doxygen not found")
    return()
endif()

file(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/docs/html")
set(DOXYGEN_GENERATE_HTML YES)
set(DOXYGEN_HTML_OUTPUT "${PROJECT_BINARY_DIR}/docs/html")    

# Add Doxygen Awesome Style
include(FetchContent)
FetchContent_Declare(doxygen-awesome-css 
    GIT_REPOSITORY https://github.com/jothepro/doxygen-awesome-css.git 
    GIT_TAG v2.0.3)

FetchContent_MakeAvailable(doxygen-awesome-css)
set(DOXYGEN_GENERATE_TREEVIEW   YES)
set(DOXYGEN_HAVE_DOT            YES)
set(DOXYGEN_DOT_IMAGE_FORMAT    svg)
set(DOXYGEN_DOT_TRANSPARENT     YES)
set(DOXYGEN_HTML_EXTRA_STYLESHEET
    ${doxygen-awesome-css_SOURCE_DIR}/doxygen-awesome.css)

# Create the doxygen target
doxygen_add_docs(doxygen 
    ${PROJECT_SOURCE_DIR}/src
    COMMENT "Generate HTML documentation")
