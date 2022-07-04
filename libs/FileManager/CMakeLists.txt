include_directories(${PROJECT_SOURCE_DIR}/libs/FileManager)

set(SOURCE_FILES
        include/FileIO.h
        include/FileHandler.h
        include/FileReader.h
        include/FileWriter.h
        src/FileHandler.cpp
        src/FileReader.cpp
        src/FileWriter.cpp)

set(LINK_LIBRARIES)

add_library(FileManagerLibrary ${SOURCE_FILES})
target_link_libraries(FileManagerLibrary ${LINK_LIBRARIES})
