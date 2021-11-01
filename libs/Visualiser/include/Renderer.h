#pragma once

#include "../../../include/Global.h"
#include <GL/glew.h>

namespace Apeiron{

#ifdef DEBUG_MODE
  #define GLCall(_function) GLClearErrors(); _function; GLLogCall(#_function, __FILE__, __LINE__)
#else
  #define GLCall(_function) _function
#endif

inline void GLClearErrors()
{
  while(glGetError() != GL_NO_ERROR);
}

inline void GLLogCall(const char* _function, const char* _file, const int _line)
{
  if(GLenum error = glGetError()) EXIT_FROM(_file, _line, "OpenGL error (0x0", std::hex, error, std::dec, ") from call:\n\t", _function)
}

}
