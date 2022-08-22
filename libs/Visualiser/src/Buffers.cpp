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
Buffer::~Buffer() { Delete(); }

void
Buffer::Init()
{
   ASSERT(glfwGetCurrentContext(), "An OpenGL context has not yet been created.")
   GLCall(glGenBuffers(1, &_ID));
}

void
Buffer::Delete()
{
   if(_ID != 0)
   {
      GLCall(glDeleteBuffers(1, &_ID));
      _ID = 0;
   }
}

/***************************************************************************************************************************************************************
* Vertex Buffer Class
***************************************************************************************************************************************************************/
void
VertexBuffer::Init(const DynamicArray<Vertex>& _vertices)
{
   Buffer::Init();
   Bind();
   Load(_vertices);
   Unbind();
}

void
VertexBuffer::Bind() const { GLCall(glBindBuffer(GL_ARRAY_BUFFER, _ID)); }

void
VertexBuffer::Unbind() const { GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0)); }

void
VertexBuffer::Load(const DynamicArray<Vertex>& _vertices) const
{
   GLCall(glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), _vertices.data(), GL_STATIC_DRAW));
}

/***************************************************************************************************************************************************************
* Index Buffer Class
***************************************************************************************************************************************************************/
void
IndexBuffer::Init(const DynamicArray<GLuint>& indices)
{
   Buffer::Init();
   IndexCount = indices.size();
   Bind();
   Load(indices);
   Unbind();
}

void
IndexBuffer::Bind() const { GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ID)); }

void
IndexBuffer::Unbind() const { GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)); }

void
IndexBuffer::Delete()
{
   Buffer::Delete();
   IndexCount = 0;
}

void
IndexBuffer::Load(const DynamicArray<GLuint>& indices) const
{
   GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexCount * sizeof(GLuint), indices.data(), GL_STATIC_DRAW));
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
}

void
ShaderStorageBuffer::Bind() const { GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, _ID)); }

void
ShaderStorageBuffer::BindBase() const { glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, _ID); }

void
ShaderStorageBuffer::Unbind() const { GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0)); }

void
ShaderStorageBuffer::Load(DynamicArray<glm::vec4>& data) const
{
   GLCall(glBufferData(GL_SHADER_STORAGE_BUFFER, data.size() * sizeof(glm::vec4), data.data(), GL_STATIC_DRAW););
}

/***************************************************************************************************************************************************************
* Vertex Array Class
***************************************************************************************************************************************************************/
VertexArray::~VertexArray() { Delete(); }

void
VertexArray::Init()
{
   ASSERT(glfwGetCurrentContext(), "An OpenGL context has not yet been created.")
   GLCall(glGenVertexArrays(1, &ID));
}

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

void
VertexArray::Bind() const { GLCall(glBindVertexArray(ID)); }

void
VertexArray::Unbind() const { GLCall(glBindVertexArray(0)); }

void
VertexArray::Delete()
{
   if(ID != 0)
   {
      GLCall(glDeleteVertexArrays(1, &ID));
      ID = 0;
   }
}

/***************************************************************************************************************************************************************
* Frame Buffer Class
***************************************************************************************************************************************************************/
FrameBuffer::FrameBuffer(FrameBuffer&& fbo) noexcept
{
   ID = fbo.ID;
   fbo.ID = 0;
}

FrameBuffer::~FrameBuffer() { Delete(); }

void
FrameBuffer::Init()
{
   ASSERT(glfwGetCurrentContext(), "An OpenGL context has not yet been created.")
   GLCall(glGenFramebuffers(1, &ID));
}

void
FrameBuffer::Bind() const { GLCall(glBindFramebuffer(GL_FRAMEBUFFER, ID)); }

void
FrameBuffer::Unbind() const { GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0)); }

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
FrameBuffer::Check() const
{
   GLCall(auto fb_status = glCheckFramebufferStatus(GL_FRAMEBUFFER));
   ASSERT(fb_status == GL_FRAMEBUFFER_COMPLETE, "Could not intialise frame buffer. Error code: ", fb_status)
}

void
FrameBuffer::Draw(GLenum mode) const { GLCall(glDrawBuffer(mode)); }

void
FrameBuffer::Read(GLenum mode) const { GLCall(glReadBuffer(mode)); }

void
FrameBuffer::Delete()
{
   if(ID != 0)
   {
      GLCall(glDeleteFramebuffers(1, &ID));
      ID = 0;
   }
}

FrameBuffer&
FrameBuffer::operator=(FrameBuffer&& fbo) noexcept
{
   Delete();
   ID = fbo.ID;
   fbo.ID = 0;

   return *this;
}

}