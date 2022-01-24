#pragma once

#include "../../../include/Global.h"
#include "../../DataContainer/include/Array.h"
#include "../../LinearAlgebra/include/Vector.h"
#include "Categories.h"
#include "Polytope.h"

namespace Apeiron {
namespace Shapes {

/***************************************************************************************************************************************************************
* Static/Dynamic Polygons
***************************************************************************************************************************************************************/
template<PolytopeCategory t_category, std::size_t t_dimension = 3>
struct Polygon : public StaticPolytope<t_category, t_dimension>
{
  constexpr Polygon() { STATIC_ASSERT((isNPolytope<t_category, 2>()), "A polygon must be a 2-polytope.") }

  constexpr Polygon(const Float _side_length);

  template<class ...t_static_vector>
  constexpr Polygon(const t_static_vector& ..._vertices);
};

template<std::size_t t_dimension>
struct Polygon<PolytopeCategory::Arbitrary2D, t_dimension> : public DynamicPolytope<PolytopeCategory::Arbitrary2D, t_dimension>
{
  Polygon(std::size_t _n_vertices, const Float _radius);

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
  constexpr Triangle(const StaticArray<Float, t_dimension>& _v0, const StaticArray<Float, t_dimension>& _v1, const StaticArray<Float, t_dimension>& _v2);

  /** Arbitary triangle with a specified base length and height. */
  constexpr Triangle(const Float _length, const Float _height, const Float _apex_position = Half);

  /** Regular triangle. */
  constexpr Triangle(const Float _radius)
    : Polygon<PolytopeCategory::Triangular, t_dimension>(3, _radius) {}
};

/***************************************************************************************************************************************************************
* Quadrilaterals
***************************************************************************************************************************************************************/
template<std::size_t t_dimension = 3>
struct Quadrilateral : public Polygon<PolytopeCategory::Quadrilateral, t_dimension>
{
  /** Arbitary quadrilateral. */
  constexpr Quadrilateral(const StaticArray<Float, t_dimension>& _v0, const StaticArray<Float, t_dimension>& _v1,
                          const StaticArray<Float, t_dimension>& _v2, const StaticArray<Float, t_dimension>& _v3);

  /** Parallelogram with prescribed length and height. Defaults to a rectangle. */
  constexpr Quadrilateral(const Float _length, const Float _height, const Float _angle = 90.0);

  /** Regular Parallelogram with prescribed side length. Defaults to a square. */
  constexpr Quadrilateral(const Float _side_length, const Float _angle = 90.0)
    : Quadrilateral<t_dimension>::Quadrilateral(_side_length, _side_length, _angle) {}
};

template<std::size_t t_dimension = 3>
struct Rectangle : public Quadrilateral<t_dimension>
{
  /** Rectangle with prescribed length and height. */
  constexpr Rectangle(const Float _length, const Float _height)
    : Quadrilateral<t_dimension>(_length, _height) {}
};

template<std::size_t t_dimension = 3>
struct Square : public Rectangle<t_dimension>
{
  /** Square with prescribed side length centred at the origin. */
  constexpr Square(const Float _side_length)
    : Rectangle<t_dimension>(_side_length, _side_length) {}
};

/***************************************************************************************************************************************************************
* Ellipses, Circles, Sectors, and Segments
***************************************************************************************************************************************************************/
template<std::size_t t_dimension = 3>
struct Ellipse : public Polygon<PolytopeCategory::Arbitrary2D, t_dimension>
{
  /** Ellipse with prescribed half-lengths centred at the origin. */
  Ellipse(const Float _x_radius, const Float _y_radius);
};

template<std::size_t t_dimension = 3>
struct Circle : public Ellipse<t_dimension>
{
  /** Circle with prescribed radius centred at the origin. */
  Circle(const Float _radius)
    : Ellipse<t_dimension>(_radius, _radius) {}
};

}
}
