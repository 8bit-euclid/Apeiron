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

#include "../include/Buffers.h"
#include <GLFW/glfw3.h>

namespace aprn::vis {

/***************************************************************************************************************************************************************
* Buffer Abstract Base Class
***************************************************************************************************************************************************************/
namespace detail {

template<BufferType T>
Buffer<T>::Buffer(Buffer<T>&& buffer) noexcept { *this = std::move(buffer); }

template<BufferType T>
Buffer<T>::~Buffer() { Delete(); }

template<BufferType T>
void
Buffer<T>::Init()
{
   ASSERT(glfwGetCurrentContext(), "Cannot intialise buffer without an OpenGL context.")

   if constexpr(T == OneOf(BT::VBO, BT::EBO, BT::SSBO)) { GLCall(glGenBuffers(1, &_ID)); }
   else if(T == BT::VAO) { GLCall(glGenVertexArrays(1, &_ID)); }
   else if(T == BT::FBO) { GLCall(glGenFramebuffers(1, &_ID)); }
   else if(T == BT::RBO) { GLCall(glGenRenderbuffers(1, &_ID)); }
   else throw "Cannot initialise buffer - unrecognised buffer type.";
}

template<BufferType T>
void
Buffer<T>::Bind() const { Bind(_ID); }

template<BufferType T>
void
Buffer<T>::Unbind() const { Bind(0); }

template<BufferType T>
void
Buffer<T>::Delete()
{
   if(_ID != 0)
   {
      if constexpr(T == OneOf(BT::VBO, BT::EBO, BT::SSBO)) { GLCall(glDeleteBuffers(1, &_ID)); }
      else if(T == BT::VAO) { GLCall(glDeleteVertexArrays(1, &_ID)); }
      else if(T == BT::FBO) { GLCall(glDeleteFramebuffers(1, &_ID)); }
      else if(T == BT::RBO) { GLCall(glDeleteRenderbuffers(1, &_ID)); }
      else throw "Cannot delete buffer - unrecognised buffer type.";
      _ID = 0;
   }
}

template<BufferType T>
Buffer<T>&
Buffer<T>::operator=(Buffer<T>&& buffer) noexcept
{
   Delete();
   _ID = buffer._ID;
   buffer._ID = 0; // Note: must not call buffer.Delete() here!
   return *this;
}

template<BufferType T>
void
Buffer<T>::Bind(const GLuint id) const
{
   if constexpr(T == BT::VBO)  { GLCall(glBindBuffer(GL_ARRAY_BUFFER         , id)); }
   else if     (T == BT::EBO)  { GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER , id)); }
   else if     (T == BT::SSBO) { GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, id)); }
   else if     (T == BT::FBO)  { GLCall(glBindFramebuffer(GL_FRAMEBUFFER     , id)); }
   else if     (T == BT::RBO)  { GLCall(glBindRenderbuffer(GL_RENDERBUFFER   , id)); }
   else if     (T == BT::VAO)  { GLCall(glBindVertexArray(id)); }
   else throw "Cannot bind/unbind buffer - unrecognised buffer type.";
}

template class Buffer<BufferType::VBO>;
template class Buffer<BufferType::VAO>;
template class Buffer<BufferType::EBO>;
template class Buffer<BufferType::FBO>;
template class Buffer<BufferType::RBO>;
template class Buffer<BufferType::SSBO>;

}

/***************************************************************************************************************************************************************
* Vertex Buffer Class
***************************************************************************************************************************************************************/
void
VertexBuffer::Init(const DynamicArray<Vertex>& vertices)
{
   Buffer::Init();
   Bind();
   Load(vertices);
   Unbind();
}

void
VertexBuffer::Load(const DynamicArray<Vertex>& vertices) const
{
   GLCall(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW));
}

/***************************************************************************************************************************************************************
* Vertex Array Class
***************************************************************************************************************************************************************/
void
VertexArray::AddBuffer(const VertexBuffer& vertex_buffer, const VertexAttributeLayout& vertex_layout)
{
   vertex_buffer.Bind();

   GLuint offset(0);
   FOR(i, vertex_layout.Attributes.size())
   {
      const auto& element = vertex_layout.Attributes[i];
      GLCall(glVertexAttribPointer(i, element.nComponents, element.GLType, element.isNormalised, vertex_layout.Stride, reinterpret_cast<void*>(offset)));
      GLCall(glEnableVertexAttribArray(i));

      offset += element.nComponents * GLTypeSize(element.GLType);
   }

   vertex_buffer.Unbind();
}

/***************************************************************************************************************************************************************
* Index Buffer Class
***************************************************************************************************************************************************************/
void
IndexBuffer::Init(const DynamicArray<GLuint>& indices)
{
   Buffer::Init();
   Bind();
   Load(indices);
   Unbind();
}

void
IndexBuffer::Load(const DynamicArray<GLuint>& indices)
{
   _IndexCount = indices.size();
   GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, _IndexCount * sizeof(GLuint), indices.data(), GL_STATIC_DRAW));
}

/***************************************************************************************************************************************************************
* Frame Buffer Class
***************************************************************************************************************************************************************/
void
FrameBuffer::AttachTexture(const GLenum attachement, const GLuint texture_id) const
{
   GLCall(glFramebufferTexture(GL_FRAMEBUFFER, attachement, texture_id, 0));
   Check();
}

void
FrameBuffer::AttachTexture2D(const GLenum attachement, const GLuint texture_id) const
{
   GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, attachement, GL_TEXTURE_2D, texture_id, 0));
   Check();
}

void
FrameBuffer::AttachRenderBuffer(GLenum attachement, GLuint rbo_id) const
{
   GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachement, GL_RENDERBUFFER, rbo_id));
   Check();
}

void
FrameBuffer::Check() const
{
   GLCall(auto fb_status = glCheckFramebufferStatus(GL_FRAMEBUFFER));
   ASSERT(fb_status == GL_FRAMEBUFFER_COMPLETE, "Could not intialise frame buffer. Error code: ", fb_status)
}

void
FrameBuffer::Draw(GLenum mode) const { GLCall(glDrawBuffer(mode)); }

void
FrameBuffer::Read(GLenum mode) const { GLCall(glReadBuffer(mode)); }

/***************************************************************************************************************************************************************
* Render Buffer Class
***************************************************************************************************************************************************************/
void
RenderBuffer::Allocate(const GLenum format, const GLsizei width, const GLsizei height)
{
   Bind();
   GLCall(glRenderbufferStorage(GL_RENDERBUFFER, format, width, height));
   Unbind();
}

/***************************************************************************************************************************************************************
* Shader Storage Buffer Class
***************************************************************************************************************************************************************/
void
ShaderStorageBuffer::Init(DynamicArray<glm::vec4>& data)
{
   Buffer::Init();
   Bind();
   Load(data);
   Unbind();
}

void
ShaderStorageBuffer::BindBase() const { glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, _ID); }

void
ShaderStorageBuffer::Load(DynamicArray<glm::vec4>& data) const
{
   GLCall(glBufferData(GL_SHADER_STORAGE_BUFFER, data.size() * sizeof(glm::vec4), data.data(), GL_STATIC_DRAW));
}

}