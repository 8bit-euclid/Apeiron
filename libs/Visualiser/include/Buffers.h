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

  inline GLuint GetID() { return ID; }

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
    Unbind();
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
    Unbind();
  }

  inline void Bind() const override { GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID)); }

  inline void Unbind() const override { GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)); }

  inline void Load(const DynamicArray<GLuint>& _indices) const { GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, nIndices * sizeof(GLuint), _indices.data(), GL_STATIC_DRAW)); }
};

/***************************************************************************************************************************************************************
* Frame Buffer Class
***************************************************************************************************************************************************************/
struct FrameBuffer
{
  FrameBuffer() { GLCall(glGenFramebuffers(1, &ID)); }

  ~FrameBuffer() { Delete(); }

  inline void Bind() const { GLCall(glBindFramebuffer(GL_FRAMEBUFFER, ID)); }

  inline void Unbind() const { GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0)); }

  inline void Load(GLenum _attachement, GLuint _mapID) const { GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, _attachement, GL_TEXTURE_2D, _mapID, 0)); }

  inline void Draw(GLenum _mode) const { GLCall(glDrawBuffer(_mode)); }

  inline void Read(GLenum _mode) const { GLCall(glReadBuffer(_mode)); }

  inline void Delete()
  {
    if(ID != 0)
    {
      GLCall(glDeleteFramebuffers(1, &ID));
      ID = 0;
    }
  }

protected:
  GLuint ID;
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
  VertexArray() { GLCall(glGenVertexArrays(1, &ID)); }

  ~VertexArray() { Delete(); }

  inline void AddBuffer(const VertexBuffer& _vertex_buffer, const VertexAttributeLayout& _vertex_layout)
  {
    _vertex_buffer.Bind();

    GLuint offset(0);
    FOR(i, _vertex_layout.Attributes.size())
    {
      const auto& element = _vertex_layout.Attributes[i];
      GLCall(glVertexAttribPointer(i, element.nComponents, element.GLType, element.isNormalised, _vertex_layout.Stride, reinterpret_cast<void*>(offset)));
      GLCall(glEnableVertexAttribArray(i));

      offset += element.nComponents * GLTypeSize(element.GLType);
    }

    _vertex_buffer.Unbind();
  }

  inline void Bind() const { GLCall(glBindVertexArray(ID)); }

  inline void Unbind() const { GLCall(glBindVertexArray(0)); }

  inline void Delete()
  {
    if(ID != 0)
    {
      GLCall(glDeleteBuffers(1, &ID));
      ID = 0;
    }
  }

  inline GLuint GetID() { return ID; }

private:
  GLuint ID;
};

}
