include_directories(${PROJECT_SOURCE_DIR}/libs/Visualiser)

set(SOURCE_FILES
        include/Buffers.h
        include/GLDebug.h
        include/GLTypes.h
        include/Renderer.h
        include/Shader.h
        include/Texture.h
        resources/external/stb_image/stb_image.h
        src/Shader.cpp
        src/Texture.cpp
        resources/external/stb_image/stb_image.cpp)

set(LINK_LIBRARIES)

add_library(VisualiserLibrary ${SOURCE_FILES})
target_link_libraries(VisualiserLibrary ${LINK_LIBRARIES})