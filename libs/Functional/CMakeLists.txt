include_directories(${PROJECT_SOURCE_DIR}/libs/Functional)

#set(SOURCE_FILES
#        include/Timer.h
#        include/Benchmark.h)

set(LINK_LIBRARIES)

add_library(FunctionalLibrary INTERFACE)

target_include_directories(FunctionalLibrary INTERFACE include/)

target_link_libraries(FunctionalLibrary ${LINK_LIBRARIES})
