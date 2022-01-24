include_directories(${PROJECT_SOURCE_DIR}/libs/Manifold)

set(SOURCE_FILES
        include/Curve.h
        include/Surface.h
        src/Curve.cpp
        src/Surface.cpp)

set(LINK_LIBRARIES
        LinearAlgebraLibrary)

add_library(ManifoldLibrary ${SOURCE_FILES})
target_link_libraries(ManifoldLibrary ${LINK_LIBRARIES})
