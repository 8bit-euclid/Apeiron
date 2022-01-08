include_directories(${PROJECT_SOURCE_DIR}/libs/Polytope)

set(SOURCE_FILES
        include/Polytope.h
        include/Categories.h
        src/Polytope.cpp)

set(LINK_LIBRARIES)

add_library(PolytopeLibrary ${SOURCE_FILES})
target_link_libraries(PolytopeLibrary ${LINK_LIBRARIES})
