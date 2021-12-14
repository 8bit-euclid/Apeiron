#include "../include/Geometry.h"

namespace Apeiron {

void CreateSquare(Model& _model, const GLfloat _length)
{
  CreateRectangle(_model, _length, _length);
}

void CreateRectangle(Model& _model, const GLfloat _length, const GLfloat _width)
{
  CreatePolygon(_model, StaticArray<GLfloat, 3>{-0.5f*_length, 0.0f, 0.5f*_width}, StaticArray<GLfloat, 3>{0.5f*_length, 0.0f, 0.5f*_width},
                        StaticArray<GLfloat, 3>{0.5f*_length, 0.0f, -0.5f*_width}, StaticArray<GLfloat, 3>{-0.5f*_length, 0.0f, -0.5f*_width});
}

template <class ...t_static_vector>
void CreatePolygon(Model& _model, const t_static_vector& ..._v)
{
  _model.Geometry.Shading = ShadingType::Flat;

  DynamicArray<Vertex>& vertices = _model.Geometry.Vertices;
  DynamicArray<GLuint>& indices = _model.Geometry.Indices;

  constexpr size_t n_vertices = sizeof...(t_static_vector);
  vertices.resize(n_vertices);
  size_t i = 0;
  ((vertices[i++].Position = ConvertStaticArrayToGlmVec(_v)), ...);

  // TODO - extend to non-convex polygons also.
  indices.resize(3 * (n_vertices - 2));
  FOR(i, n_vertices - 2)
  {
    indices[3*i] = 0;
    FOR(j, 1, 3) indices[j + 3*i] = i + j;
  }

  _model.Load();
}

void CreateTetrahedron(Model& _model, GLfloat _length)
{
  const GLfloat width = _length * std::sin(ThirdPi);
  const GLfloat height = Sqrt(iPow(_length, 2) - iPow(TwoThird * width, 2));
  CreateTetrahedron(_model, {-0.5f*_length, 0.0f, width/3.0f}, {0.5f*_length, 0.0f, width/3.0f}, {0.0f, 0.0f, -2.0f*width/3.0f}, {0.0f, height, 0.0f});
}

void CreateTetrahedron(Model& _model, const StaticArray<GLfloat, 3>& _v0, const StaticArray<GLfloat, 3>& _v1, const StaticArray<GLfloat, 3>& _v2,
                       const StaticArray<GLfloat, 3>& _v3)
{
  _model.Geometry.Shading = ShadingType::Flat;

  DynamicArray<Vertex>& vertices0 = _model.Geometry.Vertices;
  DynamicArray<GLuint>& indices0 = _model.Geometry.Indices;

  vertices0.resize(4);
  vertices0[0].Position = ConvertStaticArrayToGlmVec(_v0);
  vertices0[1].Position = ConvertStaticArrayToGlmVec(_v1);
  vertices0[2].Position = ConvertStaticArrayToGlmVec(_v2);
  vertices0[3].Position = ConvertStaticArrayToGlmVec(_v3);

  indices0.resize(12);
  indices0[0] = 0;
  indices0[1] = 2;
  indices0[2] = 1;

  indices0[3] = 0;
  indices0[4] = 1;
  indices0[5] = 3;

  indices0[6] = 0;
  indices0[7] = 3;
  indices0[8] = 2;

  indices0[9] = 1;
  indices0[10] = 2;
  indices0[11] = 3;

  _model.Load();
}

void CreateCube(Model& _model, GLfloat _length)
{
  StaticArray<GLfloat, 3> v0{-0.5f*_length, -0.5f*_length, 0.5f*_length};
  StaticArray<GLfloat, 3> v1{0.5f*_length, -0.5f*_length, 0.5f*_length};
  StaticArray<GLfloat, 3> v2{-0.5f*_length, -0.5f*_length, -0.5f*_length};
  StaticArray<GLfloat, 3> v3{0.5f*_length, -0.5f*_length, -0.5f*_length};
  StaticArray<GLfloat, 3> v4{-0.5f*_length, 0.5f*_length, 0.5f*_length};
  StaticArray<GLfloat, 3> v5{0.5f*_length, 0.5f*_length, 0.5f*_length};
  StaticArray<GLfloat, 3> v6{-0.5f*_length, 0.5f*_length, -0.5f*_length};
  StaticArray<GLfloat, 3> v7{0.5f*_length, 0.5f*_length, -0.5f*_length};

  _model.Geometry.Shading = ShadingType::Flat;

  DynamicArray<Vertex>& vertices0 = _model.Geometry.Vertices;
  DynamicArray<GLuint>& indices0 = _model.Geometry.Indices;

  vertices0.resize(8);
  vertices0[0].Position = ConvertStaticArrayToGlmVec(v0);
  vertices0[1].Position = ConvertStaticArrayToGlmVec(v1);
  vertices0[2].Position = ConvertStaticArrayToGlmVec(v2);
  vertices0[3].Position = ConvertStaticArrayToGlmVec(v3);
  vertices0[4].Position = ConvertStaticArrayToGlmVec(v4);
  vertices0[5].Position = ConvertStaticArrayToGlmVec(v5);
  vertices0[6].Position = ConvertStaticArrayToGlmVec(v6);
  vertices0[7].Position = ConvertStaticArrayToGlmVec(v7);

  indices0.resize(36);
  indices0[0] = 0;
  indices0[1] = 1;
  indices0[2] = 4;

  indices0[3] = 4;
  indices0[4] = 1;
  indices0[5] = 5;

  indices0[6] = 3;
  indices0[7] = 2;
  indices0[8] = 6;

  indices0[9] = 3;
  indices0[10] = 6;
  indices0[11] = 7;

  indices0[12] = 1;
  indices0[13] = 0;
  indices0[14] = 2;

  indices0[15] = 2;
  indices0[16] = 3;
  indices0[17] = 1;

  indices0[18] = 4;
  indices0[19] = 5;
  indices0[20] = 7;

  indices0[21] = 4;
  indices0[22] = 7;
  indices0[23] = 6;

  indices0[24] = 1;
  indices0[25] = 3;
  indices0[26] = 7;

  indices0[27] = 1;
  indices0[28] = 7;
  indices0[29] = 5;

  indices0[30] = 0;
  indices0[31] = 6;
  indices0[32] = 2;

  indices0[33] = 0;
  indices0[34] = 4;
  indices0[35] = 6;

  _model.Load();
}

}

