#pragma once

#include "../include/Global.h"
#include "../../DataContainers/include/Array.h"
#include "GLDebug.h"
#include "GLTypes.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace Apeiron {

class Window
{
  constexpr static GLint mKeys = 1024;

public:
  Window() = default;

  Window(GLint _width, GLint _height) : WindowDimensions{_width, _height} { Open(_width, _height); };

  ~Window() { glfwTerminate(); };

  void Open(GLint _width, GLint _height);

  inline bool isOpen() const { return !glfwWindowShouldClose(pWindow); }

  inline void Close() { glfwSetWindowShouldClose(pWindow, GL_TRUE); }

  inline std::pair<GLint, GLint> GetViewportDimensions() const { return {ViewportDimensions[0], ViewportDimensions[1]}; }

  inline GLfloat GetViewportAspectRatio() const { return static_cast<GLfloat>(ViewportDimensions[0]) / static_cast<GLfloat>(ViewportDimensions[1]); }

  const StaticArray<Bool, mKeys>& GetKeys() const { return Keys; }

  StaticArray<GLfloat, 2> GetMouseDisplacement();

  bool isViewPortModified();

  void SwapBuffers();

  void ComputeDeltaTime();

  GLfloat GetDeltaTime() { return DeltaTime; }

private:
  GLFWwindow* pWindow;

  GLfloat DeltaTime{0.0};
  GLfloat LastTime{0.0};

  StaticArray<Bool, mKeys> Keys;
  StaticArray<GLint, 2> WindowDimensions;
  StaticArray<GLint, 2> ViewportDimensions;
  StaticArray<GLfloat, 2> PreviousMousePosition;
  StaticArray<GLfloat, 2> MouseDisplacement;
  bool isFirstMouseMovement;

  void CreateCallBacks();

  static void HandleKeys(GLFWwindow* _p_window, GLint _key, GLint _code, GLint _action, GLint _mode);

  static void HandleMouse(GLFWwindow* _p_window, GLdouble _x_coord, GLdouble _y_coord);

  std::pair<GLint, GLint> GetFrameBufferSize() const;
};

}
