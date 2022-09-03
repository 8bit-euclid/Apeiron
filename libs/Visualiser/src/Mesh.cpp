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

#include "../include/Mesh.h"
#include "../include/GLTypes.h"

namespace aprn::vis {

template<typename T>
void VertexAttributeLayout::AddAttribute(const GLuint n_values)
{
  constexpr GLenum typeEnum = GLTypeEnum<T>();
  Attributes.emplace_back(typeEnum, n_values, (typeEnum == GL_UNSIGNED_BYTE ? GL_TRUE : GL_FALSE));
  Stride += n_values * GLTypeSize(typeEnum);
}

Mesh::Mesh()
{
  VertexLayout.AddAttribute<GLfloat>(3); // Position
  VertexLayout.AddAttribute<GLfloat>(3); // Normal
  VertexLayout.AddAttribute<GLfloat>(3); // Tangent
  VertexLayout.AddAttribute<GLfloat>(3); // Colour
  VertexLayout.AddAttribute<GLfloat>(2); // Texture coordinates

  DEBUG_ASSERT(VertexLayout.Stride == sizeof(Vertex), "Open if the vertex attribute layout has changed.")
}

void
Mesh::ComputeVertexNormals()
{
  if(Shading == ShadingType::Flat)
  {
    for(size_t it = 0; it < Indices.size(); it += 3)
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

    for(size_t it = 0; it < Indices.size(); it += 3)
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

  for(size_t it = 0; it < Indices.size(); it += 3)
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

