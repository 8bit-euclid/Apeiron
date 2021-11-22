#pragma once

#include "../../../include/Global.h"
#include "../../DataContainers/include/Array.h"
#include "GLDebug.h"
#include "GLTypes.h"

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace Apeiron{

/***************************************************************************************************************************************************************
* Buffer Abstract Base Class
***************************************************************************************************************************************************************/
class Buffer
{
protected:
  GLuint ID;

public:
  Buffer() { GLCall(glGenBuffers(1, &ID)); }

  ~Buffer() { Delete(); }

  virtual void Bind() const = 0;
  virtual void Unbind() const = 0;

  inline void Delete()
  {
    if(ID != 0)
    {
      GLCall(glDeleteBuffers(1, &ID));
      ID = 0;
    }
  }
};

/***************************************************************************************************************************************************************
* Vertex Buffer Class
***************************************************************************************************************************************************************/
struct Vertex
{
  glm::vec3 Position;
  glm::vec3 Normal;
  glm::vec3 Colour;
  glm::vec2 TextureCoordinates;
};

class VertexBuffer : public Buffer
{
public:
  VertexBuffer(const DynamicArray<Vertex>& _vertices) { Bind(); Pack(_vertices); }

  inline void Bind() const override { GLCall(glBindBuffer(GL_ARRAY_BUFFER, ID)); }

  inline void Unbind() const override { GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0)); }

  inline void Pack(const DynamicArray<Vertex>& _vertices) const { GLCall(glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), _vertices.data(), GL_STATIC_DRAW)); }
};

/***************************************************************************************************************************************************************
* Index Buffer Class
***************************************************************************************************************************************************************/
class IndexBuffer : public Buffer
{
public:
  UInt nIndices;

  IndexBuffer(const DynamicArray<GLuint>& _indices) : nIndices(_indices.size()) { Bind(); Pack(_indices); }

  inline void Bind() const override { GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID)); }

  inline void Unbind() const override { GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)); }

  inline void Pack(const DynamicArray<GLuint>& _indices) const { GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, nIndices * sizeof(GLuint), _indices.data(), GL_STATIC_DRAW)); }
};

/***************************************************************************************************************************************************************
* Shader Storage Buffer Class
***************************************************************************************************************************************************************/
class ShaderStorageBuffer : public Buffer
{
public:
  ShaderStorageBuffer(DynamicArray<glm::vec4>& _data) { Bind(); Pack(_data); }

  inline void Bind() const override { GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, ID)); }

  inline void BindBase() const { glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ID); }

  inline void Unbind() const override { GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0)); }

  inline void Pack(DynamicArray<glm::vec4>& _data) const
  {
    GLCall(glBufferData(GL_SHADER_STORAGE_BUFFER, _data.size() * sizeof(glm::vec4), _data.data(), GL_STATIC_DRAW););
  }
};

/***************************************************************************************************************************************************************
* Vertex Array Supporting Classes
***************************************************************************************************************************************************************/

struct VertexAttribute
{
  GLenum Type;
  GLint nComponents;
  GLboolean isNormalised;

  VertexAttribute(const GLenum _type, const GLint _n_components, const GLboolean _is_normalised)
    : Type(_type), nComponents(_n_components), isNormalised(_is_normalised) {}
  ~VertexAttribute() = default;
};

struct VertexBufferLayout
{
  DynamicArray<VertexAttribute> Attributes;
  GLsizei Stride;

  VertexBufferLayout() : Stride(0) {};
  ~VertexBufferLayout() = default;

  template<typename t_data_type>
  inline void AddEntity(const UInt _n_values)
  {
    constexpr GLenum typeEnum = GLTypeEnum<t_data_type>();
    Attributes.emplace_back(typeEnum, _n_values, (typeEnum == GL_UNSIGNED_BYTE ? GL_TRUE : GL_FALSE));
    Stride += _n_values * GLTypeSize(typeEnum);
  }
};

class VertexArray
{
public:
  VertexArray() { GLCall(glGenVertexArrays(1, &ID)); Bind(); }

  ~VertexArray() { Delete(); }

  inline void AddBuffer(const VertexBuffer& _vertex_buffer, const VertexBufferLayout& _layout)
  {
    _vertex_buffer.Bind();
    const auto& elements = _layout.Attributes;
    GLuint offset = 0;
    FOR(i, elements.size())
    {
      const auto& element = elements[i];
      GLCall(glEnableVertexAttribArray(i));
      GLCall(glVertexAttribPointer(i, element.nComponents, element.Type, element.isNormalised, _layout.Stride, reinterpret_cast<const void*>(offset)));
      offset += element.nComponents * GLTypeSize(element.Type);
    }
  }

  inline void Bind() const { GLCall(glBindVertexArray(ID)); }

  inline void Unbind() const { GLCall(glBindVertexArray(0)); }

  inline void Delete()
  {
    if(ID != 0)
    {
      GLCall(glDeleteVertexArrays(1, &ID));
      ID = 0;
    }
  }

private:
  GLuint ID;
};

}
