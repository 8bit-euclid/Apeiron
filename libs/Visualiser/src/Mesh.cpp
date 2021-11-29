#include "../include/Mesh.h"
#include "../include/GLTypes.h"

namespace Apeiron {

template<typename t_data_type>
void VertexAttributeLayout::AddAttribute(const GLuint _n_values)
{
  constexpr GLenum typeEnum = GLTypeEnum<t_data_type>();
  Attributes.emplace_back(typeEnum, _n_values, (typeEnum == GL_UNSIGNED_BYTE ? GL_TRUE : GL_FALSE));
  Stride += _n_values * GLTypeSize(typeEnum);
}

Mesh::Mesh()
{
  VertexLayout.AddAttribute<GLfloat>(3); // Position
  VertexLayout.AddAttribute<GLfloat>(3); // Normal
  VertexLayout.AddAttribute<GLfloat>(3); // Colour
  VertexLayout.AddAttribute<GLfloat>(2); // Texture coordinates

  DEBUG_ASSERT(VertexLayout.Stride == sizeof(Vertex), "Check if the vertex attribute layout has changed.")
}

void Mesh::ComputeVertexNormals()
{
  // Zero all vertex normals.
  FOR_EACH(vertex, Vertices) vertex.Normal = glm::vec3(0.0, 0.0, 0.0);

  for(std::size_t it = 0; it < Indices.size(); it += 3)
  {
    // Compute the normal of the current triangle and update the normal at each vertex in the current triangle
    const GLuint iv0 = Indices[it];
    const GLuint iv1 = Indices[it + 1];
    const GLuint iv2 = Indices[it + 2];
    FOR(iv, 3) Vertices[Indices[it + iv]].Normal += glm::cross(Vertices[iv1].Position - Vertices[iv0].Position, Vertices[iv2].Position - Vertices[iv0].Position);
  }

  // Normalise all vertex normals.
  FOR_EACH(vertex, Vertices) vertex.Normal = glm::normalize(vertex.Normal);
}

VertexAttributeLayout Mesh::VertexLayout = VertexAttributeLayout();

}

