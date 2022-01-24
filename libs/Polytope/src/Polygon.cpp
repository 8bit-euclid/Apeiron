#include "../include/Polygon.h"
#include "../../Functional/include/Explicit.h"

namespace Apeiron {
namespace Shapes {

/***************************************************************************************************************************************************************
* Static/Dynamic Polygons
***************************************************************************************************************************************************************/
template<PolytopeCategory t_category, std::size_t t_dimension>
constexpr Polygon<t_category, t_dimension>::Polygon(const Float _side_length)
{

}

template<PolytopeCategory t_category, std::size_t t_dimension>
template<class... t_static_vector>
constexpr Polygon<t_category, t_dimension>::Polygon(const t_static_vector &... _vertices)
{

}

template<std::size_t t_dimension>
Polygon<PolytopeCategory::Arbitrary2D, t_dimension>::Polygon(std::size_t _n_vertices, const Float _radius)
{

}

/***************************************************************************************************************************************************************
* Triangles
***************************************************************************************************************************************************************/
template<std::size_t t_dimension>
constexpr Triangle<t_dimension>::Triangle(const StaticArray<Float, t_dimension> &_v0, const StaticArray<Float, t_dimension> &_v1,
                                          const StaticArray<Float, t_dimension> &_v2)
{

}

template<std::size_t t_dimension>
constexpr Triangle<t_dimension>::Triangle(const Float _length, const Float _height, const Float _apex_position)
{

}

/***************************************************************************************************************************************************************
* Quadrilaterals
***************************************************************************************************************************************************************/
template<std::size_t t_dimension>
constexpr Quadrilateral<t_dimension>::Quadrilateral(const StaticArray<Float, t_dimension> &_v0, const StaticArray<Float, t_dimension> &_v1,
                                                    const StaticArray<Float, t_dimension> &_v2, const StaticArray<Float, t_dimension> &_v3)
{

}

template<std::size_t t_dimension>
constexpr Quadrilateral<t_dimension>::Quadrilateral(const Float _length, const Float _height, const Float _angle)
{

}

/***************************************************************************************************************************************************************
* Circles/Ellipses
***************************************************************************************************************************************************************/
template<std::size_t t_dimension>
Ellipse<t_dimension>::Ellipse(const Float _x_radius, const Float _y_radius)
{

}

}
}
