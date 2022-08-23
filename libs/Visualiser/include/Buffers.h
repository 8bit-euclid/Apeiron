/***************************************************************************************************************************************************************
* GPL-3.0 License
* Copyright (C) 2022 Niran A. Ilangakoon
*
* This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as
* published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
* of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program.
* If not, see <https://www.gnu.org/licenses/>.
***************************************************************************************************************************************************************/

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
class Buffer
{
 protected:
   Buffer() = default;

 public:
   ~Buffer();

   void Init();

   virtual void Bind() const = 0;

   virtual void Unbind() const = 0;

   virtual void Delete();

   inline GLuint ID() const { return _ID; }

 protected:
   GLuint _ID{};
};

/***************************************************************************************************************************************************************
* Vertex Buffer Class
***************************************************************************************************************************************************************/
class VertexBuffer : public Buffer
{
 public:
   void Init(const DynamicArray<Vertex>& vertices);

   void Bind() const override;

   void Unbind() const override;

   void Load(const DynamicArray<Vertex>& vertices) const;
};

/***************************************************************************************************************************************************************
* Index Buffer Class
***************************************************************************************************************************************************************/
class IndexBuffer : public Buffer
{
 public:
   void Init(const DynamicArray<GLuint>& indices);

   void Bind() const override;

   void Unbind() const override;

   void Delete() override;

   void Load(const DynamicArray<GLuint>& indices) const;

   inline size_t GetIndexCount() const { return IndexCount; }

 protected:
   size_t IndexCount;
};

/***************************************************************************************************************************************************************
* Shader Storage Buffer Class
***************************************************************************************************************************************************************/
class ShaderStorageBuffer : public Buffer
{
 public:
   void Init(DynamicArray<glm::vec4>& data);

   void Bind() const override;

   void BindBase() const;

   void Unbind() const override;

   void Load(DynamicArray<glm::vec4>& data) const;
};

/***************************************************************************************************************************************************************
* Vertex Array Class
***************************************************************************************************************************************************************/
class VertexArray
{
 public:
   ~VertexArray();

   void Init();

   void AddBuffer(const VertexBuffer& vertex_buffer, const VertexAttributeLayout& vertex_layout);

   void Bind() const;

   void Unbind() const;

   void Delete();

 private:
   GLuint ID{};
};

/***************************************************************************************************************************************************************
* Frame Buffer Class
***************************************************************************************************************************************************************/
class FrameBuffer
{
 public:
   FrameBuffer() = default;

   FrameBuffer(const FrameBuffer& fbo) = delete;

   FrameBuffer(FrameBuffer&& fbo) noexcept;

   ~FrameBuffer();

   void Init();

   void Bind() const;

   void Unbind() const;

   void AttachTexture(GLenum attachement, GLuint texture_id) const;

   void AttachTexture2D(GLenum attachement, GLuint texture_id) const;

   void AttachRenderBuffer(GLenum attachement, GLuint rbo_id) const;

   void Check() const;

   void Draw(GLenum mode) const;

   void Read(GLenum mode) const;

   void Delete();

   FrameBuffer& operator=(const FrameBuffer& fbo) = delete;

   FrameBuffer& operator=(FrameBuffer&& fbo) noexcept;

 private:
   GLuint ID{};
};

/***************************************************************************************************************************************************************
* Render Buffer Class
***************************************************************************************************************************************************************/
class RenderBuffer
{
 public:
   RenderBuffer() = default;

   RenderBuffer(const RenderBuffer& rbo) = delete;

   RenderBuffer(RenderBuffer&& rbo) noexcept;

   ~RenderBuffer();

   void Init();

   void Allocate(const GLenum format, const GLsizei width, const GLsizei height);

   void Bind() const;

   void Unbind() const;

   void Delete();

   RenderBuffer& operator=(const RenderBuffer& rbo) = delete;

   RenderBuffer& operator=(RenderBuffer&& rbo) noexcept;

   inline GLuint ID() const { return _ID; }

 private:
   GLuint _ID{};
};

}
