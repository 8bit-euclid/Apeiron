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

#include "Manifold.h"
#include "../../LinearAlgebra/include/Vector.h"

namespace aprn::mnfld {

/** Curve Alias Template */
template<size_t ambient_dim>
using Curve = Manifold<1, ambient_dim>;

using Parameter = SVectorR1;

/***************************************************************************************************************************************************************
* Linear/Piecewise Linear Curves
***************************************************************************************************************************************************************/

/** Line
***************************************************************************************************************************************************************/
template<size_t ambient_dim = 2>
class Line : public Curve<ambient_dim>
{
   using Vector = SVectorR<ambient_dim>;

 public:
   constexpr Line(const Vector& direction, const Vector& point = Vector{});

   constexpr void MakeUnitSpeed() noexcept { UnitSpeed = true; }

 protected:
   constexpr Vector ComputePoint(const Parameter& t) override;

   constexpr Vector ComputeTangent(const Parameter& t) override;

   constexpr Vector ComputeBitangent(const Parameter& t) override;

   constexpr Vector ComputeNormal(const Parameter& t) override;

   Vector Direction;
   Vector Start;
   Real   DirectionNorm;
   Real   Normaliser;
   bool   UnitSpeed{false};
};

/** Ray
***************************************************************************************************************************************************************/
template<size_t ambient_dim = 2>
class Ray final : public Line<ambient_dim>
{
   using Vector = SVectorR<ambient_dim>;

 public:
   constexpr Ray(const Vector& direction, const Vector& start = Vector{});

 private:
   constexpr Vector ComputePoint(const Parameter& t) override;
};

/** Segment
***************************************************************************************************************************************************************/
template<size_t ambient_dim = 2>
class Segment final : public Line<ambient_dim>
{
   using Vector = SVectorR<ambient_dim>;

 public:
   constexpr Segment(const Vector& start, const Vector& end);

   constexpr Real Length() const noexcept { return this->DirectionNorm; }

 private:
   template<size_t dim> friend class SegmentChain;

   constexpr Vector ComputePoint(const Parameter& t) override;
};

/** Segment Chain
***************************************************************************************************************************************************************/
template<size_t ambient_dim = 2>
class SegmentChain final : public Curve<ambient_dim>
{
   using Vector = SVectorR<ambient_dim>;

 public:
   template<class D>
   SegmentChain(const Array<Vector, D>& vertices, bool is_closed = false);

   constexpr void MakeUnitSpeed() noexcept { UnitSpeed = true; }

 private:
   constexpr Vector ComputePoint(const Parameter& t) override;

   constexpr Vector ComputeTangent(const Parameter& t) override;

   constexpr Vector ComputeBitangent(const Parameter& t) override;

   constexpr Vector ComputeNormal(const Parameter& t) override;

   DArray<Segment<ambient_dim>> Segments;
   DArray<Real>                 CumulativeLengths;
   Real                         ChainLength;
   bool                         Closed;
   bool                         UnitSpeed{false};
};

/***************************************************************************************************************************************************************
* Elliptical/Circular Curves
***************************************************************************************************************************************************************/

/** Circle
***************************************************************************************************************************************************************/
template<size_t ambient_dim = 2>
class Circle final : public Curve<ambient_dim>
{
   using Vector = SVectorR<ambient_dim>;

 public:
   Circle(const Real radius, const Vector& centre = Vector{});

   constexpr void MakeUnitSpeed() noexcept { UnitSpeed = true; }

 private:
   constexpr Vector ComputePoint(const Parameter& t) override;

   constexpr Vector ComputeTangent(const Parameter& t) override;

   constexpr Vector ComputeBitangent(const Parameter& t) override;

   constexpr Vector ComputeNormal(const Parameter& t) override;

   Vector Centre;
   Real   Radius;
   Real   Normaliser;
   bool   UnitSpeed{false};
};

/** Ellipse
***************************************************************************************************************************************************************/
template<size_t ambient_dim = 2>
class Ellipse final : public Curve<ambient_dim>
{
   using Vector = SVectorR<ambient_dim>;

 public:
   Ellipse(const Real x_radius, const Real y_radius, const Vector& centre = Vector{});

 private:
   constexpr Vector ComputePoint(const Parameter& t) override;

   constexpr Vector ComputeTangent(const Parameter& t) override;

   constexpr Vector ComputeBitangent(const Parameter& t) override;

   constexpr Vector ComputeNormal(const Parameter& t) override;

   Vector Centre;
   Real   RadiusX;
   Real   RadiusY;
};

/***************************************************************************************************************************************************************
* Polynomial Curves
***************************************************************************************************************************************************************/


/***************************************************************************************************************************************************************
* Trigonometric Curves
***************************************************************************************************************************************************************/


/***************************************************************************************************************************************************************
* Bezier Curves
***************************************************************************************************************************************************************/


/***************************************************************************************************************************************************************
* Other Parametric Curves
***************************************************************************************************************************************************************/

}

#include "Curve.tpp"
