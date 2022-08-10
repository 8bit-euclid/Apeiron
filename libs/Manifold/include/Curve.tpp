#include "LinearAlgebra/include/VectorOperations.h"

namespace aprn::mnfld {

/***************************************************************************************************************************************************************
* Linear/Piecewise Linear Curves
***************************************************************************************************************************************************************/

/** Line
***************************************************************************************************************************************************************/
template<size_t ambient_dim>
constexpr Line<ambient_dim>::Line(const SVectorF<ambient_dim>& direction, const SVectorF<ambient_dim>& coordinate)
  : Direction(direction), Coordinate0(coordinate), DirectionMagnitude(Magnitude(Direction)), Normaliser(One/DirectionMagnitude) {}

template<size_t ambient_dim>
constexpr SVectorF<ambient_dim>
Line<ambient_dim>::ComputePoint(const SVectorF1& t) { return Coordinate0 + t[0] * (isUnitSpeed ? Normaliser : One) * Direction; }

template<size_t ambient_dim>
constexpr SVectorF<ambient_dim>
Line<ambient_dim>::ComputeTangent(const SVectorF1& t) { return (isUnitSpeed ? Normaliser : One) * Direction; }

template<size_t ambient_dim>
constexpr SVectorF<ambient_dim>
Line<ambient_dim>::ComputeBitangent(const SVectorF1& t)
{
  throw("TODO");
//  return Direction;
}

template<size_t ambient_dim>
constexpr SVectorF<ambient_dim>
Line<ambient_dim>::ComputeNormal(const SVectorF1& t)
{
  throw("TODO");
//  return Direction;
}

/** Ray
***************************************************************************************************************************************************************/
template<size_t ambient_dim>
constexpr Ray<ambient_dim>::Ray(const SVectorF<ambient_dim>& direction, const SVectorF<ambient_dim>& _start)
  : Line<ambient_dim>::Line(direction, _start) {}

template<size_t ambient_dim>
constexpr SVectorF<ambient_dim>
Ray<ambient_dim>::ComputePoint(const SVectorF1& t)
{
  return isPositive(t[0]) ? Line<ambient_dim>::ComputePoint(t) : throw std::domain_error("The parameter must be positive for rays.");
}

/** Segment
***************************************************************************************************************************************************************/
template<size_t ambient_dim>
constexpr Segment<ambient_dim>::Segment(const SVectorF<ambient_dim>& _start, const SVectorF<ambient_dim>& _end)
  : Line<ambient_dim>::Line(_end - _start, _start) {}

template<size_t ambient_dim>
constexpr SVectorF<ambient_dim>
Segment<ambient_dim>::ComputePoint(const SVectorF1& t)
{
  const Float maxBound = this->isUnitSpeed ? this->DirectionMagnitude : One;
  return isBounded<true, true>(t[0], Zero, maxBound) ? Line<ambient_dim>::ComputePoint(t) :
         throw std::domain_error("The parameter must be in the range [0, " + ToStr(maxBound) + "] for this segment.");
}

/** SegmentChain
***************************************************************************************************************************************************************/
template<size_t ambient_dim>
template<class D>
SegmentChain<ambient_dim>::SegmentChain(const Array<SVectorF<ambient_dim>, D>& _vertices, const bool _is_closed)
  : isClosed(_is_closed)
{
  const auto& vertices = _vertices.Derived();
  Segments.reserve(vertices.size());
  CumulativeLengths.reserve(vertices.size());
  ChainLength = Zero;

  FOR(i, vertices.size() - static_cast<int>(!isClosed))
  {
    Segments.emplace_back(vertices[i], vertices[(i + 1) % vertices.size()]);
    Segments.back().SetIfUnitSpeed(true);

    ChainLength += Segments.back().Length();
    CumulativeLengths.emplace_back(ChainLength);
  }
}

template<size_t ambient_dim>
constexpr SVectorF<ambient_dim>
SegmentChain<ambient_dim>::ComputePoint(const SVectorF1& t)
{
  const Float upper_bound = isUnitSpeed ? ChainLength : One;
  const Float param_length = isBounded<true, true, true>(t[0], Zero, upper_bound) ? t[0] * (isUnitSpeed ? One : ChainLength) :
                             throw std::domain_error("The parameter must be in the range [0, " + ToStr(upper_bound) + "] for this segment.");
  const auto iter = std::find_if(CumulativeLengths.begin(), CumulativeLengths.end(), [param_length](auto _l){ return param_length <= _l; });
  const auto index = std::distance(CumulativeLengths.begin(), iter);
  const Float param = param_length - (index != 0 ? CumulativeLengths[index - 1] : Zero);

  return isBounded<true, true>(param, Zero, Segments[index].Length()) ? Segments[index].ComputePoint({param}) :
         throw std::domain_error("The parameter for segment " + ToStr(index) + " in the chain is out of bounds.");
}

template<size_t ambient_dim>
constexpr SVectorF<ambient_dim>
SegmentChain<ambient_dim>::ComputeTangent(const SVectorF1& t)
{
  throw("TODO");
}

template<size_t ambient_dim>
constexpr SVectorF<ambient_dim>
SegmentChain<ambient_dim>::ComputeBitangent(const SVectorF1& t)
{
  throw("TODO");
}

template<size_t ambient_dim>
constexpr SVectorF<ambient_dim>
SegmentChain<ambient_dim>::ComputeNormal(const SVectorF1& t)
{
  throw("TODO");
}

/***************************************************************************************************************************************************************
* Circular/Elliptical Curves
***************************************************************************************************************************************************************/

/** Circle
***************************************************************************************************************************************************************/
template<size_t ambient_dim>
Circle<ambient_dim>::Circle(const Float _radius, const SVectorF<ambient_dim>& _centre)
  : Centre(_centre), Radius(_radius), Normaliser(One/Radius) { ASSERT(isPositive(_radius), "A circle's radius cannot be negative.") }

template<size_t ambient_dim>
constexpr SVectorF<ambient_dim>
Circle<ambient_dim>::ComputePoint(const SVectorF1& t)
{
  const auto s = t[0] * (isUnitSpeed ? Normaliser : One);
  return ConvertVector<ambient_dim>(SVectorF3{Radius * Cos(s), Radius * Sin(s), Zero}) + Centre;
}

template<size_t ambient_dim>
constexpr SVectorF<ambient_dim>
Circle<ambient_dim>::ComputeTangent(const SVectorF1& t)
{
  throw("TODO");
}

template<size_t ambient_dim>
constexpr SVectorF<ambient_dim>
Circle<ambient_dim>::ComputeBitangent(const SVectorF1& t)
{
  throw("TODO");
}

template<size_t ambient_dim>
constexpr SVectorF<ambient_dim>
Circle<ambient_dim>::ComputeNormal(const SVectorF1& t)
{
  throw("TODO");
}

/** Ellipse
***************************************************************************************************************************************************************/
template<size_t ambient_dim>
Ellipse<ambient_dim>::Ellipse(const Float _x_radius, const Float _y_radius, const SVectorF<ambient_dim>& _centre)
  : Centre(_centre), RadiusX(_x_radius), RadiusY(_y_radius) { ASSERT(isPositive(_x_radius) && isPositive(_y_radius), "An ellipse's radii cannot be negative.") }

template<size_t ambient_dim>
constexpr SVectorF<ambient_dim>
Ellipse<ambient_dim>::ComputePoint(const SVectorF1& t)
{
  return ConvertVector<ambient_dim>(SVectorF3{RadiusX * Cos(t[0]), RadiusY * Sin(t[0]), Zero}) + Centre;
}

template<size_t ambient_dim>
constexpr SVectorF<ambient_dim>
Ellipse<ambient_dim>::ComputeTangent(const SVectorF1& t)
{
  throw("TODO");
}

template<size_t ambient_dim>
constexpr SVectorF<ambient_dim>
Ellipse<ambient_dim>::ComputeBitangent(const SVectorF1& t)
{
  throw("TODO");
}

template<size_t ambient_dim>
constexpr SVectorF<ambient_dim>
Ellipse<ambient_dim>::ComputeNormal(const SVectorF1& t)
{
  throw("TODO");
}

}