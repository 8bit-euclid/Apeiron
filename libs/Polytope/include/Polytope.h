#pragma once

#include "../../../include/Global.h"
#include "../../DataContainer/include/Array.h"
#include "../../LinearAlgebra/include/Vector.h"
#include "Categories.h"

namespace aprn::ptope {

/***************************************************************************************************************************************************************
* Polytope Abstract Base Class
***************************************************************************************************************************************************************/
template<class derived, PolytopeCategory cat, size_t dim>
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
template<PolytopeCategory cat, size_t dim>
struct StaticPolytope : public Polytope<StaticPolytope<cat, dim>, cat, dim>
{
   constexpr StaticPolytope() { STATIC_ASSERT(isStaticPolytope<cat>(), "The polytope's information must be known at compile time.") }

   virtual constexpr ~StaticPolytope() = 0;

   StaticArray<SVectorF<dim>, PolytopeVertexCount<cat>()> Vertices;
   constexpr static auto Faces{GetPolytopeFaces<cat, dim>()};
};

/***************************************************************************************************************************************************************
* Dynamic Polytope Abstract Class
***************************************************************************************************************************************************************/
template<PolytopeCategory cat, size_t dim>
struct DynamicPolytope : public Polytope<DynamicPolytope<cat, dim>, cat, dim>
{
   DynamicPolytope() { STATIC_ASSERT(isDynamicPolytope<cat>(), "Rather use StaticPolytope if the polytope's information is known at compile-time.") }

   virtual ~DynamicPolytope() = 0;

   DynamicArray<SVectorF<dim>> Vertices;
   DynamicArray<DynamicArray<size_t>> Faces;
};

}