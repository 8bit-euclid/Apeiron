include_directories(${PROJECT_SOURCE_DIR}/libs/Manifold)

set(SOURCE_FILES
        include/Categories.h
        include/Curve.h
        include/Manifold.h
        include/Surface.h
        src/Curve.cpp
        src/Manifold.cpp
        src/Surface.cpp)

set(LINK_LIBRARIES
        DataContainerLibrary
        LinearAlgebraLibrary)

add_library(ManifoldLibrary ${SOURCE_FILES})
target_link_libraries(ManifoldLibrary ${LINK_LIBRARIES})
