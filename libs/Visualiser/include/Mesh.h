#pragma once

#include "../../../include/Global.h"
#include "../../DataContainer/include/Array.h"

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace aprn::vis {

enum class ShadingType { Flat, Phong };

struct Vertex
{
   glm::vec3 Position;
   glm::vec3 Normal;
   glm::vec3 Tangent;
   glm::vec3 Colour{1.0f};
   glm::vec2 TextureCoordinates;
};

struct VertexAttribute
{
   VertexAttribute(const GLenum type, const GLuint _n_components, const GLboolean _is_normalised)
     : GLType(type), nComponents(_n_components), isNormalised(_is_normalised) {}

   GLenum    GLType;
   GLuint    nComponents;
   GLboolean isNormalised;
};

struct VertexAttributeLayout
{
   VertexAttributeLayout() : Stride(0) {};

   template<typename T>
   void AddAttribute(GLuint n_values);

   DynamicArray<VertexAttribute> Attributes;
   GLuint                        Stride;
};

class Mesh
{
 public:
   Mesh();

   void ComputeVertexNormals();

   inline const VertexAttributeLayout& GetVertexLayout() { return VertexLayout; }

 private:
   friend class Model;
   friend class ModelFactory;

   VertexAttributeLayout VertexLayout;
   DynamicArray<Vertex>  Vertices;
   DynamicArray<GLuint>  Indices;
   ShadingType           Shading{ShadingType::Flat};
};

}
