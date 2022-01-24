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
template<PolytopeCategory category, std::size_t dim = 3>
struct Polygon : public StaticPolytope<category, dim>
{
  constexpr Polygon() { STATIC_ASSERT((isNPolytope<category, 2>()), "A polygon must be a 2-polytope.") }

  constexpr Polygon(const Float _side_length);

  template<class ...t_static_vector>
  constexpr Polygon(const t_static_vector& ..._vertices);
};

template<std::size_t dim>
struct Polygon<PolytopeCategory::Arbitrary2D, dim> : public DynamicPolytope<PolytopeCategory::Arbitrary2D, dim>
{
  Polygon(std::size_t _n_vertices, const Float _radius);

  template<class ...t_static_vector>
  Polygon(const t_static_vector& ..._vertices);
};

/***************************************************************************************************************************************************************
* Triangles
***************************************************************************************************************************************************************/
template<std::size_t dim = 3>
struct Triangle : public Polygon<PolytopeCategory::Triangular, dim>
{
  /** Arbitary triangle. */
  constexpr Triangle(const SVectorF<dim>& _v0, const SVectorF<dim>& _v1, const SVectorF<dim>& _v2);

  /** Arbitary triangle with a specified base length and height. */
  constexpr Triangle(const Float _length, const Float _height, const Float _apex_position = Half);

  /** Regular triangle. */
  constexpr Triangle(const Float _radius)
    : Polygon<PolytopeCategory::Triangular, dim>(3, _radius) {}
};

/***************************************************************************************************************************************************************
* Quadrilaterals
***************************************************************************************************************************************************************/
template<std::size_t dim = 3>
struct Quadrilateral : public Polygon<PolytopeCategory::Quadrilateral, dim>
{
  /** Arbitary quadrilateral. */
  constexpr Quadrilateral(const SVectorF<dim>& _v0, const SVectorF<dim>& _v1, const SVectorF<dim>& _v2, const SVectorF<dim>& _v3);

  /** Parallelogram with prescribed length and height. Defaults to a rectangle. */
  constexpr Quadrilateral(const Float _length, const Float _height, const Float _angle = 90.0);

  /** Regular Parallelogram with prescribed side length. Defaults to a square. */
  constexpr Quadrilateral(const Float _side_length, const Float _angle = 90.0)
    : Quadrilateral<dim>::Quadrilateral(_side_length, _side_length, _angle) {}
};

template<std::size_t dim = 3>
struct Rectangle : public Quadrilateral<dim>
{
  /** Rectangle with prescribed length and height. */
  constexpr Rectangle(const Float _length, const Float _height)
    : Quadrilateral<dim>(_length, _height) {}
};

template<std::size_t dim = 3>
struct Square : public Rectangle<dim>
{
  /** Square with prescribed side length centred at the origin. */
  constexpr Square(const Float _side_length)
    : Rectangle<dim>(_side_length, _side_length) {}
};

/***************************************************************************************************************************************************************
* Ellipses, Circles, Sectors, and Segments
***************************************************************************************************************************************************************/
template<std::size_t dim = 3>
struct Ellipse : public Polygon<PolytopeCategory::Arbitrary2D, dim>
{
  /** Ellipse with prescribed half-lengths centred at the origin. */
  Ellipse(const Float _x_radius, const Float _y_radius);
};

template<std::size_t dim = 3>
struct Circle : public Ellipse<dim>
{
  /** Circle with prescribed radius centred at the origin. */
  Circle(const Float _radius)
    : Ellipse<dim>(_radius, _radius) {}
};

}
}
