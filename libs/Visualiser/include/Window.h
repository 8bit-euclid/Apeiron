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

constexpr static GLint mKeys{1024};

class Window
{
 public:
   Window(GLint width, GLint height);

   ~Window();

   void Open(GLint width, GLint height);

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
   HandleKeys(GLFWwindow* p_window, const GLint key, const GLint code, const GLint action, const GLint mode);

   static void
   HandleMousePosition(GLFWwindow* p_window, const GLdouble x_coord, const GLdouble y_coord);

   static void
   HandleMouseWheel(GLFWwindow* p_window, const GLdouble x_offset, const GLdouble y_offset);

   static void APIENTRY
   glDebugOutput(GLenum source, GLenum type, unsigned id, GLenum severity, GLsizei length, const char* message, const void* userParam);

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
