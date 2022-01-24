#pragma once

#include "../../../include/Global.h"
#include "../../DataContainer/include/Array.h"

#include <GL/glew.h>
#include <glm/glm.hpp>

//#pragma GCC diagnostic push
//#pragma GCC diagnostic ignored "-Wpedantic"
//#include "old_header.hpp"
//#pragma GCC diagnostic pop

namespace Apeiron {

enum class ShadingType
{
  Flat,
  Phong,
  None
};

struct Vertex
{
  glm::vec3 Position;
  glm::vec3 Normal;
  glm::vec3 Tangent;
  glm::vec3 Colour;
  glm::vec2 TextureCoordinates;
};

struct VertexAttribute
{
  GLenum GLType;
  GLuint nComponents;
  GLboolean isNormalised;

  VertexAttribute(const GLenum _type, const GLuint _n_components, const GLboolean _is_normalised)
    : GLType(_type), nComponents(_n_components), isNormalised(_is_normalised) {}

  ~VertexAttribute() = default;
};

struct VertexAttributeLayout
{
  DynamicArray<VertexAttribute> Attributes;
  GLuint Stride;

  VertexAttributeLayout() : Stride(0) {};

  ~VertexAttributeLayout() = default;

  template<typename T>
  void AddAttribute(GLuint _n_values);
};

class Mesh
{
public:
  DynamicArray<Vertex> Vertices;
  DynamicArray<GLuint> Indices;
  ShadingType Shading{ShadingType::None};

  Mesh();

  ~Mesh() = default;

  void ComputeVertexNormals();

  const VertexAttributeLayout& GetVertexLayout() { return VertexLayout; }

private:
  VertexAttributeLayout VertexLayout;
};

}
