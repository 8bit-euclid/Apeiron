include_directories(${PROJECT_SOURCE_DIR}/libs/LinearAlgebra)

set(SOURCE_FILES
        include/Matrix.h
        include/Vector.h
        src/Matrix.cpp
        src/Vector.cpp)

set(LINK_LIBRARIES
        DataContainerLibrary)

add_library(LinearAlgebraLibrary ${SOURCE_FILES})
target_link_libraries(LinearAlgebraLibrary ${LINK_LIBRARIES})
