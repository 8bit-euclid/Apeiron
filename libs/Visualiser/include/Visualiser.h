#pragma once

//#include "../include/Global.h"
#include "../../DataContainer/include/Array.h"
#include "Camera.h"
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
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

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

   void LoadTextures();

   void InitScenes();

   void StartFrame();

   void EndFrame();

   void UpdateScene();

   void ManageUserInputs();

   void UpdateViewFrustum();

   void RenderScene();

   template<class type> using Map = std::unordered_map<std::string, type>;

   Map<Scene>        Scenes;
   Map<Camera>       Cameras;
   Map<Shader>       Shaders;
   Map<Map<Texture>> Textures;
   Camera*           ActiveCamera;
   Scene*            CurrentScene;
   Window            OpenGLWindow;
   bool              wasViewPortModified;
};

}
