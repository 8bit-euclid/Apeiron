#pragma once

#include "../../../include/Global.h"
#include "../../DataContainers/include/Array.h"
#include "GLDebug.h"
#include "GLTypes.h"
#include "Mesh.h"

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace Apeiron{

/***************************************************************************************************************************************************************
* Buffer Abstract Base Class
***************************************************************************************************************************************************************/
struct Buffer
{
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

protected:
  GLuint ID;
};

/***************************************************************************************************************************************************************
* Vertex Buffer Class
***************************************************************************************************************************************************************/
struct VertexBuffer : public Buffer
{
  inline void Init(const DynamicArray<Vertex>& _vertices) const
  {
    Bind();
    Load(_vertices);
  }

  inline void Bind() const override { GLCall(glBindBuffer(GL_ARRAY_BUFFER, ID)); }

  inline void Unbind() const override { GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0)); }

  inline void Load(const DynamicArray<Vertex>& _vertices) const { GLCall(glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), _vertices.data(), GL_STATIC_DRAW)); }
};

/***************************************************************************************************************************************************************
* Index Buffer Class
***************************************************************************************************************************************************************/
struct IndexBuffer : public Buffer
{
  std::size_t nIndices;

  inline void Init(const DynamicArray<GLuint>& _indices)
  {
    nIndices = _indices.size();
    Bind();
    Load(_indices);
  }

  inline void Bind() const override { GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID)); }

  inline void Unbind() const override { GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)); }

  inline void Load(const DynamicArray<GLuint>& _indices) const { GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, nIndices * sizeof(GLuint), _indices.data(), GL_STATIC_DRAW)); }
};

/***************************************************************************************************************************************************************
* Shader Storage Buffer Class
***************************************************************************************************************************************************************/
struct ShaderStorageBuffer : public Buffer
{
  inline void Init(DynamicArray<glm::vec4>& _data)
  {
    Bind();
    Load(_data);
  }

  inline void Bind() const override { GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, ID)); }

  inline void BindBase() const { glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ID); }

  inline void Unbind() const override { GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0)); }

  inline void Load(DynamicArray<glm::vec4>& _data) const { GLCall(glBufferData(GL_SHADER_STORAGE_BUFFER, _data.size() * sizeof(glm::vec4), _data.data(), GL_STATIC_DRAW);); }
};

/***************************************************************************************************************************************************************
* Vertex Array Class
***************************************************************************************************************************************************************/

class VertexArray
{
public:
  VertexArray()
  {
    GLCall(glGenVertexArrays(1, &ID));
    Bind();
  }

  ~VertexArray() { Delete(); }

  inline void AddBuffer(const VertexBuffer& _vertex_buffer, const VertexAttributeLayout& _vertex_layout)
  {
    _vertex_buffer.Bind();
    const auto& elements = _vertex_layout.Attributes;
    GLuint offset = 0;
    FOR(i, elements.size())
    {
      const auto& element = elements[i];
      GLCall(glVertexAttribPointer(i, element.nComponents, element.Type, element.isNormalised, _vertex_layout.Stride, reinterpret_cast<const void*>(offset)));
      GLCall(glEnableVertexAttribArray(i));

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
