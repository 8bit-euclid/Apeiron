include_directories(${PROJECT_SOURCE_DIR}/libs/DataContainers)

set(SOURCE_FILES
        include/Array.h
        include/MultiArray.h
        src/Array.cpp include/Support.h)

set(LINK_LIBRARIES)

add_library(ArrayLibrary ${SOURCE_FILES})
target_link_libraries(ArrayLibrary ${LINK_LIBRARIES})
