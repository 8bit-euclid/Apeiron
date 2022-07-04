#pragma once

#include "../include/Global.h"
#include "../../DataContainer/include/Array.h"
#include "../../LinearAlgebra/include/Vector.h"
#include "GLDebug.h"
#include "GLTypes.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace aprn::vis {

constexpr static GLint mKeys{1024};

class Window
{
 public:
   Window(GLint width, GLint height);

   ~Window();

   void Open(GLint _width, GLint _height);

   bool isOpen() const;

   void Close();

   bool isViewPortModified();

   void ResetViewPort() const;

   void SwapBuffers();

   void ResetTime() const;

   void ComputeDeltaTime();

   Float GetDeltaTime() const;

   Float GetCurrentTime() const;

   GLfloat
   ComputeViewportAspectRatio() const;

   SVectorF2
   GetMouseDisplacement();

   SVectorF2
   GetMouseWheelDisplacement();

 private:
   friend class Visualiser;

   std::pair<GLint, GLint>
   GetFrameBufferSize() const;

   void CreateCallBacks() const;

   static void
   HandleKeys(GLFWwindow* _p_window, const GLint _key, const GLint _code, const GLint _action, const GLint _mode);

   static void
   HandleMousePosition(GLFWwindow* _p_window, const GLdouble _x_coord, const GLdouble _y_coord);

   static void
   HandleMouseWheel(GLFWwindow* _p_window, const GLdouble _x_offset, const GLdouble _y_offset);

   static void APIENTRY
   glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam);

   Float           CurrentTime{};
   Float           PreviousTime{};
   Float           DeltaTime{};
   SVectorF2       PreviousMousePosition;
   SVectorF2       MouseDisplacement;
   SVectorF2       MouseWheelDisplacement;
   SVector2<GLint> WindowDimensions;
   SVector2<GLint> ViewportDimensions;
   SArrayB<mKeys>  Keys;
   GLFWwindow*     GlfwWindow;
   bool            isFirstMouseMovement;
};

}
