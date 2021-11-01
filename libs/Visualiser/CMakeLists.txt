include_directories(${PROJECT_SOURCE_DIR}/libs/Visualiser)

set(SOURCE_FILES
        include/Renderer.h
        src/Renderer.cpp)

set(LINK_LIBRARIES)

add_library(VisualiserLibrary ${SOURCE_FILES})
target_link_libraries(VisualiserLibrary ${LINK_LIBRARIES})