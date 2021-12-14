include_directories(${PROJECT_SOURCE_DIR}/libs/DataContainers)

set(SOURCE_FILES
        include/Array.h
        include/List.h
        include/MultiArray.h
        include/Support.h
        src/Array.cpp
        src/MultiArray.cpp)

set(LINK_LIBRARIES)

add_library(ArrayLibrary ${SOURCE_FILES})
target_link_libraries(ArrayLibrary ${LINK_LIBRARIES})
