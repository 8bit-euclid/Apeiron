#pragma once

#include "../../../include/Global.h"
#include "../../DataContainers/include/Array.h"

#include <GL/glew.h>
#include <glm/glm.hpp>

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
  glm::vec3 Colour;
  glm::vec2 TextureCoordinates;
};

struct VertexAttribute
{
  GLenum Type;
  GLuint nComponents;
  GLboolean isNormalised;

  VertexAttribute(const GLenum _type, const GLuint _n_components, const GLboolean _is_normalised)
    : Type(_type), nComponents(_n_components), isNormalised(_is_normalised) {}

  ~VertexAttribute() = default;
};

struct VertexAttributeLayout
{
  DynamicArray<VertexAttribute> Attributes;
  GLuint Stride;

  VertexAttributeLayout() : Stride(0) {};

  ~VertexAttributeLayout() = default;

  template<typename t_data_type>
  void AddAttribute(GLuint _n_values);
};

class Mesh
{
public:
  ShadingType Shading;
  DynamicArray<Vertex> Vertices;
  DynamicArray<GLuint> Indices;

  Mesh();

  ~Mesh() = default;

  void ComputeVertexNormals();

  const VertexAttributeLayout& GetVertexLayout() { return VertexLayout; }

private:
  VertexAttributeLayout VertexLayout;
};

}
