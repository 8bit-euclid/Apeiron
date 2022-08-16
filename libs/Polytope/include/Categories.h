#pragma once

#include "../../../include/Global.h"

namespace aprn {
namespace ptope {

/***************************************************************************************************************************************************************
* Forward Declarations
***************************************************************************************************************************************************************/
enum class PolytopeCategory;
template<PolytopeCategory cat, size_t dim> struct StaticPolytope;

/***************************************************************************************************************************************************************
* Polytope Categories
***************************************************************************************************************************************************************/
enum class PolytopeCategory
{
  // 2-Polytopes
  Triangle,
  Quadrilateral,
  Pentagon,
  Hexagon,
  Septagon,
  Octagon,
  Arbitrary2D,

  // 3-Polytopes
  Tetrahedron,
  Cuboid,
  Octahedron,
  Dodecahedron,
  Icosahedron,
  Arbitrary3D
};

/***************************************************************************************************************************************************************
* Functions for Determining Polytope Properties
***************************************************************************************************************************************************************/

/** Get the dimension of a given polytope category. */
template<PolytopeCategory cat>
constexpr size_t
PolytopeDimension()
{
  return cat == PolytopeCategory::Triangle      ||
         cat == PolytopeCategory::Quadrilateral ||
         cat == PolytopeCategory::Pentagon      ||
         cat == PolytopeCategory::Hexagon       ||
         cat == PolytopeCategory::Septagon      ||
         cat == PolytopeCategory::Octagon       ||
         cat == PolytopeCategory::Arbitrary2D ? 2 :

         cat == PolytopeCategory::Tetrahedron   ||
         cat == PolytopeCategory::Cuboid        ||
         cat == PolytopeCategory::Octahedron    ||
         cat == PolytopeCategory::Dodecahedron  ||
         cat == PolytopeCategory::Icosahedron   ||
         cat == PolytopeCategory::Arbitrary3D ? 3 :
         throw std::invalid_argument("The dimension cannot be determined for the given polytope category.");
}

/** Init if a polytope is static, i.e. is its information known at compile-time? */
template<PolytopeCategory cat>
constexpr bool
isStaticPolytope() { return cat != PolytopeCategory::Arbitrary2D && cat != PolytopeCategory::Arbitrary3D; }

/** Init if a polytope is dynamic, i.e. is its information not known at compile-time? */
template<PolytopeCategory cat>
constexpr bool
isDynamicPolytope() { return !isStaticPolytope<cat>(); }

/** Init of a polytope in a given category is an n-polytope. */
template<PolytopeCategory cat, size_t t_N>
constexpr bool
isNPolytope() { return PolytopeDimension<cat>() == t_N; }

/** Get the number of vertices for a given polytope category. */
template<PolytopeCategory cat>
constexpr size_t
PolytopeVertexCount()
{
  return cat == PolytopeCategory::Triangle      ?  3 :
         cat == PolytopeCategory::Quadrilateral ?  4 :
         cat == PolytopeCategory::Pentagon      ?  5 :
         cat == PolytopeCategory::Hexagon       ?  6 :
         cat == PolytopeCategory::Septagon      ?  7 :
         cat == PolytopeCategory::Octagon       ?  8 :

         cat == PolytopeCategory::Tetrahedron   ?  4 :
         cat == PolytopeCategory::Cuboid        ?  8 :
         cat == PolytopeCategory::Octahedron    ?  6 :
         cat == PolytopeCategory::Dodecahedron  ? 20 :
         cat == PolytopeCategory::Icosahedron   ? 12 :
         throw std::invalid_argument("The vertex count cannot be determined for the given polytope category.");
}

/** Get the number of faces for a given polytope category. */
template<PolytopeCategory cat>
constexpr size_t
PolytopeFaceCount()
{
  return PolytopeDimension<cat>() == 2 ? PolytopeVertexCount<cat>() :
         cat == PolytopeCategory::Tetrahedron  ?  4 :
         cat == PolytopeCategory::Cuboid       ?  6 :
         cat == PolytopeCategory::Octahedron   ?  8 :
         cat == PolytopeCategory::Dodecahedron ? 12 :
         cat == PolytopeCategory::Icosahedron  ? 20 :
         throw std::invalid_argument("The face count cannot be determined for the given polytope category.");
}

/** Get the number of faces for a given polytope category. */
template<PolytopeCategory cat>
constexpr size_t
PolytopeFaceVertexCount()
{
  return PolytopeDimension<cat>() == 2 ? 2 :
         cat == PolytopeCategory::Tetrahedron  ?  4 :
         cat == PolytopeCategory::Cuboid       ?  6 :
         cat == PolytopeCategory::Octahedron   ?  8 :
         cat == PolytopeCategory::Dodecahedron ? 12 :
         cat == PolytopeCategory::Icosahedron  ? 20 :
         throw std::invalid_argument("The face vertex count cannot be determined for the given polytope category.");
}

/***************************************************************************************************************************************************************
* Static Polytope Face-Vertex Connectivity (Gambit Neutral Format)
***************************************************************************************************************************************************************/
template<PolytopeCategory cat, size_t dim>
constexpr auto
GetPolytopeFaces(const StaticPolytope<cat, dim>& _polytope = StaticPolytope<cat, dim>());

}
}
