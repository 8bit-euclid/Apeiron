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
#include "LinearAlgebra/include/VectorOperations.h"

namespace aprn::mnfld {

/***************************************************************************************************************************************************************
* Curve Class Implementation
***************************************************************************************************************************************************************/

template<size_t D>
constexpr SVectorR<D>
Curve<D>::Binormal(const Vector& tangent, const Vector& normal) const { return CrossProduct(tangent, normal); }

/***************************************************************************************************************************************************************
* Linear/Piecewise Linear Curves
***************************************************************************************************************************************************************/

/** Line
***************************************************************************************************************************************************************/
template<size_t D>
constexpr Line<D>::Line(const Vector& direction, const Vector& point)
   : Direction(direction), Start(point), DirectionNorm(Magnitude(Direction)), Normaliser(One / DirectionNorm) {}

template<size_t D>
constexpr SVectorR<D>
Line<D>::Point(const Real t) const { return Start + t * (UnitSpeed ? Normaliser : One) * Direction; }

template<size_t D>
constexpr SVectorR<D>
Line<D>::Tangent([[maybe_unused]] const Real t) const { return (UnitSpeed ? Normaliser : One) * Direction; }

template<size_t D>
constexpr SVectorR<D>
Line<D>::Normal(const Real t) const
{
   EXIT("TODO")
   return Direction;
}

/** Ray
***************************************************************************************************************************************************************/
template<size_t D>
constexpr Ray<D>::Ray(const Vector& direction, const Vector& start)
   : Line<D>::Line(direction, start) {}

template<size_t D>
constexpr SVectorR<D>
Ray<D>::Point(const Real t) const
{
   return Positive(t) ? Line<D>::Point(t) : throw std::domain_error("The parameter must be positive for rays.");
}

/** Segment
***************************************************************************************************************************************************************/
template<size_t D>
constexpr Segment<D>::Segment(const Vector& start, const Vector& end)
   : Line<D>::Line(end - start, start) {}

template<size_t D>
constexpr SVectorR<D>
Segment<D>::Point(const Real t) const
{
   const Real max_bound = this->UnitSpeed ? this->DirectionNorm : One;
   return isBounded<true, true>(t, Zero, max_bound) ? Line<D>::Point(t) :
          throw std::domain_error("The parameter must be in the range [0, " + ToString(max_bound) + "] for this segment.");
}

/** SegmentChain
***************************************************************************************************************************************************************/
template<size_t Dim>
template<class D>
SegmentChain<Dim>::SegmentChain(const Array<Vector, D>& vertex_list, const bool is_closed)
   : Closed(is_closed)
{
   const auto& vertices = vertex_list.Derived();

   Segments.reserve(vertices.size());
   CumulativeLengths.reserve(vertices.size());
   ChainLength = Zero;

   FOR(i, vertices.size() - static_cast<size_t>(!Closed))
   {
      Segments.emplace_back(vertices[i], vertices[(i + 1) % vertices.size()]);
      Segments.back().MakeUnitSpeed();

      ChainLength += Segments.back().Length();
      CumulativeLengths.emplace_back(ChainLength);
   }
}

template<size_t D>
constexpr SVectorR<D>
SegmentChain<D>::Point(const Real t) const
{
   const Real upper_bound  = UnitSpeed ? ChainLength : One;
   const Real param_length = isBounded<true, true, true>(t, Zero, upper_bound) ? t * (UnitSpeed ? One : ChainLength) :
                             throw std::domain_error("The parameter must be in the range [0, " + ToString(upper_bound) + "] for this segment.");
   const auto iter  = std::find_if(CumulativeLengths.begin(), CumulativeLengths.end(), [param_length](auto _l){ return param_length <= _l; });
   const auto index = std::distance(CumulativeLengths.begin(), iter);
   const Real param = param_length - (index != 0 ? CumulativeLengths[index - 1] : Zero);

   return isBounded<true, true>(param, Zero, Segments[index].Length()) ? Segments[index].Point(param) :
          throw std::domain_error("The parameter for segment " + ToString(index) + " in the chain is out of bounds.");
}

template<size_t D>
constexpr SVectorR<D>
SegmentChain<D>::Tangent(const Real t) const
{
   EXIT("TODO")
}

template<size_t D>
constexpr SVectorR<D>
SegmentChain<D>::Normal(const Real t) const
{
   EXIT("TODO")
}

/***************************************************************************************************************************************************************
* Circular/Elliptical Curves
***************************************************************************************************************************************************************/

/** Circle
***************************************************************************************************************************************************************/
template<size_t D>
Circle<D>::Circle(const Real radius, const Vector& centre)
   : Centre(centre), Radius(radius), Normaliser(One/Radius) { ASSERT(Positive(radius), "A circle's radius cannot be negative.") }

template<size_t D>
constexpr SVectorR<D>
Circle<D>::Point(const Real t) const
{
   const auto s = t * (UnitSpeed ? Normaliser : One);
   return ToVector<D>(SVectorR3{Radius * Cos(s), Radius * Sin(s), Zero}) + Centre;
}

template<size_t D>
constexpr SVectorR<D>
Circle<D>::Tangent(const Real t) const
{
   EXIT("TODO")
}

template<size_t D>
constexpr SVectorR<D>
Circle<D>::Normal(const Real t) const
{
   EXIT("TODO")
}

/** Ellipse
***************************************************************************************************************************************************************/
template<size_t D>
Ellipse<D>::Ellipse(const Real x_radius, const Real y_radius, const Vector& centre)
   : Centre(centre), RadiusX(x_radius), RadiusY(y_radius) { ASSERT(Positive(x_radius) && Positive(y_radius), "An ellipse's radii cannot be negative.") }

template<size_t D>
constexpr SVectorR<D>
Ellipse<D>::Point(const Real t) const
{
   return ToVector<D>(SVectorR3{RadiusX * Cos(t), RadiusY * Sin(t), Zero}) + Centre;
}

template<size_t D>
constexpr SVectorR<D>
Ellipse<D>::Tangent(const Real t) const
{
   EXIT("TODO")
}

template<size_t D>
constexpr SVectorR<D>
Ellipse<D>::Normal(const Real t) const
{
   EXIT("TODO")
}

}