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
IndexBuffer::Init(const DynamicArray<GLuint>& _indices)
{
   Buffer::Init();
   IndexCount = _indices.size();
   Bind();
   Load(_indices);
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
IndexBuffer::Load(const DynamicArray<GLuint>& _indices) const
{
   GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexCount * sizeof(GLuint), _indices.data(), GL_STATIC_DRAW));
}

/***************************************************************************************************************************************************************
* Shader Storage Buffer Class
***************************************************************************************************************************************************************/
void
ShaderStorageBuffer::Init(DynamicArray<glm::vec4>& _data)
{
   Buffer::Init();
   Bind();
   Load(_data);
}

void
ShaderStorageBuffer::Bind() const { GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, _ID)); }

void
ShaderStorageBuffer::BindBase() const { glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, _ID); }

void
ShaderStorageBuffer::Unbind() const { GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0)); }

void
ShaderStorageBuffer::Load(DynamicArray<glm::vec4>& _data) const
{
   GLCall(glBufferData(GL_SHADER_STORAGE_BUFFER, _data.size() * sizeof(glm::vec4), _data.data(), GL_STATIC_DRAW););
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
VertexArray::AddBuffer(const VertexBuffer& _vertex_buffer, const VertexAttributeLayout& _vertex_layout)
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
FrameBuffer::FrameBuffer(FrameBuffer&& _fbo) noexcept
{
   ID = _fbo.ID;
   _fbo.ID = 0;
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
FrameBuffer::AttachTexture(GLenum _attachement, GLuint _mapID) const
{
   GLCall(glFramebufferTexture(GL_FRAMEBUFFER, _attachement, _mapID, 0));
}

void
FrameBuffer::AttachTexture2D(GLenum _attachement, GLuint _mapID) const
{
   GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, _attachement, GL_TEXTURE_2D, _mapID, 0));
}

void
FrameBuffer::Draw(GLenum _mode) const { GLCall(glDrawBuffer(_mode)); }

void
FrameBuffer::Read(GLenum _mode) const { GLCall(glReadBuffer(_mode)); }

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
FrameBuffer::operator=(FrameBuffer&& _fbo) noexcept
{
   Delete();
   ID = _fbo.ID;
   _fbo.ID = 0;

   return *this;
}

}