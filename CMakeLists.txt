#***************************************************************************************************************************************************************
# Configure Compile Settings
#***************************************************************************************************************************************************************
cmake_minimum_required(VERSION 3.21.3 FATAL_ERROR)

project(Apeiron)
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set(CMAKE_BUILD_TYPE Debug)

# Set compiler standard
set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)

# Set default, debug, and release flags
set(CMAKE_CXX_FLAGS "-Wall -DNDEBUG")
#set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -Wextra -pedantic -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-include-dirs -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wundef -Wno-unused -Wno-variadic-macros -Wno-parentheses -fdiagnostics-show-option")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -funroll-loops")

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
# Build Instructions for OpenGL Tools.
#***************************************************************************************************************************************************************
find_package(OpenGL REQUIRED)
find_package(GLEW 2.2 REQUIRED)
find_package(glm REQUIRED)
#find_package(imgui REQUIRED)

#find_path(STB_INCLUDE_DIRS "stb.h")

include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_DIRS})
include_directories(${GLM_INCLUDE_DIRS})

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(${PROJECT_SOURCE_DIR}/thirdparty/GLFW)
add_subdirectory(${PROJECT_SOURCE_DIR}/thirdparty/glm)

# Compile ImGui as static library
set(IMGUI_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/thirdparty/imgui)
file(GLOB IMGUI_SOURCE_FILES ${IMGUI_INCLUDE_DIRS}/*.cpp ${IMGUI_INCLUDE_DIRS}/backends/imgui_impl_glfw.cpp ${IMGUI_INCLUDE_DIRS}/backends/imgui_impl_opengl3.cpp)
add_library(ImGui STATIC ${IMGUI_SOURCE_FILES})
target_include_directories(ImGui SYSTEM PUBLIC ${IMGUI_INCLUDE_DIRS})
target_link_libraries(ImGui glfw)

#***************************************************************************************************************************************************************
# Setup Project Sub-directories.
#***************************************************************************************************************************************************************
include_directories(${PROJECT_SOURCE_DIR}/libs)
add_subdirectory(${PROJECT_SOURCE_DIR}/libs/DataContainer)
add_subdirectory(${PROJECT_SOURCE_DIR}/libs/Benchmark)
add_subdirectory(${PROJECT_SOURCE_DIR}/libs/Functional)
add_subdirectory(${PROJECT_SOURCE_DIR}/libs/LinearAlgebra)
add_subdirectory(${PROJECT_SOURCE_DIR}/libs/Manifold)
add_subdirectory(${PROJECT_SOURCE_DIR}/libs/Polytope)
add_subdirectory(${PROJECT_SOURCE_DIR}/libs/Sort)
add_subdirectory(${PROJECT_SOURCE_DIR}/libs/Tensor)
add_subdirectory(${PROJECT_SOURCE_DIR}/libs/Visualiser)

#***************************************************************************************************************************************************************
# Apeiron Binary Build Instructions.
#***************************************************************************************************************************************************************
set(APEIRON_SOURCE_FILES
        src/main.cpp)

set(APEIRON_LINK_LIBRARIES
        DataContainerLibrary
        BenchmarkLibrary
        FunctionalLibrary
        LinearAlgebraLibrary
        ManifoldLibrary
        PolytopeLibrary
        SortLibrary
        TensorLibrary
        VisualiserLibrary)

add_executable(${PROJECT_NAME} ${APEIRON_SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${APEIRON_LINK_LIBRARIES} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} glfw glm ImGui)

#***************************************************************************************************************************************************************
# GoogleTest Build Instructions.
#***************************************************************************************************************************************************************
enable_testing()

# Fetch GoogleTest directories
include(FetchContent)
FetchContent_Declare(GoogleTest URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip)
FetchContent_MakeAvailable(GoogleTest)

# Add unit test executables
add_executable(UnitTestBasicMath ${PROJECT_SOURCE_DIR}/test/UnitTestBasicMath.cpp)
add_executable(UnitTestComparators ${PROJECT_SOURCE_DIR}/test/UnitTestComparators.cpp)
add_executable(UnitTestMath ${PROJECT_SOURCE_DIR}/test/UnitTestMath.cpp)

add_executable(UnitTestArray ${PROJECT_SOURCE_DIR}/libs/DataContainer/test/UnitTestArray.cpp)
add_executable(UnitTestNumericContainer ${PROJECT_SOURCE_DIR}/libs/DataContainer/test/UnitTestNumericContainer.cpp)
add_executable(UnitTestVector ${PROJECT_SOURCE_DIR}/libs/LinearAlgebra/test/UnitTestVector.cpp)

# Link with gtest, gtest_main, and associated libraries.
target_link_libraries(UnitTestArray gtest gtest_main)
target_link_libraries(UnitTestBasicMath gtest gtest_main)
target_link_libraries(UnitTestComparators gtest gtest_main)
target_link_libraries(UnitTestMath gtest gtest_main)
target_link_libraries(UnitTestNumericContainer gtest gtest_main)
target_link_libraries(UnitTestVector gtest gtest_main)

#add_test(UnitTestArray ${BIN_DIRECTORY}/UnitTestArray)
#add_test(UnitTestBasicMath ${BIN_DIRECTORY}/UnitTestBasicMath)
#add_test(UnitTestComparators ${BIN_DIRECTORY}/UnitTestComparators)
#add_test(UnitTestMath ${BIN_DIRECTORY}/UnitTestMath)
