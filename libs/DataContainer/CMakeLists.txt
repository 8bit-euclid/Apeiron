include_directories(${PROJECT_SOURCE_DIR}/libs/DataContainer)

set(SOURCE_FILES
        include/Array.h
        include/List.h
        include/MultiArray.h
        include/Detail.h
        src/Array.cpp
        src/MultiArray.cpp)

set(LINK_LIBRARIES)

add_library(DataContainerLibrary ${SOURCE_FILES})
target_link_libraries(DataContainerLibrary ${LINK_LIBRARIES})
