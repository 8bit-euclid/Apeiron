include_directories(${PROJECT_SOURCE_DIR}/libs/Benchmark)

#set(SOURCE_FILES
#        include/Timer.h
#        include/Benchmark.h)

set(LINK_LIBRARIES)

add_library(BenchmarkLibrary INTERFACE)

target_include_directories(BenchmarkLibrary INTERFACE include/)

target_link_libraries(BenchmarkLibrary ${LINK_LIBRARIES})
