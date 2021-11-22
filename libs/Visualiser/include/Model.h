#pragma once

#include "../../../include/Global.h"
#include "../../DataContainers/include/Array.h"
#include "Buffers.h"
#include "Texture.h"

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace Apeiron {

class Model
{
public:
  Model(const DynamicArray<Vertex>& _vertices, const DynamicArray<GLuint>& _indices);

  ~Model();

  void Draw();

  void Delete();

private:
  VertexArray VAO;
  VertexBuffer VBO;
  IndexBuffer IBO;
//  ShaderStorageBuffer SSBO;

  glm::mat4 ModelMatrix;
  DynamicArray<Texture> Textures;
};

}
