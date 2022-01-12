#pragma once

#include "../../../include/Global.h"
#include "../../DataContainers/include/Array.h"
#include "Categories.h"

namespace Apeiron {
namespace Geometry {

/***************************************************************************************************************************************************************
* n-Polytope Abstract Base Class Definitions
***************************************************************************************************************************************************************/
template<PolytopeCategory t_category, std::size_t t_dimension>
struct Polytope
{
  virtual ~Polytope() = 0;
};

template<PolytopeCategory t_category, std::size_t t_dimension>
struct StaticPolytope : public Polytope<t_category, t_dimension>
{
  StaticPolytope() { STATIC_ASSERT(isStaticPolytope<t_category>(), "The polytope's information must be known at compile time.") }

  virtual ~StaticPolytope() = 0;

  StaticArray<StaticArray<Float, t_dimension>, GetPolytopeVertexCount<t_category>()> Vertices;
  StaticArray<StaticArray<std::size_t, GetPolytopeFaceVertexCount<t_category>()>, GetPolytopeFaceCount<t_category>()> Faces;
};

template<PolytopeCategory t_category, std::size_t t_dimension>
struct DynamicPolytope : public Polytope<t_category, t_dimension>
{
  DynamicPolytope() { STATIC_ASSERT(isDynamicPolytope<t_category>(), "Rather use StaticPolytope if the polytope's information is known at compile-time.") }

  virtual ~DynamicPolytope() = 0;

  DynamicArray<StaticArray<Float, t_dimension>> Vertices;
  DynamicArray<DynamicArray<std::size_t>> Faces;
};

}
}