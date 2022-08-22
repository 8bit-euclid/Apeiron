/***************************************************************************************************************************************************************
* GPL-3.0 License
* Copyright (C) 2022 Niran A. Ilangakoon
*
* This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as
* published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
* of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program.
* If not, see <https://www.gnu.org/licenses/>.
***************************************************************************************************************************************************************/

#include "../../../include/Global.h"
#include "../include/Window.h"
#include "LinearAlgebra/include/Vector.h"

#define GL_DEBUG_MODE

namespace aprn::vis {

/***************************************************************************************************************************************************************
* Public Interface
***************************************************************************************************************************************************************/
Window::Window(GLint width, GLint height)
   : WindowDimensions{width, height} { Open(width, height); }

Window::~Window() { glfwTerminate(); }

void
Window::Open(GLint width, GLint height)
{
   if(!glfwInit())
   {
     glfwTerminate();
     EXIT("Failed to Initialise GLFW.")
   }

   // Window properties
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Enforces backward incompatibility
   glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#ifdef GL_DEBUG_MODE
   glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif

   // Framebuffer properties
//   glfwWindowHint(GLFW_RED_BITS, 8);
//   glfwWindowHint(GLFW_GREEN_BITS, 8);
//   glfwWindowHint(GLFW_BLUE_BITS, 8);
//   glfwWindowHint(GLFW_ALPHA_BITS, 8);

   // Anti-aliasing properties
   glfwWindowHint(GLFW_SAMPLES, 24);
   GLCall(glEnable(GL_MULTISAMPLE));

   // Create a window and its OpenGL context.
   WindowDimensions = {width, height};
   GlfwWindow = glfwCreateWindow(width, height, "Apeiron Visualiser", nullptr, nullptr);
   if(!GlfwWindow)
   {
     glfwTerminate();
     EXIT("Could not create an OpenGL window.")
   }

   // Set context for GLEW to use
   glfwMakeContextCurrent(GlfwWindow);
   glfwSwapInterval(1);

   // Set viewport dimensions
   std::tie(ViewportDimensions[0], ViewportDimensions[1]) = GetFrameBufferSize();

   // Handle key mouse inputs
   CreateCallBacks();
//   glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
   glfwSetInputMode(GlfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

   // Allow modern extension features
   glewExperimental = GL_TRUE;

   // Initialise GLEW
   if(glewInit() != GLEW_OK)
   {
     glfwDestroyWindow(GlfwWindow);
     glfwTerminate();
     EXIT("Failed to Initialise GLEW.")
   }
//   else Print("\nRunning OpenGL Version:", glGetString(GL_VERSION));

   // Initialise OpenGL debug output
#ifdef GL_DEBUG_MODE
   int flags;
   GLCall(glGetIntegerv(GL_CONTEXT_FLAGS, &flags));
   if(flags & GL_CONTEXT_FLAG_DEBUG_BIT)
   {
     GLCall(glEnable(GL_DEBUG_OUTPUT));
     GLCall(glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS));
     GLCall(glDebugMessageCallback(glDebugOutput, nullptr));
     GLCall(glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE));
   }
#endif

   GLCall(glEnable(GL_DEPTH_TEST));
   GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));

//   GLCall(glEnable(GL_BLEND));
//   GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

   glfwSetWindowUserPointer(GlfwWindow, this);
}

bool
Window::isOpen() const { return !glfwWindowShouldClose(GlfwWindow); }

void
Window::Close() { glfwSetWindowShouldClose(GlfwWindow, GL_TRUE); }

bool
Window::isViewPortModified()
{
   DEBUG_ASSERT(isOpen(), "The OpenGL window is not open.")

   // Adjust viewport, if necessary
   auto [width, height] = GetFrameBufferSize();
   if(width != ViewportDimensions[0] || height != ViewportDimensions[1])
   {
     ViewportDimensions[0] = width;
     ViewportDimensions[1] = height;
     GLCall(glViewport(0, 0, width, height));
     return true;
   }
   else return false;
}

void
Window::ResetViewPort() const { GLCall(glViewport(0, 0, ViewportDimensions[0], ViewportDimensions[1])); }

void
Window::SwapBuffers() { glfwSwapBuffers(GlfwWindow); }

void
Window::ResetTime() const { glfwSetTime(Zero); }

void
Window::ComputeDeltaTime()
{
   CurrentTime  = glfwGetTime();
   DeltaTime    = CurrentTime - PreviousTime;
   PreviousTime = CurrentTime;
}

Float
Window::GetCurrentTime() const { return CurrentTime; }

Float
Window::GetDeltaTime() const { return DeltaTime; }

