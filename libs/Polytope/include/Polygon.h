/***************************************************************************************************************************************************************
* GPL-3.0 License
* Copyright (C) 2022 Niran A. Ilangakoon
*
* This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as
* published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
* of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program.
* If not, see <https://www.gnu.org/licenses/>.
***************************************************************************************************************************************************************/

#pragma once

//#include "../../../include/Global.h"
#include "../../DataContainer/include/Array.h"
#include "../../LinearAlgebra/include/Vector.h"
#include "Categories.h"
#include "Polytope.h"

namespace aprn::ptope {

/***************************************************************************************************************************************************************
* Static/Dynamic Polygons
***************************************************************************************************************************************************************/
template<PolytopeCategory cat, size_t dim = 3>
struct Polygon : public StaticPolytope<cat, dim>
{
  constexpr Polygon() { STATIC_ASSERT((isNPolytope<cat, 2>()), "A polygon must be a 2-polytope.") }

  constexpr Polygon(const Float _side_length);

  template<class... static_vector>
  constexpr Polygon(const static_vector&... vertices);
};

template<size_t dim>
struct Polygon<PolytopeCategory::Arbitrary2D, dim> : public DynamicPolytope<PolytopeCategory::Arbitrary2D, dim>
{
  Polygon(size_t _n_vertices, const Float _radius);

  template<class... static_vector>
  Polygon(const static_vector&... vertices);
};

/***************************************************************************************************************************************************************
* Triangles
***************************************************************************************************************************************************************/
template<size_t dim = 3>
struct Triangle : public Polygon<PolytopeCategory::Triangle, dim>
{
  /** Arbitary triangle. */
  constexpr Triangle(const SVectorF<dim>& _v0, const SVectorF<dim>& _v1, const SVectorF<dim>& _v2);

  /** Arbitary triangle with a specified base length and height. */
  constexpr Triangle(const Float _length, const Float height, const Float _apex_ratio);

  /** Regular triangle. */
  constexpr Triangle(const Float _radius)
    : Polygon<PolytopeCategory::Triangle, dim>(3, _radius) {}
};

/***************************************************************************************************************************************************************
* Quadrilaterals
***************************************************************************************************************************************************************/
template<size_t dim = 3>
struct Quadrilateral : public Polygon<PolytopeCategory::Quadrilateral, dim>
{
  /** Arbitary quadrilateral. */
  constexpr Quadrilateral(const SVectorF<dim>& _v0, const SVectorF<dim>& _v1, const SVectorF<dim>& _v2, const SVectorF<dim>& _v3);

  /** Parallelogram with prescribed length and height. Defaults to a rectangle. */
  constexpr Quadrilateral(const Float _length, const Float height, const Float angle = 90.0);

  /** Regular Parallelogram with prescribed side length. Defaults to a square. */
  constexpr Quadrilateral(const Float _side_length, const Float angle = 90.0)
    : Quadrilateral<dim>::Quadrilateral(_side_length, _side_length, angle) {}
};

template<size_t dim = 3>
struct Rectangle : public Quadrilateral<dim>
{
  /** Rectangle with prescribed length and height. */
  constexpr Rectangle(const Float _length, const Float height)
    : Quadrilateral<dim>(_length, height) {}
};

template<size_t dim = 3>
struct Square : public Rectangle<dim>
{
  /** Square with prescribed side length centred at the origin. */
  constexpr Square(const Float _side_length)
    : Rectangle<dim>(_side_length, _side_length) {}
};

}
