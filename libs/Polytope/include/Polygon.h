#pragma once

#include "../../../include/Global.h"
#include "../../DataContainers/include/Array.h"
#include "Categories.h"
#include "Polytope.h"

namespace Apeiron {
namespace Geometry {

/***************************************************************************************************************************************************************
* Static/Dynamic Polygons
***************************************************************************************************************************************************************/
template<PolytopeCategory t_category, std::size_t t_dimension = 3>
struct Polygon : public StaticPolytope<t_category, t_dimension>
{
  Polygon() { STATIC_ASSERT((isNPolytope<t_category, 2>()), "A polygon must be a 2-polytope.") }

  Polygon(const Float _side_length);

  template<class ...t_static_vector>
  Polygon(const t_static_vector& ..._vertices);
};

template<std::size_t t_dimension>
struct Polygon<PolytopeCategory::Arbitrary2D, t_dimension> : public DynamicPolytope<PolytopeCategory::Arbitrary2D, t_dimension>
{
  Polygon(std::size_t _n_vertices, const Float _side_length);

  template<class ...t_static_vector>
  Polygon(const t_static_vector& ..._vertices);
};

/***************************************************************************************************************************************************************
* Triangles
***************************************************************************************************************************************************************/
template<std::size_t t_dimension = 3>
struct Triangle : public Polygon<PolytopeCategory::Triangular, t_dimension>
{
  /** Arbitary triangle. */
  Triangle(const StaticArray<Float, t_dimension>& _v0, const StaticArray<Float, t_dimension>& _v1, const StaticArray<Float, t_dimension>& _v2);

  /** Regular triangle. */
  Triangle(const Float _side_length);

  /** Arbitary triangle with a specified base length and height. */
  Triangle(const Float _length, const Float _height, const Float _apex_position = Half);
};

/***************************************************************************************************************************************************************
* Quadrilaterals
***************************************************************************************************************************************************************/
template<std::size_t t_dimension = 3>
struct Quadrilateral : public Polygon<PolytopeCategory::Quadrilateral, t_dimension>
{
  Quadrilateral(const StaticArray<Float, t_dimension>& _v0, const StaticArray<Float, t_dimension>& _v1,
                const StaticArray<Float, t_dimension>& _v2, const StaticArray<Float, t_dimension>& _v3);

  Quadrilateral(const Float _length, const Float _height);

  Quadrilateral(const Float _side_length);

  Quadrilateral(const Float _length, const Float _height, const Float _angle);
};

template<std::size_t t_dimension = 3>
struct Rectangle : public Quadrilateral<t_dimension>
{
  Rectangle(const Float _length, const Float _height);
};

template<std::size_t t_dimension = 3>
struct Square : public Rectangle<t_dimension>
{
  Square(const Float _side_length);
};

}
}
