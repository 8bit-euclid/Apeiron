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

#include <GL/glew.h>
#include "../../../include/Global.h"

namespace aprn::vis {

/** OpenGL function wrapper for error checking in debug mode. */
#ifdef DEBUG_MODE
   #define GLCall(function) GLClearErrors(); function; GLLogCall(#function, __FILE__, __LINE__)
#else
   #define GLCall(function) function
#endif

/** Clear all OpenGL errors. */
inline void GLClearErrors() { while(glGetError() != GL_NO_ERROR) {} }

/** Throw current OpenGL error and exit program. */
inline void GLLogCall(const char* function, const char* file, const int line)
{
  if(GLenum error_code = glGetError())
  {
    std::string error_message;
    switch(error_code)
    {
      case GL_INVALID_ENUM:                  error_message = "INVALID_ENUM";                  break;
      case GL_INVALID_VALUE:                 error_message = "INVALID_VALUE";                 break;
      case GL_INVALID_OPERATION:             error_message = "INVALID_OPERATION";             break;
      case GL_STACK_OVERFLOW:                error_message = "STACK_OVERFLOW";                break;
      case GL_STACK_UNDERFLOW:               error_message = "STACK_UNDERFLOW";               break;
      case GL_OUT_OF_MEMORY:                 error_message = "OUT_OF_MEMORY";                 break;
      case GL_INVALID_FRAMEBUFFER_OPERATION: error_message = "INVALID_FRAMEBUFFER_OPERATION"; break;
    }

    EXIT_FROM(file, line, "OpenGL error 0x0", std::hex, error_code, std::dec, " (", error_message,") from call:\n\t", function)
  }
}

}
