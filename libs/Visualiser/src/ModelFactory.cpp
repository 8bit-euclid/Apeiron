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

#include "../include/ModelFactory.h"

namespace aprn::vis {

/** 1D models
***************************************************************************************************************************************************************/
Model
ModelFactory::Segment(const SVector3<GLfloat>& v0, const SVector3<GLfloat>& v1)
{
   return Model();
}

template<class... svectors>
Model
ModelFactory::SegmentChain(const svectors&... vs)
{
   return Model();
}

/** 2D models
***************************************************************************************************************************************************************/
Model
ModelFactory::Triangle(const GLfloat length) { return Triangle(length, length * Sin(ThirdPi), Half); }

Model
ModelFactory::Triangle(const GLfloat length, const GLfloat height, const GLfloat apex_ratio)
{
   const GLfloat x = Half * static_cast<Real>(length);
   const GLfloat y = static_cast<Real>(height);
   const GLfloat apex_x = x * (Two * apex_ratio - One);

   return Triangle(SVector3<GLfloat>{-x, 0.0f, 0.0f}, SVector3<GLfloat>{x, 0.0f, 0.0f}, SVector3<GLfloat>{apex_x, y, 0.0f});
}

Model
ModelFactory::Triangle(const SVector3<GLfloat>& v0, const SVector3<GLfloat>& v1, const SVector3<GLfloat>& v2) { return Polygon(v0, v1, v2); }

Model
ModelFactory::Square(const GLfloat length) { return Rectangle(length, length); }

Model
ModelFactory::Rectangle(const GLfloat length, const GLfloat height)
{
   const GLfloat x = Half * length;
   const GLfloat y = Half * height;

   return Quadrilateral(SVector3<GLfloat>{-x, -y, 0.0f},
                        SVector3<GLfloat>{ x, -y, 0.0f},
                        SVector3<GLfloat>{ x,  y, 0.0f},
                        SVector3<GLfloat>{-x,  y, 0.0f});
}

Model
ModelFactory::ScreenQuad() { return Square(Two); }

Model
ModelFactory::Quadrilateral(const SVector3<GLfloat>& v0, const SVector3<GLfloat>& v1, const SVector3<GLfloat>& v2, const SVector3<GLfloat>& v3)
{
   Model part = Polygon(v0, v1, v2, v3);

   auto& mesh = part.Mesh_;

   // Set tangents
   FOR_EACH(vertex, mesh.Vertices) vertex.Tangent = glm::vec3(1.0f, 0.0f, 0.0f);

   // Set texture coordinates
   mesh.Vertices[0].TextureCoordinates = glm::vec2(0.0f, 0.0f);
   mesh.Vertices[1].TextureCoordinates = glm::vec2(1.0f, 0.0f);
   mesh.Vertices[2].TextureCoordinates = glm::vec2(1.0f, 1.0f);
   mesh.Vertices[3].TextureCoordinates = glm::vec2(0.0f, 1.0f);

   return part;
}

template<class... svectors>
Model
ModelFactory::Polygon(const svectors&... vs)
{
   Model part;
   part.Mesh_.Shading = ShadingType::Flat;

   auto& vertices = part.Mesh_.Vertices;
   auto& indices  = part.Mesh_.Indices;

   constexpr size_t n_vertices = sizeof...(svectors);
   vertices.resize(n_vertices);
   size_t i = 0;
   ((vertices[i++].Position = SVectorToGlmVec(vs)), ...);

   // TODO - extend to non-convex polygons as well!
   indices.resize(3 * (n_vertices - 2));
   FOR(i, n_vertices - 2)
   {
      indices[3 * i] = 0;
      FOR(j, 1, 3) indices[j + 3 * i] = i + j;
   }

   return part;
}

Model
ModelFactory::Arc(const GLfloat radius, const GLfloat angle)
{
   return Model();
}

Model
ModelFactory::Sector(const GLfloat radius, const GLfloat angle)
{
   return Model();
}

Model
ModelFactory::Circle(const GLfloat radius)
{
   return Model();
}

Model
ModelFactory::Ellipse(const GLfloat radius_x, const GLfloat radius_y)
{
   return Model();
}

Model
ModelFactory::Tetrahedron(const GLfloat length)
{
   const GLfloat width  = static_cast<Real>(length) * Sin(ThirdPi);
   const GLfloat height = Sqrt(iPow(length, 2) - iPow(TwoThird * width, 2));

   return Tetrahedron({-0.5f * length, 0.0f, width / 3.0f}, {0.5f * length, 0.0f, width / 3.0f}, {0.0f, 0.0f, -2.0f * width / 3.0f}, {0.0f, height, 0.0f});
}

Model
ModelFactory::Tetrahedron(const SVector3<GLfloat>& v0, const SVector3<GLfloat>& v1, const SVector3<GLfloat>& v2, const SVector3<GLfloat>& v3)
{
   Model part;
   part.Mesh_.Shading = ShadingType::Flat;

   auto& vertices = part.Mesh_.Vertices;
   auto& indices  = part.Mesh_.Indices;

   vertices.resize(4);
   vertices[0].Position = SVectorToGlmVec(v0);
   vertices[1].Position = SVectorToGlmVec(v1);
   vertices[2].Position = SVectorToGlmVec(v2);
   vertices[3].Position = SVectorToGlmVec(v3);

   indices.resize(12);
   // Face 0
   indices[0] = 0;
   indices[1] = 2;
   indices[2] = 1;

   // Face 1
   indices[3] = 0;
   indices[4] = 1;
   indices[5] = 3;

   // Face 2
   indices[6] = 0;
   indices[7] = 3;
   indices[8] = 2;

   // Face 3
   indices[9]  = 1;
   indices[10] = 2;
   indices[11] = 3;

   return part;
}

Model
ModelFactory::Cube(const GLfloat length) { return Cuboid(length, length, length); }

Model
ModelFactory::Cuboid(const GLfloat length, const GLfloat width, const GLfloat height)
{
   const GLfloat x = Half * static_cast<Real>(length);
   const GLfloat y = Half * static_cast<Real>(width);
   const GLfloat z = Half * static_cast<Real>(height);

   SVector3<GLfloat> v0{-x, -y,  z};
   SVector3<GLfloat> v1{ x, -y,  z};
   SVector3<GLfloat> v2{-x, -y, -z};
   SVector3<GLfloat> v3{ x, -y, -z};
   SVector3<GLfloat> v4{-x,  y,  z};
   SVector3<GLfloat> v5{ x,  y,  z};
   SVector3<GLfloat> v6{-x,  y, -z};
   SVector3<GLfloat> v7{ x,  y, -z};

   Model part;
   part.Mesh_.Shading = ShadingType::Flat;

   auto& vertices = part.Mesh_.Vertices;
   auto& indices  = part.Mesh_.Indices;

   vertices.resize(8);
   vertices[0].Position = SVectorToGlmVec(v0);
   vertices[1].Position = SVectorToGlmVec(v1);
   vertices[2].Position = SVectorToGlmVec(v2);
   vertices[3].Position = SVectorToGlmVec(v3);
   vertices[4].Position = SVectorToGlmVec(v4);
   vertices[5].Position = SVectorToGlmVec(v5);
   vertices[6].Position = SVectorToGlmVec(v6);
   vertices[7].Position = SVectorToGlmVec(v7);

   indices.resize(36);
   // Face 0
   indices[0] = 0;
   indices[1] = 1;
   indices[2] = 4;

   indices[3] = 4;
   indices[4] = 1;
   indices[5] = 5;

   // Face 1
   indices[6] = 3;
   indices[7] = 2;
   indices[8] = 6;

   indices[9] = 3;
   indices[10] = 6;
   indices[11] = 7;

   // Face 2
   indices[12] = 1;
   indices[13] = 0;
   indices[14] = 2;

   indices[15] = 2;
   indices[16] = 3;
   indices[17] = 1;

   // Face 3
   indices[18] = 4;
   indices[19] = 5;
   indices[20] = 7;

   indices[21] = 4;
   indices[22] = 7;
   indices[23] = 6;

   // Face 4
   indices[24] = 1;
   indices[25] = 3;
   indices[26] = 7;

   indices[27] = 1;
   indices[28] = 7;
   indices[29] = 5;

   // Face 5
   indices[30] = 0;
   indices[31] = 6;
   indices[32] = 2;

   indices[33] = 0;
   indices[34] = 4;
   indices[35] = 6;

   return part;
}

}
