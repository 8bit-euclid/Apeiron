#pragma once

#include "../../../include/Global.h"
#include "../../DataContainer/include/Array.h"
#include "GLDebug.h"
#include "GLTypes.h"
#include "Mesh.h"

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace aprn::vis {

/***************************************************************************************************************************************************************
* Buffer Abstract Base Class
***************************************************************************************************************************************************************/
struct Buffer
{
 protected:
   Buffer();

 public:
   ~Buffer();

   virtual void Bind() const = 0;

   virtual void Unbind() const = 0;

   virtual void Delete();

   inline GLuint GetID() const { return ID; }

 protected:
   GLuint ID{};
};

/***************************************************************************************************************************************************************
* Vertex Buffer Class
***************************************************************************************************************************************************************/
struct VertexBuffer : public Buffer
{
   void Init(const DynamicArray<Vertex>& _vertices) const;

   void Bind() const override;

   void Unbind() const override;

   void Load(const DynamicArray<Vertex>& _vertices) const;
};

/***************************************************************************************************************************************************************
* Index Buffer Class
***************************************************************************************************************************************************************/
struct IndexBuffer : public Buffer
{
   void Init(const DynamicArray<GLuint>& _indices);

   void Bind() const override;

   void Unbind() const override;

   void Delete() override;

   void Load(const DynamicArray<GLuint>& _indices) const;

   inline size_t GetIndexCount() const { return IndexCount; }

 protected:
   size_t IndexCount;
};

/***************************************************************************************************************************************************************
* Shader Storage Buffer Class
***************************************************************************************************************************************************************/
struct ShaderStorageBuffer : public Buffer
{
   void Init(DynamicArray<glm::vec4>& _data);

   void Bind() const override;

   void BindBase() const;

   void Unbind() const override;

   void Load(DynamicArray<glm::vec4>& _data) const;
};

/***************************************************************************************************************************************************************
* Vertex Array Class
***************************************************************************************************************************************************************/
class VertexArray
{
 public:
   VertexArray();

   ~VertexArray();

   void AddBuffer(const VertexBuffer& _vertex_buffer, const VertexAttributeLayout& _vertex_layout);

   void Bind() const;

   void Unbind() const;

   void Delete();

 private:
   GLuint ID{};
};

/***************************************************************************************************************************************************************
* Frame Buffer Class
***************************************************************************************************************************************************************/
struct FrameBuffer
{
   FrameBuffer();

   FrameBuffer(const FrameBuffer& _fbo) = delete;

   FrameBuffer(FrameBuffer&& _fbo) noexcept;

   ~FrameBuffer();

   void Bind() const;

   void Unbind() const;

   void AttachTexture(GLenum _attachement, GLuint _mapID) const;

   void AttachTexture2D(GLenum _attachement, GLuint _mapID) const;

   void Draw(GLenum _mode) const;

   void Read(GLenum _mode) const;

   void Delete();

   FrameBuffer& operator=(const FrameBuffer& _fbo) = delete;

   FrameBuffer& operator=(FrameBuffer&& _fbo) noexcept;

 private:
   GLuint ID{};
};

}
