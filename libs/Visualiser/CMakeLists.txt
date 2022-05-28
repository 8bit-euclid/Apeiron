include_directories(${PROJECT_SOURCE_DIR}/libs/Visualiser)
#include_directories(SYSTEM ${PROJECT_SOURCE_DIR}/libs/Visualiser/resources/external/stb_image)

set(SOURCE_FILES
        include/Action.h
        include/ActionBase.h
        include/Buffers.h
        include/Camera.h
        include/Colour.h
        include/GLDebug.h
        include/GLTypes.h
        include/HDR.h
        include/Light.h
        include/Material.h
        include/Mesh.h
        include/Model.h
        include/ModelFactory.h
        include/Shader.h
        include/Shadow.h
        include/Texture.h
        include/Visualiser.h
        include/Window.h
        resources/external/stb_image/stb_image.h
        test/VisualiserTest.h
        src/Action.cpp
        src/ActionBase.cpp
        src/Buffers.cpp
        src/Camera.cpp
        src/HDR.cpp
        src/Light.cpp
        src/Mesh.cpp
        src/Model.cpp
        src/ModelFactory.cpp
        src/Shader.cpp
        src/Shadow.cpp
        src/Texture.cpp
        src/Visualiser.cpp
        src/Window.cpp
        resources/external/stb_image/stb_image.cpp
        test/VisualiserTest.cpp)

set(LINK_LIBRARIES
        DataContainerLibrary
        FunctionalLibrary
        LinearAlgebraLibrary)

add_library(VisualiserLibrary ${SOURCE_FILES})
target_include_directories(VisualiserLibrary SYSTEM PRIVATE resources/external/stb_image)
target_link_libraries(VisualiserLibrary ${LINK_LIBRARIES} ImGui glfw)