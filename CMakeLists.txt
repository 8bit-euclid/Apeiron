#***************************************************************************************************************************************************************
# Configure Compile Settings
#***************************************************************************************************************************************************************

cmake_minimum_required(VERSION 3.16.3)
project(Apeiron)
option(OPTIMISER "Optimiser level 3" OFF)

#***************************************************************************************************************************************************************
# Setup Compiler
#***************************************************************************************************************************************************************

# Set compiler standard
add_compile_options(-std=c++2a)

# Set optimisation flags
if(OPTIMISER)
    MESSAGE(STATUS "Setting optimiser flags")
    set(OPTIMISER_FLAGS "-O3 -funroll-loops")
endif()

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
# Project Sub-directories.
#***************************************************************************************************************************************************************

# Add Apeiron library directories
include_directories(${PROJECT_SOURCE_DIR}/libs)
add_subdirectory(${PROJECT_SOURCE_DIR}/libs/Array)

#***************************************************************************************************************************************************************
# Apeiron Build Recipe.
#***************************************************************************************************************************************************************

set(APEIRON_SOURCE_FILES
        include/Comparators.h
        include/Debug.h
        include/FloatingPoint.h
        include/Global.h
        include/Loops.h
        include/Macros.h
        include/Math.h
        include/Print.h
        include/StandardLibrary.h
        src/main.cpp)

set(APEIRON_LINK_LIBRARIES
        Array_Library)

add_executable(${PROJECT_NAME} ${APEIRON_SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${APEIRON_LINK_LIBRARIES})