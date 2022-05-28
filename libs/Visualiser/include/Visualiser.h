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

   Visualiser(GLint _window_width, GLint _window_height);

   ~Visualiser() = default;

   void Add(Model& _model, const std::string& _name = "");

   void Add(Camera&& _camera, const std::string& _name = "");

   void Add(DirectionalLight&& _light, const std::string& _name = "");

   void Add(PointLight&& _light, const std::string& _name = "");

   void Add(SpotLight&& _light, const std::string& _name = "");

   void Render();

 private:
   void Init();

   void Add(std::shared_ptr<Model> _model, const std::string& _model_name);

   void AddTextures();

   void AddMaterials();

   void BeginFrame();

   void UpdateModels();

   void ManageUserInputs();

   void UpdateViewFrustum();

   void RenderDirectionalShadows();

   void RenderPointShadows();

   void RenderFullScene();

   void RenderModels(const std::string& _shader_name);

   void EndFrame();

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
