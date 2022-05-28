#include "../include/Categories.h"
#include "../../DataContainer/include/Array.h"

namespace aprn {
namespace ptope {

template<PolytopeCategory cat, size_t dim>
constexpr auto
GetPolytopeFaces(const StaticPolytope<cat, dim>&)
{
  STATIC_ASSERT(isStaticPolytope<cat>(), "The face vertices can only be determined for static polytopes.")

  constexpr auto n_faces = PolytopeFaceCount<cat>();
  typedef StaticArray<size_t, PolytopeFaceVertexCount<cat>()> face;
  typedef StaticArray<face, n_faces> faces;

  if(isNPolytope<cat, 2>()) return faces([](size_t i){ return face{i, (i + 1) % n_faces}; });
  else
  {
    switch(cat)
    {
      case PolytopeCategory::Tetrahedron: return faces{{1, 0, 2}, {0, 1, 3}, {1, 2, 3}, {2, 0, 3}};
      case PolytopeCategory::Cuboid: return faces{{0, 1, 5, 4}, {1, 3, 7, 5}, {3, 2, 6, 7}, {2, 0, 4, 6}, {1, 0, 2, 3}, {4, 5, 7, 6}};
      case PolytopeCategory::Octahedron: return faces{{0, 1, 4}, {1, 3, 4}, {3, 2, 4}, {2, 0, 4}, {1, 0, 5}, {3, 1, 5}, {2, 3, 5}, {0, 2, 5}};
      case PolytopeCategory::Dodecahedron: return faces{};
      case PolytopeCategory::Icosahedron: return faces{};
      default: throw "Cannot determine the face vertices for the given polytope.";
    }
  }
}

}
}