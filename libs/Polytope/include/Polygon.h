#pragma once

#include "../../../include/Global.h"
#include "../../DataContainers/include/Array.h"
#include "Categories.h"
#include "Polytope.h"

namespace Apeiron {
namespace Geometry {

/***************************************************************************************************************************************************************
* Arbitrary Static/Dynamic Polygons
***************************************************************************************************************************************************************/
template<std::size_t t_dimension = 3, PolytopeCategory t_category = PolytopeCategory::Arbitrary2D>
struct Polygon;

template<std::size_t t_dimension, PolytopeCategory t_category>
struct Polygon : public StaticPolytope<t_category, t_dimension>
{
  Polygon() { STATIC_ASSERT((isNPolytope<t_category, 2>()), "A polygon must be a 2-polytope.") }

  Polygon(const Float _side_length);

  template<class ...t_static_vector>
  Polygon(const t_static_vector &..._vertices);
};

template<std::size_t t_dimension>
struct Polygon<t_dimension, PolytopeCategory::Arbitrary2D> : public DynamicPolytope<PolytopeCategory::Arbitrary2D, t_dimension>
{
  Polygon(std::size_t _n_vertices, const Float _side_length);

  template<class ...t_static_vector>
  Polygon(const t_static_vector &..._vertices);
};

/***************************************************************************************************************************************************************
* Triangles
***************************************************************************************************************************************************************/
template<std::size_t t_dimension = 3>
struct Triangle : public Polygon<t_dimension, PolytopeCategory::Triangular>
{
  Triangle(const StaticArray<Float, t_dimension> &_v0, const StaticArray<Float, t_dimension> &_v1, const StaticArray<Float, t_dimension> &_v2);
};

template<std::size_t t_dimension = 3>
struct EquilateralTriangle : public Triangle<t_dimension>
{
  EquilateralTriangle(const Float _side_length);
};

template<std::size_t t_dimension = 3>
struct RightTriangle : public Triangle<t_dimension>
{
  RightTriangle(const Float _length, const Float _height, const bool _flip_horizontally = false);
};

template<std::size_t t_dimension = 3>
struct IsoscelesTriangle : public Triangle<t_dimension>
{
  IsoscelesTriangle(const Float _length, const Float _height);
};

/***************************************************************************************************************************************************************
* Quadrilaterals
***************************************************************************************************************************************************************/
template<std::size_t t_dimension = 3>
struct Quadrilateral : public Polygon<t_dimension, PolytopeCategory::Quadrilateral>
{
  Quadrilateral(const StaticArray<Float, t_dimension>& _v0, const StaticArray<Float, t_dimension>& _v1,
                const StaticArray<Float, t_dimension>& _v2, const StaticArray<Float, t_dimension>& _v3);
};

template<std::size_t t_dimension = 3>
struct Rectangle : public Quadrilateral<t_dimension>
{
  Rectangle(const Float _length, const Float _height);
};

/** Square ****************************************************************************************************************************************************/
template<std::size_t t_dimension = 3>
struct Square : public Rectangle<t_dimension>
{
  Square(const Float _side_length);
};

template<std::size_t t_dimension = 3>
struct Test : public Polygon<t_dimension>
{

};


}
}
