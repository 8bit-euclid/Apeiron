include_directories(${PROJECT_SOURCE_DIR}/libs/Visualiser)

set(SOURCE_FILES
        include/Buffers.h
        include/Camera.h
        include/Geometry.h
        include/GLDebug.h
        include/GLTypes.h
        include/Light.h
        include/Material.h
        include/Mesh.h
        include/Model.h
        include/Shader.h
        include/Texture.h
        include/Visualiser.h
        include/Window.h
        resources/external/stb_image/stb_image.h
        src/Camera.cpp
        src/Geometry.cpp
        src/Light.cpp
        src/Mesh.cpp
        src/Model.cpp
        src/Shader.cpp
        src/Texture.cpp
        src/Visualiser.cpp
        src/Window.cpp
        resources/external/stb_image/stb_image.cpp)

set(LINK_LIBRARIES)

add_library(VisualiserLibrary ${SOURCE_FILES})
target_include_directories(VisualiserLibrary SYSTEM PRIVATE resources/external/stb_image)
target_link_libraries(VisualiserLibrary ${LINK_LIBRARIES} ImGui glfw)