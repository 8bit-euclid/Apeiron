#pragma once

#include "../include/Global.h"
#include "../../DataContainer/include/Array.h"
#include "../../DataContainer/include/List.h"
#include "Camera.h"
#include "Geometry.h"
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

namespace Apeiron {

class Visualiser
{
public:
  Visualiser() : Visualiser(1920, 1080) {}

  Visualiser(GLint _window_width, GLint _window_height)
    : OpenGLWindow(_window_width, _window_height), MainLight(glm::vec3(0.0, -1.0, -1.0), glm::vec4(1.0, 1.0, 1.0, 1.0), 0.3, 0.6) {}

  ~Visualiser() = default;

  inline GLFWwindow*
  GetGLFWWindow() { return OpenGLWindow.pWindow; }

  inline bool
  isWindowOpen() { return OpenGLWindow.isOpen(); }

  const StaticArray<Bool, 1024>&
  GetKeys() const { return OpenGLWindow.GetKeys(); }

  SVectorF2
  GetMouseDisplacement() { return OpenGLWindow.GetMouseDisplacement(); }

  SVectorF2
  GetMouseWheelDisplacement() { return OpenGLWindow.GetMouseWheelDisplacement(); }

  inline std::pair<GLint, GLint>
  ViewportDimensions() { return {OpenGLWindow.ViewportDimensions[0], OpenGLWindow.ViewportDimensions[1]}; }

  inline GLfloat
  ViewportAspectRatio() { return OpenGLWindow.GetViewportAspectRatio(); }

  void BeginFrame();

  GLfloat GetDeltaTime() { return OpenGLWindow.GetDeltaTime(); }

  void RenderScene();

  void RenderDirectionalShadows();

  void RenderPointShadows();

  void RenderModels(UInt _shader_index);

  void EndFrame();

private:
  Window OpenGLWindow;

public:
  bool isViewPortModified{false};

  List<Model> Models;
  List<Shader> Shaders;
  List<Material> Materials;
  List<Texture> Textures;

  DirectionalLight MainLight;
  List<PointLight> PointLights;
  List<SpotLight> SpotLights;
  List<Camera> Cameras;
};

}
