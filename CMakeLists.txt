#***************************************************************************************************************************************************************
# Configure Compile Settings
#***************************************************************************************************************************************************************

cmake_minimum_required(VERSION 3.16.3 FATAL_ERROR)
project(Apeiron)
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
option(OPTIMISER "Optimiser level 3" OFF)

#***************************************************************************************************************************************************************
# Setup C++ Compiler
#***************************************************************************************************************************************************************

# Set compiler standard
add_compile_options(-std=c++2a)

# Set optimisation flags
if(OPTIMISER)
    MESSAGE(STATUS "Setting optimiser flags")
    set(OPTIMISER_FLAGS "-O3 -funroll-loops")
endif()

#***************************************************************************************************************************************************************
# Setup Build and Run Directories
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
# Setup Project Sub-directories.
#***************************************************************************************************************************************************************

# Add Apeiron library directories
include_directories(${PROJECT_SOURCE_DIR}/libs)
add_subdirectory(${PROJECT_SOURCE_DIR}/libs/Array)
add_subdirectory(${PROJECT_SOURCE_DIR}/libs/Benchmark)
add_subdirectory(${PROJECT_SOURCE_DIR}/libs/Sort)

#***************************************************************************************************************************************************************
# Apeiron Binary Build Instructions.
#***************************************************************************************************************************************************************

set(APEIRON_SOURCE_FILES
        include/BasicMath.h
        include/Comparators.h
        include/Constants.h
        include/Debug.h
        include/Types.h
        include/Global.h
        include/Loops.h
        include/Macros.h
        include/Math.h
        include/Print.h
        include/Random.h
        include/StandardLibrary.h
        src/main.cpp)

set(APEIRON_LINK_LIBRARIES
        ArrayLibrary
        BenchmarkLibrary
        SortLibrary)

add_executable(${PROJECT_NAME} ${APEIRON_SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${APEIRON_LINK_LIBRARIES})

#***************************************************************************************************************************************************************
# GoogleTest Build Instructions.
#***************************************************************************************************************************************************************

# Fetch GoogleTest directories
include(FetchContent)
FetchContent_Declare(GoogleTest URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip)
FetchContent_MakeAvailable(GoogleTest)

# Add unit test executables
add_executable(UnitTestApeiron ${PROJECT_SOURCE_DIR}/test/UnitTestApeiron.cpp)
add_executable(UnitTestArray ${PROJECT_SOURCE_DIR}/libs/Array/test/UnitTestArray.cpp)

# Link with gtest, gtest_main, and associated libraries.
target_link_libraries(UnitTestApeiron gtest gtest_main)
target_link_libraries(UnitTestArray gtest gtest_main)