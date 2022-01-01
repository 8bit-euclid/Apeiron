#pragma once

#include "../../../include/Global.h"
#include "../../DataContainers/include/Array.h"
#include "Buffers.h"
#include "Mesh.h"
#include "Material.h"
#include "Texture.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Apeiron {

class Model
{
public:
  Mesh Geometry;

  Model()
    : VAO(), VBO(), EBO(), SSBO() {}

  ~Model() { Delete(); }

  void Load();

  void Draw();

  void Delete();

  void Reset();

  void Scale(const glm::vec3& _factors);

  void Translate(const glm::vec3& _displacement);

  void Rotate(const GLfloat _degree_angle, const glm::vec3& _axis);

  const glm::mat4& GetModelMatrix() const;

private:
  VertexArray VAO;
  VertexBuffer VBO;
  IndexBuffer EBO;
  ShaderStorageBuffer SSBO;

  glm::mat4 ModelMatrix{1.0f};
};

}
