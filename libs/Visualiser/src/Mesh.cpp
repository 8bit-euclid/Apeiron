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
  : Shading(ShadingType::None)
{
  VertexLayout.AddAttribute<GLfloat>(3); // Position
  VertexLayout.AddAttribute<GLfloat>(3); // Normal
  VertexLayout.AddAttribute<GLfloat>(3); // Colour
  VertexLayout.AddAttribute<GLfloat>(2); // Texture coordinates

  DEBUG_ASSERT(VertexLayout.Stride == sizeof(Vertex), "Check if the vertex attribute layout has changed.")
}

void Mesh::ComputeVertexNormals()
{
  if(Shading == ShadingType::Flat)
  {
    for(std::size_t it = 0; it < Indices.size(); it += 3)
    {
      // Assign the normal of the current triangle to each vertex normal.
      const GLuint iv0 = Indices[it];
      const GLuint iv1 = Indices[it + 1];
      const GLuint iv2 = Indices[it + 2];
      const glm::vec3 face_normal = glm::cross(Vertices[iv1].Position - Vertices[iv0].Position, Vertices[iv2].Position - Vertices[iv0].Position);
      FOR(iv, 3) Vertices[Indices[it + iv]].Normal = face_normal;
    }
  }
  else if(Shading == ShadingType::Phong)
  {
    // Zero all vertex normals.
    FOR_EACH(vertex, Vertices) vertex.Normal = glm::vec3(0.0, 0.0, 0.0);

    for(std::size_t it = 0; it < Indices.size(); it += 3)
    {
      // Compute the normal of the current triangle and update the normal at each vertex in the current triangle
      const GLuint iv0 = Indices[it];
      const GLuint iv1 = Indices[it + 1];
      const GLuint iv2 = Indices[it + 2];
      const glm::vec3 face_normal = glm::cross(Vertices[iv1].Position - Vertices[iv0].Position, Vertices[iv2].Position - Vertices[iv0].Position);
      FOR(iv, 3) Vertices[Indices[it + iv]].Normal += face_normal;
    }

    // Normalise all vertex normals.
    FOR_EACH(vertex, Vertices) vertex.Normal = glm::normalize(vertex.Normal);
  }
  else EXIT("Unrecognised shading type prescribed.")

  if(Shading == ShadingType::Phong) FOR_EACH(vertex, Vertices) vertex.Normal = glm::vec3(0.0, 0.0, 0.0);

  for(std::size_t it = 0; it < Indices.size(); it += 3)
  {
    // Compute the normal of the current triangular face and update the normal at each vertex in the current triangle accordingly.
    const GLuint iv0 = Indices[it];
    const GLuint iv1 = Indices[it + 1];
    const GLuint iv2 = Indices[it + 2];
    glm::vec3 face_normal = glm::cross(Vertices[iv1].Position - Vertices[iv0].Position, Vertices[iv2].Position - Vertices[iv0].Position);
    if(Shading == ShadingType::Flat)
    {
      face_normal = glm::normalize(face_normal);
      FOR(iv, 3) Vertices[Indices[it + iv]].Normal = face_normal;
    }
    else if(Shading == ShadingType::Phong) FOR(iv, 3) Vertices[Indices[it + iv]].Normal += face_normal;
    else EXIT("Unrecognised shading type prescribed.")
  }

  if(Shading == ShadingType::Phong) FOR_EACH(vertex, Vertices) vertex.Normal = glm::normalize(vertex.Normal);
}

}

