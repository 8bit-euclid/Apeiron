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

#pragma once

#include "../include/Global.h"
#include "../../DataContainer/include/Array.h"
#include "../../LinearAlgebra/include/Vector.h"
#include "GLDebug.h"
#include "GLTypes.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace aprn::vis {

class Window
{
 public:
   Window(GLint width, GLint height);

   ~Window();

   void Open(GLint width, GLint height);

   bool isOpen() const;

   void Close();

   bool isViewportModified();

   void ResetViewport() const;

   void SetTitle(const std::string& title, const bool append = false);

   void SwapBuffers();

   void InitTime() const;

   void ComputeDeltaTime();

   void ComputeFrameRate();

   Float DeltaTime() const;

   Float CurrentTime() const;

   GLfloat ViewportAspectRatio() const;

   SVectorF2 CursorDisplacement();

   SVectorF2 WheelDisplacement();

 private:
   friend class Visualiser;

   std::pair<GLint, GLint>
   ViewportDimensions() const;

   void CreateCallBacks() const;

   static void
   HandleKeys(GLFWwindow* p_window, const GLint key, const GLint code, const GLint action, const GLint mode);

   static void
   HandleMousePosition(GLFWwindow* p_window, const GLdouble x_coord, const GLdouble y_coord);

   static void
   HandleMouseWheel(GLFWwindow* p_window, const GLdouble x_offset, const GLdouble y_offset);

   static void APIENTRY
   glDebugOutput(GLenum source, GLenum type, unsigned id, GLenum severity, GLsizei length, const char* message, const void* userParam);

   std::string            _Title;
   Float                  _CurrentTime{};
   Float                  _PreviousTime{};
   Float                  _DeltaTime{};
   SVectorF2              _PreviousMousePosition;
   SVectorF2              _CursorDisplacement;
   SVectorF2              _WheelDisplacement;
   SVector2<GLint>        _WindowDimensions;
   SVector2<GLint>        _ViewportDimensions;
   constexpr static GLint _KeyCount{1024};
   SArrayB<_KeyCount>     _Keys;
   GLFWwindow*            _GlfwWindow;
   Float                  _PreviousFrameTime{};
   size_t                 _FrameCounter;
   bool                   _isFirstMouseMovement;
};

}
