#pragma once

//#include "../include/Global.h"
#include "../../DataContainer/include/Array.h"
#include "Camera.h"
#include "ModelFactory.h"
#include "Light.h"
#include "Model.h"
#include "Material.h"
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

class Visualiser
{
 public:
   Visualiser();

   Visualiser(GLint window_width, GLint window_height);

   ~Visualiser() = default;

   void Add(Model& model, const std::string& name = "");

   void Add(Camera&& camera, const std::string& name = "");

   void Add(DirectionalLight&& light, const std::string& name = "");

   void Add(PointLight&& light, const std::string& name = "");

   void Add(SpotLight&& light, const std::string& name = "");

   void Render();

 private:
   void Init();

   void Add(std::shared_ptr<Model> model, const std::string& name);

   void AddTextures();

   void BeginFrame();

   void EndFrame();

   void UpdateModels();

   void ManageUserInputs();

   void UpdateViewFrustum();

   void RenderModels(const std::string& _shader_name);

   void RenderDirectionalShadows();

   void RenderPointShadows();

   void RenderFullScene();

   template<class type> using Map = std::unordered_map<std::string, type>;
   Window                          OpenGLWindow;
   Map<std::shared_ptr<Model>>     Models;
   Map<Shader>                     Shaders;
   Map<Camera>                     Cameras;
   Map<Map<Texture>>               Textures;
   Map<DirectionalLight>           DirectionalLights;
   Map<PointLight>                 PointLights;
   Map<SpotLight>                  SpotLights;
   std::reference_wrapper<Camera>  ActiveCamera;
   bool                            isViewPortModified{false};
};

}
