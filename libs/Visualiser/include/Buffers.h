#pragma once

#include "../../../include/Global.h"
#include "Renderer.h"

namespace Apeiron{

/***************************************************************************************************************************************************************
* Buffer Abstract Base Class
***************************************************************************************************************************************************************/
class Buffer
{
protected:
  UInt BufferID;

public:
  Buffer()
  {
    STATIC_ASSERT((areTypesEqual<UInt, GLuint>()), "The unsigned integer type is incompatible with OpenGL.")
    GLCall(glGenBuffers(1, &BufferID));
  }
  ~Buffer() { GLCall(glDeleteBuffers(1, &BufferID)); }

  virtual void Pack(const void*, const UInt) const = 0;
  virtual void Bind() const = 0;
  virtual void Unbind() const = 0;
};

/***************************************************************************************************************************************************************
* Vertex Buffer Class
***************************************************************************************************************************************************************/
class VertexBuffer : public Buffer
{
public:
  VertexBuffer(const void* _data, const UInt _n_bytes) { Bind(); Pack(_data, _n_bytes);}

  inline void Pack(const void* _data, const UInt _n_bytes) const override { GLCall(glBufferData(GL_ARRAY_BUFFER, _n_bytes, _data, GL_STATIC_DRAW)); }

  inline void Bind() const override { GLCall(glBindBuffer(GL_ARRAY_BUFFER, BufferID)); }

  inline void Unbind() const override { GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0)); }
};

/***************************************************************************************************************************************************************
* Index Buffer Class
***************************************************************************************************************************************************************/
class IndexBuffer : public Buffer
{
public:
  UInt nIndices;

  IndexBuffer(const UInt* _data, const UInt _n_indices) : nIndices(_n_indices) { Bind(); Pack(_data, _n_indices); }

  inline void Pack(const void* _data, const UInt _n_indices) const override { GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, _n_indices * sizeof(UInt), _data, GL_STATIC_DRAW)); }

  inline void Bind() const override { GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferID)); }

  inline void Unbind() const override { GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)); }
};

}
