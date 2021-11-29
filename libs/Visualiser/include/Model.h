#pragma once

#include "../../../include/Global.h"
#include "../../DataContainers/include/Array.h"
#include "Buffers.h"
#include "Mesh.h"
#include "Material.h"
#include "Texture.h"

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace Apeiron {

class Model
{
public:
  Mesh Geometry;

  Model() : VAO(), VBO(), IBO(), SSBO() {}

  ~Model() { Delete(); }

  void Load();

  void Draw();

  void Delete();

private:
  VertexArray VAO;
  VertexBuffer VBO;
  IndexBuffer IBO;
  ShaderStorageBuffer SSBO;

  glm::mat4 ModelMatrix;
  DynamicArray<Material> Materials;
  DynamicArray<Texture> Textures;
};

}
