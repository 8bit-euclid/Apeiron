#include "../include/Categories.h"
#include "../../DataContainer/include/Array.h"

namespace Apeiron {
namespace Shapes {

template<PolytopeCategory category, std::size_t dim>
constexpr auto
GetPolytopeFaces(const StaticPolytope<category, dim>&)
{
  STATIC_ASSERT(isStaticPolytope<category>(), "The face vertices can only be determined for static polytopes.")

  constexpr auto n_faces = PolytopeFaceCount<category>();
  typedef StaticArray<std::size_t, PolytopeFaceVertexCount<category>()> face;
  typedef StaticArray<face, n_faces> faces;

  if(isNPolytope<category, 2>()) return faces([](std::size_t i){ return face{i, (i + 1) % n_faces}; });
  else
  {
    switch(category)
    {
      case PolytopeCategory::Tetrahedral: return faces{{1,0,2}, {0,1,3}, {1,2,3}, {2,0,3}};
      case PolytopeCategory::Cuboidal: return faces{{0,1,5,4}, {1,3,7,5}, {3,2,6,7}, {2,0,4,6}, {1,0,2,3}, {4,5,7,6}};
      case PolytopeCategory::Octahedral: return faces{{0,1,4}, {1,3,4}, {3,2,4}, {2,0,4}, {1,0,5}, {3,1,5}, {2,3,5}, {0,2,5}};
      case PolytopeCategory::Dodecahedral: return faces{};
      case PolytopeCategory::Icosahedral: return faces{};
      default: throw "Cannot determine the face vertices for the given polytope.";
    }
  }
}

}
}