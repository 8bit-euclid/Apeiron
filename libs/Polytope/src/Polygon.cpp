#include "../include/Polygon.h"
#include "../../Functional/include/Explicit.h"

namespace aprn {
namespace ptope {

/***************************************************************************************************************************************************************
* Static/Dynamic Polygons
***************************************************************************************************************************************************************/
template<PolytopeCategory cat, size_t dim>
constexpr Polygon<cat, dim>::Polygon(const Float _side_length)
{

}

template<PolytopeCategory cat, size_t dim>
template<class... static_vector>
constexpr Polygon<cat, dim>::Polygon(const static_vector&... _vertices)
{

}

template<size_t dim>
Polygon<PolytopeCategory::Arbitrary2D, dim>::Polygon(size_t _n_vertices, const Float _radius)
{

}

template<size_t dim>
template<class... static_vector>
Polygon<PolytopeCategory::Arbitrary2D, dim>::Polygon(const static_vector&... _vertices)
{

}

/***************************************************************************************************************************************************************
* Triangles
***************************************************************************************************************************************************************/
template<size_t dim>
constexpr Triangle<dim>::Triangle(const SVectorF<dim>& _v0, const SVectorF<dim>& _v1, const SVectorF<dim>& _v2)
{

}

template<size_t dim>
constexpr Triangle<dim>::Triangle(const Float _length, const Float _height, const Float _apex_ratio)
{

}

/***************************************************************************************************************************************************************
* Quadrilaterals
***************************************************************************************************************************************************************/
template<size_t dim>
constexpr Quadrilateral<dim>::Quadrilateral(const SVectorF<dim>& _v0, const SVectorF<dim>& _v1, const SVectorF<dim>& _v2, const SVectorF<dim>& _v3)
{

}

template<size_t dim>
constexpr Quadrilateral<dim>::Quadrilateral(const Float _length, const Float _height, const Float angle)
{

}

}
}
