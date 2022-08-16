#pragma once

#include "../../../include/Global.h"
#include "DataContainer/include/Array.h"
#include "FileManager/include/FileSystem.h"

#include "Camera.h"
#include "GlyphSheet.h"
#include "ModelFactory.h"
#include "Light.h"
#include "Model.h"
#include "Material.h"
#include "Scene.h"
#include "Shader.h"
#include "Window.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <map>

namespace aprn::vis {

/***************************************************************************************************************************************************************
* Visualiser Class
***************************************************************************************************************************************************************/
class Visualiser
{
 public:
   Visualiser();

   Visualiser(GLint window_width, GLint window_height);

   void Add(Scene& scene, const std::string& name = "");

   void Add(Camera&& camera, const std::string& name = "");

   void Render();

 private:
   void Init();

   void InitScenes();

   void InitTeXBoxes();

   void InitTextures();

   void StartFrame();

   void EndFrame();

   void UpdateScene();

   void ManageUserInputs();

   void UpdateViewFrustum();

   void RenderScene();

   template<class type> using UMap = std::unordered_map<std::string, type>;
   Window              _OpenGLWindow;
   UMap<Scene>         _Scenes;
   UMap<Camera>        _Cameras;
   UMap<Shader>        _Shaders;
   UMap<UMap<Texture>> _Textures;
   Camera*             _ActiveCamera;
   Scene*              _CurrentScene;
   bool                _isViewPortModified{};
};

}
