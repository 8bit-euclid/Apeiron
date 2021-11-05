#***************************************************************************************************************************************************************
# Configure Compile Settings
#***************************************************************************************************************************************************************
cmake_minimum_required(VERSION 3.21.3 FATAL_ERROR)
project(Apeiron)
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
option(OPTIMISER "Optimiser level 3" OFF)

#***************************************************************************************************************************************************************
# Setup C++ Compiler
#***************************************************************************************************************************************************************
# Set compiler standard
set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)

# Set optimisation flags
if(OPTIMISER)
    MESSAGE(STATUS "Setting optimiser flags")
    set(OPTIMISER_FLAGS "-O3 -funroll-loops")
endif()

#***************************************************************************************************************************************************************
# Setup Build, Library, and Binary Directories
#***************************************************************************************************************************************************************
set(BUILD_DIRECTORY "${CMAKE_SOURCE_DIR}/build")
set(LIB_DIRECTORY "${BUILD_DIRECTORY}/lib")
set(BIN_DIRECTORY "${BUILD_DIRECTORY}/bin")

# Set all compiler outputs to the build directory.
set(DEFAULT_OUT_OF_SOURCE_FOLDER ${BUILD_DIRECTORY})
set(CMAKE_BUILD_FILES_DIRECTORY ${BUILD_DIRECTORY})
set(CMAKE_BUILD_DIRECTORY ${BUILD_DIRECTORY})

# Set cmake library and binary output folders
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${LIB_DIRECTORY})
set(CMAKE_LIBRARY_OUTPUT_PATH ${LIB_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIRECTORY})

#***************************************************************************************************************************************************************
# GLFW/GLEW Build Instructions for OpenGL.
#***************************************************************************************************************************************************************
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

find_package(GLEW 2.2 REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(${PROJECT_SOURCE_DIR}/dependencies/GLFW)
set(OPENGL_LINK_LIBRARIES ${OPENGL_LIBRARIES} glfw ${GLEW_LIBRARIES} GLEW::GLEW)

#***************************************************************************************************************************************************************
# Setup Project Sub-directories.
#***************************************************************************************************************************************************************
include_directories(${PROJECT_SOURCE_DIR}/libs)
add_subdirectory(${PROJECT_SOURCE_DIR}/libs/DataContainers)
add_subdirectory(${PROJECT_SOURCE_DIR}/libs/Benchmark)
add_subdirectory(${PROJECT_SOURCE_DIR}/libs/Functional)
add_subdirectory(${PROJECT_SOURCE_DIR}/libs/Sort)
add_subdirectory(${PROJECT_SOURCE_DIR}/libs/Tensor)
add_subdirectory(${PROJECT_SOURCE_DIR}/libs/Visualiser)

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
        FunctionalLibrary
        SortLibrary
        TensorLibrary
        VisualiserLibrary)

add_executable(${PROJECT_NAME} ${APEIRON_SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${APEIRON_LINK_LIBRARIES} ${OPENGL_LINK_LIBRARIES})

#***************************************************************************************************************************************************************
# GoogleTest Build Instructions.
#***************************************************************************************************************************************************************
enable_testing()

# Fetch GoogleTest directories
include(FetchContent)
FetchContent_Declare(GoogleTest URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip)
FetchContent_MakeAvailable(GoogleTest)

# Add unit test executables
add_executable(UnitTestArray ${PROJECT_SOURCE_DIR}/libs/DataContainers/test/UnitTestArray.cpp)
add_executable(UnitTestBasicMath ${PROJECT_SOURCE_DIR}/test/UnitTestBasicMath.cpp)
add_executable(UnitTestComparators ${PROJECT_SOURCE_DIR}/test/UnitTestComparators.cpp)
add_executable(UnitTestMath ${PROJECT_SOURCE_DIR}/test/UnitTestMath.cpp)

# Link with gtest, gtest_main, and associated libraries.
target_link_libraries(UnitTestArray gtest gtest_main)
target_link_libraries(UnitTestBasicMath gtest gtest_main)
target_link_libraries(UnitTestComparators gtest gtest_main)
target_link_libraries(UnitTestMath gtest gtest_main)

#add_test(UnitTestArray ${BIN_DIRECTORY}/UnitTestArray)
#add_test(UnitTestBasicMath ${BIN_DIRECTORY}/UnitTestBasicMath)
#add_test(UnitTestComparators ${BIN_DIRECTORY}/UnitTestComparators)
#add_test(UnitTestMath ${BIN_DIRECTORY}/UnitTestMath)
