include_directories(${PROJECT_SOURCE_DIR}/libs/Visualiser)

set(SOURCE_FILES
        include/Action.h
        include/ActionBase.h
        include/Buffers.h
        include/Camera.h
        include/Colour.h
        include/GLDebug.h
        include/GLTypes.h
        include/Glyph.h
        include/GlyphSheet.h
        include/HDR.h
        include/Light.h
        include/ParseTeX.h
        include/ParseTeX.tpp
        include/Material.h
        include/Mesh.h
        include/Model.h
        include/ModelFactory.h
        include/Scene.h
        include/FrameTexture.h
        include/Shader.h
        include/Shadow.h
        include/String.h
        include/TeXBox.h
        include/TeXBoxFactory.h
        include/Texture.h
        include/Transition.h
        include/Visualiser.h
        include/Window.h
        ../../dependencies/stb_image/stb_image.h
        include/Action.tpp
        src/ActionBase.cpp
        src/Buffers.cpp
        src/Camera.cpp
        src/Glyph.cpp
        src/HDR.cpp
        src/Light.cpp
        src/ParseTeX.cpp
        src/Mesh.cpp
        src/Model.cpp
        src/ModelFactory.cpp
        src/Scene.cpp
        src/FrameTexture.cpp
        src/Shader.cpp
        src/Shadow.cpp
        src/String.cpp
        src/TeXBox.cpp
        src/Texture.cpp
        src/Transition.cpp
        src/Visualiser.cpp
        src/Window.cpp
        ../../dependencies/stb_image/stb_image.cpp)

set(LINK_LIBRARIES
        ${OPENGL_LIBRARIES}
        ${GLEW_LIBRARIES}
        DataContainerLibrary
        FileManagerLibrary
        FunctionalLibrary
        LinearAlgebraLibrary
        glfw)

add_library(VisualiserLibrary ${SOURCE_FILES})
target_include_directories(VisualiserLibrary SYSTEM PRIVATE ../../dependencies/stb_image)
target_link_libraries(VisualiserLibrary ${LINK_LIBRARIES})