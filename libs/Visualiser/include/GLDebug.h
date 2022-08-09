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

/** Clear all errors OpenGL errors. */
inline void GLClearErrors()
{
  while(glGetError() != GL_NO_ERROR) {}
}

/** Throw current OpenGL error and exit program. */
inline void GLLogCall(const char* function, const char* file, const int line)
{
  if(GLenum error_code = glGetError())
  {
    std::string error_message;
    switch(error_code)
    {
      case GL_INVALID_ENUM:                  error_message = "INVALID_ENUM"; break;
      case GL_INVALID_VALUE:                 error_message = "INVALID_VALUE"; break;
      case GL_INVALID_OPERATION:             error_message = "INVALID_OPERATION"; break;
      case GL_STACK_OVERFLOW:                error_message = "STACK_OVERFLOW"; break;
      case GL_STACK_UNDERFLOW:               error_message = "STACK_UNDERFLOW"; break;
      case GL_OUT_OF_MEMORY:                 error_message = "OUT_OF_MEMORY"; break;
      case GL_INVALID_FRAMEBUFFER_OPERATION: error_message = "INVALID_FRAMEBUFFER_OPERATION"; break;
    }

    EXIT_FROM(file, line, "OpenGL error 0x0", std::hex, error_code, std::dec, " (", error_message,") from call:\n\t", function)
  }
}

}
