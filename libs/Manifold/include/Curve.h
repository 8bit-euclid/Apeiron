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
template<class D, size_t ambient_dim>
using Curve = Manifold<D, 1, ambient_dim>;

/***************************************************************************************************************************************************************
* Linear/Piecewise Linear Curves
***************************************************************************************************************************************************************/

/** Line
***************************************************************************************************************************************************************/
template<size_t ambient_dim = 2>
class Line : public Curve<Line<ambient_dim>, ambient_dim>
{
   using Vector     = SVectorR<ambient_dim>;
   using Coordinate = Vector;
   using Parameter  = SVectorR1;

 public:
   constexpr Line(const Vector& direction, const Coordinate& point = Coordinate{});

   constexpr void MakeUnitSpeed() noexcept { UnitSpeed = true; }

 protected:
   friend Curve<Line<ambient_dim>, ambient_dim>;

   constexpr Coordinate ComputePoint(const Parameter& t) override;

   constexpr Vector ComputeTangent(const Parameter& t) override;

   constexpr Vector ComputeBitangent(const Parameter& t) override;

   constexpr Vector ComputeNormal(const Parameter& t) override;

   Vector     Direction;
   Coordinate Start;
   Real       DirectionNorm;
   Real       Normaliser;
   bool       UnitSpeed{false};
};

/** Ray
***************************************************************************************************************************************************************/
template<size_t ambient_dim = 2>
class Ray final : public Line<ambient_dim>
{
   using Vector     = SVectorR<ambient_dim>;
   using Coordinate = Vector;
   using Parameter  = SVectorR1;

 public:
   constexpr Ray(const Vector& direction, const Coordinate& start = Coordinate{});

 private:
   constexpr Coordinate ComputePoint(const Parameter& t) override;
};

/** Segment
***************************************************************************************************************************************************************/
template<size_t ambient_dim = 2>
class Segment final : public Line<ambient_dim>
{
   using Coordinate = SVectorR<ambient_dim>;
   using Parameter  = SVectorR1;

 public:
   constexpr Segment(const Coordinate& start, const Coordinate& end);

   constexpr Real Length() const noexcept { return this->DirectionNorm; }

 private:
   template<size_t dim> friend class SegmentChain;

   constexpr Coordinate ComputePoint(const Parameter& t) override;
};

/** Segment Chain
***************************************************************************************************************************************************************/
template<size_t ambient_dim = 2>
class SegmentChain final : public Curve<SegmentChain<ambient_dim>, ambient_dim>
{
   using Vector     = SVectorR<ambient_dim>;
   using Coordinate = Vector;
   using Parameter  = SVectorR1;

 public:
   template<class D>
   SegmentChain(const Array<Coordinate, D>& vertices, bool is_closed = false);

   constexpr void MakeUnitSpeed() noexcept { UnitSpeed = true; }

 private:
   constexpr Coordinate ComputePoint(const Parameter& t) override;

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
class Circle final : public Curve<Circle<ambient_dim>, ambient_dim>
{
   using Vector     = SVectorR<ambient_dim>;
   using Coordinate = Vector;
   using Parameter  = SVectorR1;

 public:
   Circle(const Real radius, const Coordinate& centre = Coordinate{});

   constexpr void MakeUnitSpeed() noexcept { UnitSpeed = true; }

 private:
   friend Curve<Circle<ambient_dim>, ambient_dim>;

   constexpr Coordinate ComputePoint(const Parameter& t) override;

   constexpr Vector ComputeTangent(const Parameter& t) override;

   constexpr Vector ComputeBitangent(const Parameter& t) override;

   constexpr Vector ComputeNormal(const Parameter& t) override;

   Coordinate Centre;
   Real       Radius;
   Real       Normaliser;
   bool       UnitSpeed{false};
};

/** Ellipse
***************************************************************************************************************************************************************/
template<size_t ambient_dim = 2>
class Ellipse final : public Curve<Ellipse<ambient_dim>, ambient_dim>
{
   using Vector     = SVectorR<ambient_dim>;
   using Coordinate = Vector;
   using Parameter  = SVectorR1;

 public:
   Ellipse(const Real x_radius, const Real y_radius, const Coordinate& centre = Coordinate{});

 private:
   friend Curve<Ellipse<ambient_dim>, ambient_dim>;

   constexpr Coordinate ComputePoint(const Parameter& t) override;

   constexpr Vector ComputeTangent(const Parameter& t) override;

   constexpr Vector ComputeBitangent(const Parameter& t) override;

   constexpr Vector ComputeNormal(const Parameter& t) override;

   Coordinate Centre;
   Real       RadiusX;
   Real       RadiusY;
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
