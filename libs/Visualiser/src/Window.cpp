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

namespace aprn::vis {

/***************************************************************************************************************************************************************
* Public Interface
***************************************************************************************************************************************************************/
Window::Window(GLint width, GLint height)
   : _WindowDimensions{width, height} {  }

Window::~Window() { glfwTerminate(); }

void
Window::Open() { Open(_WindowDimensions.x(), _WindowDimensions.y()); }

void
Window::Open(const GLint width, const GLint height)
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
   glfwWindowHint(GLFW_SAMPLES, 24); // Anti-aliasing multi-sample count
#ifdef DEBUG_MODE
   glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif

   // Create a window and its OpenGL context.
   _WindowDimensions = { width, height };
   _GlfwWindow = glfwCreateWindow(width, height, "Apeiron", nullptr, nullptr);
   if(!_GlfwWindow)
   {
      glfwTerminate();
      EXIT("Could not create an OpenGL window.")
   }

   // Set context for GLEW to use
   glfwMakeContextCurrent(_GlfwWindow);
   glfwSwapInterval(1);

   // Set viewport dimensions
   std::tie(_ViewportDimensions[0], _ViewportDimensions[1]) = ViewportDimensions();

   // Handle key mouse inputs
   CreateCallBacks();
   glfwSetInputMode(_GlfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Don't show cursor in window.

   // Allow modern extension features
   glewExperimental = GL_TRUE;

   // Initialise GLEW
   if(glewInit() != GLEW_OK)
   {
      glfwDestroyWindow(_GlfwWindow);
      glfwTerminate();
      EXIT("Failed to Initialise GLEW.")
   }
   else Print("\nRunning OpenGL Version:", glGetString(GL_VERSION));

   glfwSetWindowUserPointer(_GlfwWindow, this);
}

bool
Window::isOpen() const { return !glfwWindowShouldClose(_GlfwWindow); }

void
Window::Close() { glfwSetWindowShouldClose(_GlfwWindow, GL_TRUE); }

bool
Window::isViewportModified()
{
   DEBUG_ASSERT(isOpen(), "The OpenGL window is not open.")

   // Adjust viewport, if necessary
   auto [width, height] = ViewportDimensions();
   if(width != _ViewportDimensions[0] || height != _ViewportDimensions[1])
   {
      _ViewportDimensions[0] = width;
      _ViewportDimensions[1] = height;
      GLCall(glViewport(0, 0, width, height));
      return true;
   }
   else return false;
}

void
Window::ResetViewport() const { GLCall(glViewport(0, 0, _ViewportDimensions[0], _ViewportDimensions[1])); }

void
Window::SetTitle(const std::string& title, const bool append)
{
   if(append) glfwSetWindowTitle(_GlfwWindow, (_Title + title).c_str());
   else
   {
      _Title = title;
      glfwSetWindowTitle(_GlfwWindow, _Title.c_str());
   }
}

void
Window::SwapBuffers() { glfwSwapBuffers(_GlfwWindow); }

void
Window::InitTime() const { glfwSetTime(Zero); }

void
Window::ComputeDeltaTime()
{
   _CurrentTime  = glfwGetTime();
   _DeltaTime    = _CurrentTime - _PreviousTime;
   _PreviousTime = _CurrentTime;
}

void
Window::ComputeFrameRate()
{
   _CurrentTime  = glfwGetTime();
   const auto delta_time = _CurrentTime - _PreviousFrameTime;
   ++_FrameCounter;

   if(delta_time > Tenth)
   {
      const auto fps = static_cast<Float>(_FrameCounter) / delta_time;
      const auto frame_duration = 1.0e3 / fps; // in milliseconds
      const std::string title_suffix = "  |  " + ToString(fps, 2) + " fps  |  " + ToString(frame_duration, 2) + " ms";
      SetTitle(title_suffix, true);

      _PreviousFrameTime = _CurrentTime;
      _FrameCounter = 0;
   }
}

Float
Window::CurrentTime() const { return _CurrentTime; }

Float
Window::DeltaTime() const { return _DeltaTime; }

GLfloat
Window::ViewportAspectRatio() const { return static_cast<GLfloat>(_ViewportDimensions[0]) / static_cast<GLfloat>(_ViewportDimensions[1]); }

SVectorF2
Window::CursorDisplacement()
{
   const auto x_disp = _CursorDisplacement[0];
   const auto y_disp = _CursorDisplacement[1];
   _CursorDisplacement[0] = Zero;
   _CursorDisplacement[1] = Zero;
   return { x_disp, y_disp };
}

SVectorF2
Window::WheelDisplacement()
{
   const auto x_disp = _WheelDisplacement[0];
   const auto y_disp = _WheelDisplacement[1];
   _WheelDisplacement[0] = Zero;
   _WheelDisplacement[1] = Zero;
   return { x_disp, y_disp };
}

/***************************************************************************************************************************************************************
* Private Interface
***************************************************************************************************************************************************************/
Pair<GLint>
Window::ViewportDimensions() const
{
   GLint width, height;
   glfwGetFramebufferSize(_GlfwWindow, &width, &height);
   return { width, height };
}

void
Window::CreateCallBacks() const
{
   glfwSetKeyCallback(_GlfwWindow, HandleKeys);
   glfwSetCursorPosCallback(_GlfwWindow, HandleMousePosition);
   glfwSetScrollCallback(_GlfwWindow, HandleMouseWheel);
}

void
Window::HandleKeys(GLFWwindow* p_window, const GLint key, [[maybe_unused]] const GLint code, const GLint action, [[maybe_unused]] const GLint mode)
{
   // Get pointer to the Window object which contains p_window.
   Window* window = static_cast<Window*>(glfwGetWindowUserPointer(p_window));

   // Check for pressing of the ESC key.
   if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(p_window, GL_TRUE);

   // Check for press/release of all other keys.
   if(isBounded(key, static_cast<GLint>(0), _KeyCount))
   {
      if     (action == GLFW_PRESS)   window->_Keys[key] = true;
      else if(action == GLFW_RELEASE) window->_Keys[key] = false;
   }
}

void
Window::HandleMousePosition(GLFWwindow* p_window, const GLdouble x_coord, const GLdouble y_coord)
{
   Window* window = static_cast<Window*>(glfwGetWindowUserPointer(p_window));

   if(window->_isFirstCursorMotion)
   {
      window->_PreviousCursorPosition = {x_coord, y_coord };
      window->_isFirstCursorMotion = false;
   }

   window->_CursorDisplacement    = { x_coord - window->_PreviousCursorPosition[0], y_coord - window->_PreviousCursorPosition[1] };
   window->_PreviousCursorPosition = {x_coord, y_coord };
}

void
Window::HandleMouseWheel(GLFWwindow* p_window, [[maybe_unused]] const GLdouble x_offset, const GLdouble y_offset)
{
   Window* window = static_cast<Window*>(glfwGetWindowUserPointer(p_window));
   window->_WheelDisplacement = {0.0, y_offset };
}

}

