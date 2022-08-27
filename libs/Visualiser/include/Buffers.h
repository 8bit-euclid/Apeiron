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
* Buffer Types
***************************************************************************************************************************************************************/
enum class BufferType
{
   VBO,  // Vertex buffer
   VAO,  // Vertex array
   EBO,  // Index buffer
   FBO,  // Frame buffer
   RBO,  // Render buffer
   SSBO  // Shader storage buffer
};

/***************************************************************************************************************************************************************
* Buffer Abstract Base Class
***************************************************************************************************************************************************************/
namespace detail {

template<BufferType T>
class Buffer
{
   typedef BufferType BT;

 protected:
   Buffer() = default;

 public:
   Buffer(const Buffer<T>& buffer) = delete;

   Buffer(Buffer<T>&& buffer) noexcept;

   ~Buffer();

   void Init();

   void Bind() const;

   void Unbind() const;

   void Delete();

   Buffer& operator=(const Buffer<T>& buffer) = delete;

   Buffer& operator=(Buffer<T>&& buffer) noexcept;

   inline GLuint ID() const { return _ID; }

 protected:
   GLuint _ID{};

 private:
   void Bind(const GLuint id) const;
};

}

/***************************************************************************************************************************************************************
* Vertex Buffer Class
***************************************************************************************************************************************************************/
struct VertexBuffer : public detail::Buffer<BufferType::VBO>
{
   void Init(const DynamicArray<Vertex>& vertices);

   void Load(const DynamicArray<Vertex>& vertices) const;
};

/***************************************************************************************************************************************************************
* Vertex Array Class
***************************************************************************************************************************************************************/
struct VertexArray : public detail::Buffer<BufferType::VAO>
{
   void AddBuffer(const VertexBuffer& vertex_buffer, const VertexAttributeLayout& vertex_layout);
};

/***************************************************************************************************************************************************************
* Index Buffer Class
***************************************************************************************************************************************************************/
struct IndexBuffer : public detail::Buffer<BufferType::EBO>
{
   void Init(const DynamicArray<GLuint>& indices);

   void Load(const DynamicArray<GLuint>& indices);

   inline size_t IndexCount() const { return _IndexCount; }

 protected:
   size_t _IndexCount;
};

/***************************************************************************************************************************************************************
* Frame Buffer Class
***************************************************************************************************************************************************************/
struct FrameBuffer : public detail::Buffer<BufferType::FBO>
{
   void AttachTexture(GLenum attachement, GLuint texture_id) const;

   void AttachTexture2D(GLenum attachement, GLuint texture_id) const;

   void AttachRenderBuffer(GLenum attachement, GLuint rbo_id) const;

   void Check() const;

   void Draw(GLenum mode) const;

   void Draw(const DArray<GLenum>& modes) const;

   void Read(GLenum mode) const;
};

/***************************************************************************************************************************************************************
* Render Buffer Class
***************************************************************************************************************************************************************/
struct RenderBuffer : public detail::Buffer<BufferType::RBO>
{
   void Allocate(const GLenum format, const GLsizei width, const GLsizei height);
};

/***************************************************************************************************************************************************************
* Shader Storage Buffer Class
***************************************************************************************************************************************************************/
struct ShaderStorageBuffer : public detail::Buffer<BufferType::SSBO>
{
   void Init(DynamicArray<glm::vec4>& data);

   void BindBase() const;

   void Load(DynamicArray<glm::vec4>& data) const;
};

}
