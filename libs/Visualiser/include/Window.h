#pragma once

#include "../include/Global.h"
#include "../../DataContainer/include/Array.h"
#include "GLDebug.h"
#include "GLTypes.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace Apeiron {

class Window
{
  friend class Visualiser;
  constexpr static GLint mKeys{1024};

public:
  Window(GLint _width, GLint _height);

  ~Window() { glfwTerminate(); };

  void Open(GLint _width, GLint _height);

  inline bool isOpen() const { return !glfwWindowShouldClose(pWindow); }

  inline void Close() { glfwSetWindowShouldClose(pWindow, GL_TRUE); }

  inline std::pair<GLint, GLint> GetViewportDimensions() const { return {ViewportDimensions[0], ViewportDimensions[1]}; }

  inline GLfloat GetViewportAspectRatio() const { return static_cast<GLfloat>(ViewportDimensions[0]) / static_cast<GLfloat>(ViewportDimensions[1]); }

  const StaticArray<Bool, mKeys>& GetKeys() const { return Keys; }

  StaticArray<GLdouble, 2> GetMouseDisplacement();

  StaticArray<GLdouble, 2> GetMouseWheelDisplacement();

  bool isViewPortModified();

  void ResetViewPort();

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
  StaticArray<GLdouble, 2> PreviousMousePosition;
  StaticArray<GLdouble, 2> MouseDisplacement;
  StaticArray<GLdouble, 2> MouseWheelDisplacement;
  bool isFirstMouseMovement;

  std::pair<GLint, GLint> GetFrameBufferSize() const;

  void CreateCallBacks();

  static void HandleKeys(GLFWwindow* _p_window, const GLint _key, const GLint _code, const GLint _action, const GLint _mode);

  static void HandleMousePosition(GLFWwindow* _p_window, const GLdouble _x_coord, const GLdouble _y_coord);

  static void HandleMouseWheel(GLFWwindow* _p_window, const GLdouble _x_offset, const GLdouble _y_offset);

  static void APIENTRY glDebugOutput(GLenum _source, GLenum _type, unsigned int id, GLenum _severity, GLsizei length, const char* message, const void* userParam);
};

}
