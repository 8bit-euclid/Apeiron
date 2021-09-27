include_directories(${PROJECT_SOURCE_DIR}/libs/Array)

set(SOURCE_FILES
        include/Array.h
        src/Array.cpp)

set(LINK_LIBRARIES)

add_library(ArrayLibrary ${SOURCE_FILES})
target_link_libraries(ArrayLibrary ${LINK_LIBRARIES})
