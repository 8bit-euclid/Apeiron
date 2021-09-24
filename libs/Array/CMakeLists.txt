include_directories(${PROJECT_SOURCE_DIR}/libs/Array)

set(SOURCE_FILES
        include/Array.h
        src/Array.cpp)

set(LINK_LIBRARIES)


add_library(Array_Library ${SOURCE_FILES})
target_link_libraries(Array_Library ${LINK_LIBRARIES})
