#pragma once

#include "../../../include/Global.h"

namespace Apeiron {
namespace Shapes {

/***************************************************************************************************************************************************************
* Forward Declarations
***************************************************************************************************************************************************************/
enum class PolytopeCategory;
template<PolytopeCategory category, std::size_t dim> struct StaticPolytope;

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
template<PolytopeCategory category>
constexpr bool
isStaticPolytope() { return category != PolytopeCategory::Arbitrary2D && category != PolytopeCategory::Arbitrary3D; }

/** Check if a polytope is considered dynamic, i.e. is its information not known at compile-time? */
template<PolytopeCategory category>
constexpr bool
isDynamicPolytope() { return !isStaticPolytope<category>(); }

/** Get the number of vertices for a given polytope category. */
template<PolytopeCategory category>
constexpr std::size_t
PolytopeDimension()
{
  return (category == PolytopeCategory::Triangular ||
          category == PolytopeCategory::Quadrilateral ||
          category == PolytopeCategory::Pentagonal ||
          category == PolytopeCategory::Hexagonal ||
          category == PolytopeCategory::Septagonal ||
          category == PolytopeCategory::Octagonal ||
          category == PolytopeCategory::Arbitrary2D) ? 2 :

         (category == PolytopeCategory::Tetrahedral ||
          category == PolytopeCategory::Cuboidal ||
          category == PolytopeCategory::Octahedral ||
          category == PolytopeCategory::Dodecahedral ||
          category == PolytopeCategory::Icosahedral ||
          category == PolytopeCategory::Arbitrary3D) ? 3 :
         throw std::invalid_argument("The dimension cannot be determined for the given polytope category.");
}

/** Get the number of vertices for a given polytope category. */
template<PolytopeCategory category, std::size_t t_N>
constexpr bool
isNPolytope() { return PolytopeDimension<category>() == t_N; }

/** Get the number of vertices for a given polytope category. */
template<PolytopeCategory category>
constexpr std::size_t
PolytopeVertexCount()
{
  return category == PolytopeCategory::Triangular ? 3 :
         category == PolytopeCategory::Quadrilateral ? 4 :
         category == PolytopeCategory::Pentagonal ? 5 :
         category == PolytopeCategory::Hexagonal ? 6 :
         category == PolytopeCategory::Septagonal ? 7 :
         category == PolytopeCategory::Octagonal ? 8 :

         category == PolytopeCategory::Tetrahedral ? 4 :
         category == PolytopeCategory::Cuboidal ? 8 :
         category == PolytopeCategory::Octahedral ? 6 :
         category == PolytopeCategory::Dodecahedral ? 20 :
         category == PolytopeCategory::Icosahedral ? 12 :
         throw std::invalid_argument("The vertex count cannot be determined for the given polytope category.");
}

/** Get the number of faces for a given polytope category. */
template<PolytopeCategory category>
constexpr std::size_t
PolytopeFaceCount()
{
  return PolytopeDimension<category>() == 2 ? PolytopeVertexCount<category>() :
         category == PolytopeCategory::Tetrahedral ? 4 :
         category == PolytopeCategory::Cuboidal ? 6 :
         category == PolytopeCategory::Octahedral ? 8 :
         category == PolytopeCategory::Dodecahedral ? 12 :
         category == PolytopeCategory::Icosahedral ? 20 :
         throw std::invalid_argument("The face count cannot be determined for the given polytope category.");
}

/** Get the number of faces for a given polytope category. */
template<PolytopeCategory category>
constexpr std::size_t
PolytopeFaceVertexCount()
{
  return PolytopeDimension<category>() == 2 ? 2 :
         category == PolytopeCategory::Tetrahedral ? 4 :
         category == PolytopeCategory::Cuboidal ? 6 :
         category == PolytopeCategory::Octahedral ? 8 :
         category == PolytopeCategory::Dodecahedral ? 12 :
         category == PolytopeCategory::Icosahedral ? 20 :
         throw std::invalid_argument("The face vertex count cannot be determined for the given polytope category.");
}

/***************************************************************************************************************************************************************
* Static Polytope Face-Vertex Connectivity (Gambit Neutral Format)
***************************************************************************************************************************************************************/
template<PolytopeCategory category, std::size_t dim>
constexpr auto
GetPolytopeFaces(const StaticPolytope<category, dim>& _polytope = StaticPolytope<category, dim>());

}
}
