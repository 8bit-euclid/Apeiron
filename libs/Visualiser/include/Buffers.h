#pragma once

#include <GL/glew.h>
#include "../../../include/Global.h"
#include "../../DataContainers/include/Array.h"
#include "GLDebug.h"
#include "GLTypes.h"

namespace Apeiron{

/***************************************************************************************************************************************************************
* Buffer Abstract Base Class
***************************************************************************************************************************************************************/
class Buffer
{
protected:
  UInt RendererID;

public:
  Buffer()
  {
    STATIC_ASSERT((isTypeEqual<UInt, GLuint>()), "The unsigned integer type is incompatible with OpenGL.")
    GLCall(glGenBuffers(1, &RendererID));
  }
  ~Buffer() { GLCall(glDeleteBuffers(1, &RendererID)); }

  virtual void Bind() const = 0;
  virtual void Unbind() const = 0;
};

/***************************************************************************************************************************************************************
* Vertex Buffer Class
***************************************************************************************************************************************************************/
class VertexBuffer : public Buffer
{
public:
  VertexBuffer(const void* _data, const UInt _n_bytes) { Bind(); Pack(_data, _n_bytes); }

  inline void Bind() const override { GLCall(glBindBuffer(GL_ARRAY_BUFFER, RendererID)); }

  inline void Unbind() const override { GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0)); }

  inline void Pack(const void* _data, const UInt _n_bytes) const { GLCall(glBufferData(GL_ARRAY_BUFFER, _n_bytes, _data, GL_STATIC_DRAW)); }
};

/***************************************************************************************************************************************************************
* Index Buffer Class
***************************************************************************************************************************************************************/
class IndexBuffer : public Buffer
{
public:
  UInt nIndices;

  IndexBuffer(const UInt* _data, const UInt _n_indices) : nIndices(_n_indices) { Bind(); Pack(_data, _n_indices); }

  inline void Bind() const override { GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RendererID)); }

  inline void Unbind() const override { GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)); }

  inline void Pack(const UInt* _data, const UInt _n_indices) const { GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, _n_indices * sizeof(UInt), _data, GL_STATIC_DRAW)); }
};

/***************************************************************************************************************************************************************
* Vertex Array Supporting Classes
***************************************************************************************************************************************************************/

struct VertexBufferEntity
{
  GLenum Type;
  GLint nAttributes;
  GLboolean isNormalised;

  VertexBufferEntity(const GLenum _type, const GLint _n_attributes, const GLboolean _is_normalised)
    : Type(_type), nAttributes(_n_attributes), isNormalised(_is_normalised) {}
  ~VertexBufferEntity() = default;
};

struct VertexBufferLayout
{
  DynamicArray<VertexBufferEntity> Entities;
  GLsizei Stride;

  VertexBufferLayout() : Stride(0) {};
  ~VertexBufferLayout() = default;

  template<typename t_data_type>
  inline void Push(const UInt _n_values)
  {
    constexpr GLenum typeEnum = GLTypeEnum<t_data_type>();
    Entities.emplace_back(typeEnum, _n_values, (typeEnum == GL_UNSIGNED_BYTE ? GL_TRUE : GL_FALSE));
    Stride += _n_values * GLTypeSize(typeEnum);
  }
};

class VertexArray
{
private:
  UInt VertexArrayID;

public:
  VertexArray() { GLCall(glGenVertexArrays(1, &VertexArrayID)); }

  ~VertexArray() { GLCall(glDeleteVertexArrays(1, &VertexArrayID)); }

  inline void AddBuffer(const VertexBuffer& _vertex_buffer, const VertexBufferLayout& _layout)
  {
    Bind();
    _vertex_buffer.Bind();
    const auto& elements = _layout.Entities;
    UInt offset = 0;
    FOR(i, elements.size())
    {
      const auto& element = elements[i];
      GLCall(glEnableVertexAttribArray(i));
      GLCall(glVertexAttribPointer(i, element.nAttributes, element.Type, element.isNormalised, _layout.Stride, (const void*)(LInt)offset));
      offset += element.nAttributes * GLTypeSize(element.Type);
    }
  }

  inline void Bind() const { GLCall(glBindVertexArray(VertexArrayID)); }

  inline void Unbind() const { GLCall(glBindVertexArray(0)); }
};

}
