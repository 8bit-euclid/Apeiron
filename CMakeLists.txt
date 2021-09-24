cmake_minimum_required(VERSION 3.16.3)

project(Apeiron)

set(APEIRON_SOURCE_FILES
        ${PROJECT_SOURCE_DIR}/include/Global.h
        ${PROJECT_SOURCE_DIR}/include/Standard_Library.h
        ${PROJECT_SOURCE_DIR}/src/main.cpp)

set(APEIRON_LINK_LIBRARIES
        Array_Library)

#***************************************************************************************************************************************************************
# Build and Run Directory Setup
#***************************************************************************************************************************************************************
set(BUILD_DIRECTORY "${CMAKE_SOURCE_DIR}/build/")

# Set all compiler outputs to the build directory.
set(DEFAULT_OUT_OF_SOURCE_FOLDER ${BUILD_DIRECTORY})
set(CMAKE_BUILD_FILES_DIRECTORY ${BUILD_DIRECTORY})
set(CMAKE_BUILD_DIRECTORY ${BUILD_DIRECTORY})

# Set cmake library and binary output folders
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BUILD_DIRECTORY}/lib)
set(CMAKE_LIBRARY_OUTPUT_PATH ${BUILD_DIRECTORY}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BUILD_DIRECTORY}/bin)

#***************************************************************************************************************************************************************
# Block 1.3: Project sub-directories.
#***************************************************************************************************************************************************************

## Add all third-party directories
#include_directories(${PROJECT_SOURCE_DIR}/libs/ThirdParty)
#add_subdirectory(${PROJECT_SOURCE_DIR}/libs/ThirdParty/Eigen)
#add_subdirectory(${PROJECT_SOURCE_DIR}/libs/ThirdParty/GoogleTest)

# Add all Apeiron library directories
include_directories(${PROJECT_SOURCE_DIR}/libs)
add_subdirectory(${PROJECT_SOURCE_DIR}/libs/Array)

add_executable(${PROJECT_NAME} ${APEIRON_SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${APEIRON_LINK_LIBRARIES})