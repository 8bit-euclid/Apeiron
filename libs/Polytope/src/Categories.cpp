#include "../include/Categories.h"
#include "../../DataContainer/include/Array.h"

namespace Apeiron {
namespace Shapes {

template<PolytopeCategory t_category, std::size_t t_dimension>
constexpr auto
GetPolytopeFaces(const StaticPolytope<t_category, t_dimension>&)
{
  STATIC_ASSERT(isStaticPolytope<t_category>(), "The face vertices can only be determined for static polytopes.")

  constexpr auto n_faces = PolytopeFaceCount<t_category>();
  typedef StaticArray<std::size_t, PolytopeFaceVertexCount<t_category>()> face;
  typedef StaticArray<face, n_faces> faces;

  if(isNPolytope<t_category, 2>()) return faces([](std::size_t i){ return face{i, (i + 1) % n_faces}; });
  else
  {
    switch(t_category)
    {
      case PolytopeCategory::Tetrahedral: return faces{face{1,0,2}, face{0,1,3}, face{1,2,3}, face{2,0,3}};
      case PolytopeCategory::Cuboidal: return faces{face{0,1,5,4}, face{1,3,7,5}, face{3,2,6,7}, face{2,0,4,6}, face{1,0,2,3}, face{4,5,7,6}};
      case PolytopeCategory::Octahedral: return faces{face{0,1,4}, face{1,3,4}, face{3,2,4}, face{2,0,4}, face{1,0,5}, face{3,1,5}, face{2,3,5}, face{0,2,5}};
      case PolytopeCategory::Dodecahedral: return faces{};
      case PolytopeCategory::Icosahedral: return faces{};
      default: throw "Cannot determine the face vertices for the given polytope.";
    }
  }
}

}
}