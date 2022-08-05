#include "../include/ModelFactory.h"

namespace aprn::vis {

Model
ModelFactory::Segment(const SVector3<GLfloat>& _v0, const SVector3<GLfloat>& _v1)
{
   return Model();
}

template<class... svectors>
Model
ModelFactory::SegmentChain(const svectors& ..._v)
{
   return Model();
}

Model
ModelFactory::Triangle(GLfloat _length) { return Triangle(_length, _length * Sin(ThirdPi), Half); }

Model
ModelFactory::Triangle(GLfloat _length, GLfloat _height, GLfloat _apex_ratio)
{
   const GLfloat x = Half * static_cast<Float>(_length);
   const GLfloat y = static_cast<Float>(_height);
   const GLfloat apex_x = x * (Two * _apex_ratio - One);

   return Triangle(SVector3<GLfloat>{-x, Zero, Zero}, SVector3<GLfloat>{x, Zero, Zero}, SVector3<GLfloat>{apex_x, y, Zero});
}

Model
ModelFactory::Triangle(const SVector3<GLfloat>& _v0, const SVector3<GLfloat>& _v1, const SVector3<GLfloat>& _v2) { return Polygon(_v0, _v1, _v2); }

Model
ModelFactory::Square(GLfloat _length) { return Rectangle(_length, _length); }

Model
ModelFactory::Rectangle(GLfloat _length, GLfloat _height)
{
   const GLfloat x = Half * static_cast<Float>(_length);
   const GLfloat y = Half * static_cast<Float>(_height);

   return Quadrilateral(SVector3<GLfloat>{-x, -y, Zero}, SVector3<GLfloat>{x, -y, Zero}, SVector3<GLfloat>{x, y, Zero}, SVector3<GLfloat>{-x, y, Zero});
}

Model
ModelFactory::Quadrilateral(const SVector3<GLfloat>& _v0, const SVector3<GLfloat>& _v1, const SVector3<GLfloat>& _v2, const SVector3<GLfloat>& _v3)
{
   Model model = Polygon(_v0, _v1, _v2, _v3);

   // Set tangents
   FOR_EACH(vertex, model._Mesh.Vertices) vertex.Tangent = glm::vec3(1.0f, 0.0f, 0.0f);

   // Set texture coordinates
   model._Mesh.Vertices[0].TextureCoordinates = glm::vec2(0.0f, 0.0f);
   model._Mesh.Vertices[1].TextureCoordinates = glm::vec2(1.0f, 0.0f);
   model._Mesh.Vertices[2].TextureCoordinates = glm::vec2(1.0f, 1.0f);
   model._Mesh.Vertices[3].TextureCoordinates = glm::vec2(0.0f, 1.0f);

   return model;
}

template<class... svectors>
Model
ModelFactory::Polygon(const svectors& ..._v)
{
   Model model;
   model._Mesh.Shading = ShadingType::Flat;

   auto& vertices = model._Mesh.Vertices;
   auto& indices  = model._Mesh.Indices;

   constexpr size_t n_vertices = sizeof...(svectors);
   vertices.resize(n_vertices);
   size_t i = 0;
   ((vertices[i++].Position = SArrayToGlmVec(_v)), ...);

   // TODO - extend to non-convex polygons as well!
   indices.resize(3 * (n_vertices - 2));
   FOR(i, n_vertices - 2)
   {
      indices[3 * i] = 0;
      FOR(j, 1, 3) indices[j + 3 * i] = i + j;
   }

   return model;
}

Model
ModelFactory::Arc(GLfloat _radius, GLfloat angle)
{
   return Model();
}

Model
ModelFactory::Sector(GLfloat _radius, GLfloat angle)
{
   return Model();
}

Model
ModelFactory::Circle(GLfloat _radius)
{
   return Model();
}

Model
ModelFactory::Ellipse(GLfloat _radius_x, GLfloat _radius_y)
{
   return Model();
}

Model
ModelFactory::Tetrahedron(GLfloat _length)
{
   const GLfloat width  = static_cast<Float>(_length) * Sin(ThirdPi);
   const GLfloat height = Sqrt(iPow(_length, 2) - iPow(TwoThird * width, 2));

   return Tetrahedron({-0.5f * _length, 0.0f, width / 3.0f}, {0.5f * _length, 0.0f, width / 3.0f}, {0.0f, 0.0f, -2.0f * width / 3.0f}, {0.0f, height, 0.0f});
}

Model
ModelFactory::Tetrahedron(const SVector3<GLfloat>& _v0, const SVector3<GLfloat>& _v1, const SVector3<GLfloat>& _v2, const SVector3<GLfloat>& _v3)
{
   Model model;
   model._Mesh.Shading = ShadingType::Flat;

   auto& vertices = model._Mesh.Vertices;
   auto& indices  = model._Mesh.Indices;

   vertices.resize(4);
   vertices[0].Position = SArrayToGlmVec(_v0);
   vertices[1].Position = SArrayToGlmVec(_v1);
   vertices[2].Position = SArrayToGlmVec(_v2);
   vertices[3].Position = SArrayToGlmVec(_v3);

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

   return model;
}

Model
ModelFactory::Cube(GLfloat _length) { return Cuboid(_length, _length, _length); }

Model
ModelFactory::Cuboid(GLfloat _length, GLfloat _width, GLfloat _height)
{
   const GLfloat x = Half * static_cast<Float>(_length);
   const GLfloat y = Half * static_cast<Float>(_width);
   const GLfloat z = Half * static_cast<Float>(_height);

   SVector3<GLfloat> v0{-x, -y,  z};
   SVector3<GLfloat> v1{ x, -y,  z};
   SVector3<GLfloat> v2{-x, -y, -z};
   SVector3<GLfloat> v3{ x, -y, -z};
   SVector3<GLfloat> v4{-x,  y,  z};
   SVector3<GLfloat> v5{ x,  y,  z};
   SVector3<GLfloat> v6{-x,  y, -z};
   SVector3<GLfloat> v7{ x,  y, -z};

   Model model;
   model._Mesh.Shading = ShadingType::Flat;

   auto& vertices = model._Mesh.Vertices;
   auto& indices = model._Mesh.Indices;

   vertices.resize(8);
   vertices[0].Position = SArrayToGlmVec(v0);
   vertices[1].Position = SArrayToGlmVec(v1);
   vertices[2].Position = SArrayToGlmVec(v2);
   vertices[3].Position = SArrayToGlmVec(v3);
   vertices[4].Position = SArrayToGlmVec(v4);
   vertices[5].Position = SArrayToGlmVec(v5);
   vertices[6].Position = SArrayToGlmVec(v6);
   vertices[7].Position = SArrayToGlmVec(v7);

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

   return model;
}

}

