#pragma once

#include "../../../include/Global.h"
#include "../../LinearAlgebra/include/Vector.h"

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace Apeiron {

template<typename T>
consteval GLenum
GLTypeEnum(const T& _value = T())
{
  if constexpr(isTypeEqual<T, GLbyte>()) return GL_BYTE;
  else if constexpr(isTypeEqual<T, GLubyte>()) return GL_UNSIGNED_BYTE;
  else if constexpr(isTypeEqual<T, GLshort>()) return GL_SHORT;
  else if constexpr(isTypeEqual<T, GLushort>()) return GL_UNSIGNED_SHORT;
  else if constexpr(isTypeEqual<T, GLint>()) return GL_INT;
  else if constexpr(isTypeEqual<T, GLuint>()) return GL_UNSIGNED_INT;
  else if constexpr(isTypeEqual<T, GLfloat>()) return GL_FLOAT;
  else if constexpr(isTypeEqual<T, GLdouble>()) return GL_DOUBLE;
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
template<std::size_t size, typename T, glm::qualifier qual = glm::defaultp>
consteval StaticVector<T, size>
ConvertGlmVecToStaticArray(const glm::vec<size, T, qual>& _in_vector)
{
  StaticVector<T, size> out_vector;
  FOR(i, size) out_vector[i] = _in_vector[i];
  return out_vector;
}

/** Convert a static vector to a GLM vector. */
template<std::size_t size, typename T, glm::qualifier qual = glm::defaultp>
constexpr glm::vec<size, T, qual>
ConvertStaticArrayToGlmVec(const StaticVector<T, size>& _in_vector)
{
  glm::vec<size, T, qual> out_vector;
  FOR(i, size) out_vector[i] = _in_vector[i];
  return out_vector;
}

}
