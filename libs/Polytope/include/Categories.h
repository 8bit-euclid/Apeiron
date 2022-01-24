#pragma once

#include "../../../include/Global.h"

namespace Apeiron {
namespace Shapes {

/***************************************************************************************************************************************************************
* Forward Declarations
***************************************************************************************************************************************************************/
enum class PolytopeCategory;
template<PolytopeCategory t_category, std::size_t t_dimension> struct StaticPolytope;

/***************************************************************************************************************************************************************
* Polytope Categories
***************************************************************************************************************************************************************/
enum class PolytopeCategory
{
  // 2-Polytopes
  Triangular,
  Quadrilateral,
  Pentagonal,
  Hexagonal,
  Septagonal,
  Octagonal,
  Arbitrary2D,

  // 3-Polytopes
  Tetrahedral,
  Cuboidal,
  Octahedral,
  Dodecahedral,
  Icosahedral,
  Arbitrary3D
};

/***************************************************************************************************************************************************************
* Functions for Determining Polytope Properties
***************************************************************************************************************************************************************/
/** Check if a polytope is considered static, i.e. is its information known at compile-time? */
template <PolytopeCategory t_category>
constexpr bool
isStaticPolytope() { return t_category != PolytopeCategory::Arbitrary2D && t_category != PolytopeCategory::Arbitrary3D; }

/** Check if a polytope is considered dynamic, i.e. is its information not known at compile-time? */
template <PolytopeCategory t_category>
constexpr bool
isDynamicPolytope() { return !isStaticPolytope<t_category>(); }

/** Get the number of vertices for a given polytope category. */
template <PolytopeCategory t_category>
constexpr std::size_t
PolytopeDimension()
{
  return (t_category == PolytopeCategory::Triangular ||
          t_category == PolytopeCategory::Quadrilateral ||
          t_category == PolytopeCategory::Pentagonal ||
          t_category == PolytopeCategory::Hexagonal ||
          t_category == PolytopeCategory::Septagonal ||
          t_category == PolytopeCategory::Octagonal ||
          t_category == PolytopeCategory::Arbitrary2D) ? 2 :

         (t_category == PolytopeCategory::Tetrahedral ||
          t_category == PolytopeCategory::Cuboidal ||
          t_category == PolytopeCategory::Octahedral ||
          t_category == PolytopeCategory::Dodecahedral ||
          t_category == PolytopeCategory::Icosahedral ||
          t_category == PolytopeCategory::Arbitrary3D) ? 3 :
         throw std::invalid_argument("The dimension cannot be determined for the given polytope category.");
}

/** Get the number of vertices for a given polytope category. */
template <PolytopeCategory t_category, std::size_t t_N>
constexpr bool
isNPolytope() { return PolytopeDimension<t_category>() == t_N; }

/** Get the number of vertices for a given polytope category. */
template <PolytopeCategory t_category>
constexpr std::size_t
PolytopeVertexCount()
{
  return t_category == PolytopeCategory::Triangular ? 3 :
         t_category == PolytopeCategory::Quadrilateral ? 4 :
         t_category == PolytopeCategory::Pentagonal ? 5 :
         t_category == PolytopeCategory::Hexagonal ? 6 :
         t_category == PolytopeCategory::Septagonal ? 7 :
         t_category == PolytopeCategory::Octagonal ? 8 :

         t_category == PolytopeCategory::Tetrahedral ? 4 :
         t_category == PolytopeCategory::Cuboidal ? 8 :
         t_category == PolytopeCategory::Octahedral ? 6 :
         t_category == PolytopeCategory::Dodecahedral ? 20 :
         t_category == PolytopeCategory::Icosahedral ? 12 :
         throw std::invalid_argument("The vertex count cannot be determined for the given polytope category.");
}

/** Get the number of faces for a given polytope category. */
template <PolytopeCategory t_category>
constexpr std::size_t
PolytopeFaceCount()
{
  return PolytopeDimension<t_category>() == 2 ? PolytopeVertexCount<t_category>() :
         t_category == PolytopeCategory::Tetrahedral ? 4 :
         t_category == PolytopeCategory::Cuboidal ? 6 :
         t_category == PolytopeCategory::Octahedral ? 8 :
         t_category == PolytopeCategory::Dodecahedral ? 12 :
         t_category == PolytopeCategory::Icosahedral ? 20 :
         throw std::invalid_argument("The face count cannot be determined for the given polytope category.");
}

/** Get the number of faces for a given polytope category. */
template <PolytopeCategory t_category>
constexpr std::size_t
PolytopeFaceVertexCount()
{
  return PolytopeDimension<t_category>() == 2 ? 2 :
         t_category == PolytopeCategory::Tetrahedral ? 4 :
         t_category == PolytopeCategory::Cuboidal ? 6 :
         t_category == PolytopeCategory::Octahedral ? 8 :
         t_category == PolytopeCategory::Dodecahedral ? 12 :
         t_category == PolytopeCategory::Icosahedral ? 20 :
         throw std::invalid_argument("The face vertex count cannot be determined for the given polytope category.");
}

/***************************************************************************************************************************************************************
* Static Polytope Face-Vertex Connectivity (Gambit Neutral Format)
***************************************************************************************************************************************************************/
template<PolytopeCategory t_category, std::size_t t_dimension>
constexpr auto
GetPolytopeFaces(const StaticPolytope<t_category, t_dimension>& _polytope = StaticPolytope<t_category, t_dimension>());

}
}
