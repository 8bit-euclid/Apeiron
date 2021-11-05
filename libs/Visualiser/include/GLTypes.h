#pragma once

#include <GL/glew.h>
#include "../../../include/Global.h"

namespace Apeiron {

template<typename t_data_type>
constexpr GLenum GLTypeEnum(const t_data_type& _value = t_data_type())
{
  if constexpr(isTypeEqual<t_data_type, GLbyte>()) return GL_BYTE;
  else if constexpr(isTypeEqual<t_data_type, GLubyte>()) return GL_UNSIGNED_BYTE;
  else if constexpr(isTypeEqual<t_data_type, GLshort>()) return GL_SHORT;
  else if constexpr(isTypeEqual<t_data_type, GLushort>()) return GL_UNSIGNED_SHORT;
  else if constexpr(isTypeEqual<t_data_type, GLint>()) return GL_INT;
  else if constexpr(isTypeEqual<t_data_type, GLuint>()) return GL_UNSIGNED_INT;
  else if constexpr(isTypeEqual<t_data_type, GLfloat>()) return GL_FLOAT;
  else if constexpr(isTypeEqual<t_data_type, GLdouble>()) return GL_DOUBLE;
  else EXIT("Unrecognised OpenGL data type: ");
}

constexpr GLsizei GLTypeSize(const GLenum _gl_type)
{
  switch(_gl_type)
  {
    case GL_BYTE: return sizeof(GLbyte);
    case GL_UNSIGNED_BYTE: return sizeof(GLubyte);
    case GL_SHORT: return sizeof(GLshort);
    case GL_UNSIGNED_SHORT: return sizeof(GLushort);
    case GL_INT: return sizeof(GLint);
    case GL_UNSIGNED_INT: return sizeof(GLuint);
    case GL_FLOAT: return sizeof(GLfloat);
    case GL_DOUBLE: return sizeof(GLdouble);
  }
  EXIT("Unrecognised OpenGL data type.");
}

}
