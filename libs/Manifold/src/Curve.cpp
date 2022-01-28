#include "../include/Curve.h"

namespace Apeiron {
namespace Manifold {

/***************************************************************************************************************************************************************
* Linear/Piecewise Linear Curves
***************************************************************************************************************************************************************/
template<std::size_t ambient_dim>
Line<ambient_dim>::Line(const SVectorF<ambient_dim>& _unit_direction, const SVectorF<ambient_dim>& _coordinate)
  : Coordinate0(_coordinate), Direction(_unit_direction)
{
//  STATIC_ASSERT(, "The direction vector must be of unit length.")
}

template<std::size_t ambient_dim>
constexpr SVectorF<ambient_dim>
Line<ambient_dim>::ComputePoint(const SVectorF1& _t) { return Coordinate0 + _t[0] * Direction; }

template<std::size_t ambient_dim>
constexpr SVectorF<ambient_dim> Line<ambient_dim>::ComputeTangent(const SVectorF1& _t) { return Direction; }

template<std::size_t ambient_dim>
constexpr SVectorF<ambient_dim> Line<ambient_dim>::ComputeBitangent(const SVectorF1& _t)
{
  throw("TODO");
//  return Direction;
}

template<std::size_t ambient_dim>
constexpr SVectorF<ambient_dim> Line<ambient_dim>::ComputeNormal(const SVectorF1& _t)
{
  throw("TODO");
//  return Direction;
}

template<std::size_t ambient_dim>
Ray<ambient_dim>::Ray(const SVectorF<ambient_dim>& _unit_direction, const SVectorF<ambient_dim>& _start)
  : Line<ambient_dim>::Line(_unit_direction, _start) {}

//template<std::size_t ambient_dim>
//Segment<ambient_dim>::Segment(const SVectorF<ambient_dim>& _start, const SVectorF<ambient_dim>& _end):Line(_start, _end)
//{
//
//}

/***************************************************************************************************************************************************************
* Elliptical/Circular Curves
***************************************************************************************************************************************************************/


}
}