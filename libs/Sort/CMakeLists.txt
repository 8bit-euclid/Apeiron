include_directories(${PROJECT_SOURCE_DIR}/libs/DataContainers)

set(SOURCE_FILES
        include/Sort.h
        src/Sort.cpp)

set(LINK_LIBRARIES)

add_library(SortLibrary ${SOURCE_FILES})
target_link_libraries(SortLibrary ${LINK_LIBRARIES})
