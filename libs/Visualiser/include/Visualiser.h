#pragma once

#include "../include/Global.h"
#include "../../DataContainers/include/Array.h"
#include "../../DataContainers/include/List.h"
#include "Camera.h"
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
  bool isViewPortModified{false};
  List<Model> Models;
  List<Material> Materials;
  List<Texture> Textures;
  List<DirectionalLight> DirectionalLights;
  List<PointLight> PointLights;
  List<SpotLight> SpotLights;
  List<Camera> Cameras;
  List<Shader> Shaders;

  Visualiser() = default;

  ~Visualiser() = default;

  inline void OpenWindow(GLint _window_width, GLint _window_height) { OpenGLWindow.Open(_window_width, _window_height); };

  inline void CloseWindow() { OpenGLWindow.Close(); }

  inline bool isWindowOpen() { return OpenGLWindow.isOpen(); }

  const StaticArray<Bool, 1024>& GetKeys() const { return OpenGLWindow.GetKeys(); }

  StaticArray<GLdouble, 2> GetMouseDisplacement() { return OpenGLWindow.GetMouseDisplacement(); }

  inline std::pair<GLint, GLint> ViewportDimensions() { return OpenGLWindow.GetViewportDimensions(); }

  inline GLfloat ViewportAspectRatio() { return OpenGLWindow.GetViewportAspectRatio(); }

  void BeginFrame();

  GLfloat GetDeltaTime() { return OpenGLWindow.GetDeltaTime(); }

  void EndFrame();

private:
  Window OpenGLWindow;
};

}