GLfloat
Window::ComputeViewportAspectRatio() const { return static_cast<GLfloat>(ViewportDimensions[0]) / static_cast<GLfloat>(ViewportDimensions[1]); }

SVectorF2
Window::GetMouseDisplacement()
{
   const auto x_disp = MouseDisplacement[0];
   const auto y_disp = MouseDisplacement[1];
   MouseDisplacement[0] = Zero;
   MouseDisplacement[1] = Zero;
   return { x_disp, y_disp };
}

SVectorF2
Window::GetMouseWheelDisplacement()
{
   const auto x_disp = MouseWheelDisplacement[0];
   const auto y_disp = MouseWheelDisplacement[1];
   MouseWheelDisplacement[0] = Zero;
   MouseWheelDisplacement[1] = Zero;
   return {x_disp, y_disp };
}

/***************************************************************************************************************************************************************
* Private Interface
***************************************************************************************************************************************************************/
std::pair<GLint, GLint>
Window::GetFrameBufferSize() const
{
   GLint width, height;
   glfwGetFramebufferSize(GlfwWindow, &width, &height);
   return { width, height };
}

void
Window::CreateCallBacks() const
{
   glfwSetKeyCallback(GlfwWindow, HandleKeys);
   glfwSetCursorPosCallback(GlfwWindow, HandleMousePosition);
   glfwSetScrollCallback(GlfwWindow, HandleMouseWheel);
}

void
Window::HandleKeys(GLFWwindow* p_window, const GLint key, const GLint code, const GLint action, const GLint mode)
{
   // Get pointer to the Window object which contains p_window.
   Window* window = static_cast<Window*>(glfwGetWindowUserPointer(p_window));

   // Check for pressing of the ESC key.
   if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(p_window, GL_TRUE);

   // Check for press/release of all other keys.
   if(isBounded(key, static_cast<GLint>(0), mKeys))
   {
      if     (action == GLFW_PRESS)   window->Keys[key] = true;
      else if(action == GLFW_RELEASE) window->Keys[key] = false;
   }
}

void
Window::HandleMousePosition(GLFWwindow* p_window, const GLdouble x_coord, const GLdouble y_coord)
{
   Window* window = static_cast<Window*>(glfwGetWindowUserPointer(p_window));

   if(window->isFirstMouseMovement)
   {
      window->PreviousMousePosition = { x_coord, y_coord };
      window->isFirstMouseMovement = false;
   }

   window->MouseDisplacement     = { x_coord - window->PreviousMousePosition[0], y_coord - window->PreviousMousePosition[1] };
   window->PreviousMousePosition = { x_coord, y_coord };
}

void
Window::HandleMouseWheel(GLFWwindow* p_window, const GLdouble x_offset, const GLdouble y_offset)
{
   Window* window = static_cast<Window*>(glfwGetWindowUserPointer(p_window));
   window->MouseWheelDisplacement = { 0.0, y_offset };
}

void
Window::glDebugOutput(GLenum source, GLenum type, unsigned id, GLenum severity, GLsizei length, const char* message, const void* userParam)
{
   // Ignore insignificant error/warning codes
   if(id == OneOf(131169u, 131185u, 131218u, 131204u)) return;

   std::cout << "---------------" << std::endl;
   std::cout << "Debug message (" << id << "): " << message << std::endl;

   // Write error source
   switch(source)
   {
     case GL_DEBUG_SOURCE_API:               std::cout << "Source: API"; break;
     case GL_DEBUG_SOURCE_WINDOW_SYSTEM:     std::cout << "Source: Window System"; break;
     case GL_DEBUG_SOURCE_SHADER_COMPILER:   std::cout << "Source: Shader Compiler"; break;
     case GL_DEBUG_SOURCE_THIRD_PARTY:       std::cout << "Source: Third Party"; break;
     case GL_DEBUG_SOURCE_APPLICATION:       std::cout << "Source: Application"; break;
     case GL_DEBUG_SOURCE_OTHER:             std::cout << "Source: Other"; break;
   }
   std::cout << std::endl;

   // Write error type
   switch(type)
   {
     case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
     case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
     case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
     case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
     case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
     case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
     case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
     case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
     case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
   }
   std::cout << std::endl;

   // Write error severity
   switch(severity)
   {
     case GL_DEBUG_SEVERITY_HIGH:            std::cout << "Severity: high"; break;
     case GL_DEBUG_SEVERITY_MEDIUM:          std::cout << "Severity: medium"; break;
     case GL_DEBUG_SEVERITY_LOW:             std::cout << "Severity: low"; break;
     case GL_DEBUG_SEVERITY_NOTIFICATION:    std::cout << "Severity: notification"; break;
   }
   std::cout << std::endl;
}

}

