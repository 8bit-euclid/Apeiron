#pragma once

#include "../../../include/Global.h"
#include "../../DataContainer/include/Array.h"
#include "../../LinearAlgebra/include/Vector.h"
#include "Categories.h"

namespace Apeiron {
namespace Shapes {

/***************************************************************************************************************************************************************
* Polytope Abstract Base Class
***************************************************************************************************************************************************************/
template<class derived, PolytopeCategory t_category, std::size_t t_dimension>
struct Polytope
{
  constexpr virtual ~Polytope() = 0;

  /** Vertices/Faces Access */
  constexpr auto& Vertices() { return Derived().Vertices; }

  constexpr const auto& Vertices() const { return Derived().Vertices; }

  constexpr auto& Faces() { return Derived().Faces; }

  constexpr const auto& Faces() const { return Derived().Faces; }

private:
  /** Derived Class Access */
  constexpr derived& Derived() noexcept { return static_cast<derived&>(*this); }

  constexpr const derived& Derived() const noexcept { return static_cast<const derived&>(*this); }
};

/***************************************************************************************************************************************************************
* Static Polytope Abstract Class
***************************************************************************************************************************************************************/
template<PolytopeCategory t_category, std::size_t t_dimension>
struct StaticPolytope : public Polytope<StaticPolytope<t_category, t_dimension>, t_category, t_dimension>
{
  constexpr StaticPolytope() { STATIC_ASSERT(isStaticPolytope<t_category>(), "The polytope's information must be known at compile time.") }

  constexpr virtual ~StaticPolytope() = 0;

  StaticArray<StaticArray<Float, t_dimension>, PolytopeVertexCount<t_category>()> Vertices;
  constexpr static auto Faces{GetPolytopeFaces<t_category, t_dimension>()};
};

/***************************************************************************************************************************************************************
* Dynamic Polytope Abstract Class
***************************************************************************************************************************************************************/
template<PolytopeCategory t_category, std::size_t t_dimension>
struct DynamicPolytope : public Polytope<DynamicPolytope<t_category, t_dimension>, t_category, t_dimension>
{
  DynamicPolytope() { STATIC_ASSERT(isDynamicPolytope<t_category>(), "Rather use StaticPolytope if the polytope's information is known at compile-time.") }

  virtual ~DynamicPolytope() = 0;

  DynamicArray<StaticArray<Float, t_dimension>> Vertices;
  DynamicArray<DynamicArray<std::size_t>> Faces;
};

}
}