#include "../include/Model.h"

namespace Apeiron {

void Model::Load()
{
  // Compute vertex normals
  Geometry.ComputeVertexNormals();

  // Load vertex and index buffer objects
  VAO.Bind();
  EBO.Init(Geometry.Indices);
  VBO.Init(Geometry.Vertices);

  // Add vertex buffer to vertex array object
  VAO.AddBuffer(VBO, Geometry.GetVertexLayout());
  VAO.Unbind();
}

void Model::Draw()
{
  VAO.Bind();
  EBO.Bind();

  GLCall(glDrawElements(GL_TRIANGLES, EBO.nIndices, GL_UNSIGNED_INT, nullptr));

  EBO.Unbind();
  VAO.Unbind();
}

void Model::Delete()
{
  VBO.Delete();
  VAO.Delete();
  EBO.Delete();
  EBO.nIndices = 0;
}

void Model::Reset()
{
  ModelMatrix = glm::mat4(1.0);
}

void Model::Scale(const glm::vec3& _factors)
{
  ModelMatrix = glm::scale(ModelMatrix, _factors);
}

void Model::Translate(const glm::vec3& _displacement)
{
  ModelMatrix = glm::translate(ModelMatrix, _displacement);
}

void Model::Rotate(const GLfloat _degree_angle, const glm::vec3& _axis)
{
  ModelMatrix = glm::rotate(ModelMatrix, static_cast<GLfloat>(ToRadians(_degree_angle)), _axis);
}

const glm::mat4& Model::GetModelMatrix() const
{
  return ModelMatrix;
}

}

