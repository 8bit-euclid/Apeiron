#pragma once

#include "../../../include/Global.h"
#include "../../LinearAlgebra/include/Vector.h"

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace aprn::vis {

template<typename T>
consteval GLenum
GLTypeEnum(const T& _value = T())
{
  if constexpr(isTypeSame<T, GLbyte>()) return GL_BYTE;
  else if constexpr(isTypeSame<T, GLubyte>()) return GL_UNSIGNED_BYTE;
  else if constexpr(isTypeSame<T, GLshort>()) return GL_SHORT;
  else if constexpr(isTypeSame<T, GLushort>()) return GL_UNSIGNED_SHORT;
  else if constexpr(isTypeSame<T, GLint>()) return GL_INT;
  else if constexpr(isTypeSame<T, GLuint>()) return GL_UNSIGNED_INT;
  else if constexpr(isTypeSame<T, GLfloat>()) return GL_FLOAT;
  else if constexpr(isTypeSame<T, GLdouble>()) return GL_DOUBLE;
  else EXIT("Unrecognised OpenGL data type: ")
}

constexpr GLuint
GLTypeSize(const GLenum _gl_type)
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
  EXIT("Unrecognised OpenGL data type.")
}

/** Convert a GLM vector to a static vector. */
template<size_t size, typename T, glm::qualifier qual = glm::defaultp>
consteval StaticVector<T, size>
GlmVecToSArray(const glm::vec<size, T, qual>& _in_vector)
{
  StaticVector<T, size> out_vector;
  FOR(i, size) out_vector[i] = _in_vector[i];
  return out_vector;
}

/** Convert a static vector to a GLM vector. */
template<size_t size, typename T, glm::qualifier qual = glm::defaultp>
constexpr glm::vec<size, T, qual>
SArrayToGlmVec(const StaticVector<T, size>& _in_vector)
{
  glm::vec<size, T, qual> out_vector;
  FOR(i, size) out_vector[i] = _in_vector[i];
  return out_vector;
}

}
