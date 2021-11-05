#pragma once

#include <GL/glew.h>
#include "../../../include/Global.h"
#include "Buffers.h"
#include "Shader.h"

namespace Apeiron{

class Renderer
{
public:
  inline void Draw(const VertexArray& _vertex_array, const IndexBuffer& _index_buffer, const Shader& _shader) const
  {
    _shader.Bind();
    _vertex_array.Bind();
    _index_buffer.Bind();
    GLCall(glDrawElements(GL_TRIANGLES, _index_buffer.nIndices, GL_UNSIGNED_INT, nullptr));
  }

  inline void Clear() const { GLCall(glClear(GL_COLOR_BUFFER_BIT)); }
};

}
