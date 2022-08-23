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
template<class derived, size_t ambient_dim>
using Curve = Manifold<derived, 1, ambient_dim>;

/***************************************************************************************************************************************************************
* Linear/Piecewise Linear Curves
***************************************************************************************************************************************************************/

/** Line
***************************************************************************************************************************************************************/
template<size_t ambient_dim = 2>
class Line : public Curve<Line<ambient_dim>, ambient_dim>
{
 public:
   constexpr Line(const SVectorF<ambient_dim>& direction, const SVectorF<ambient_dim>& coordinate = SVectorF<ambient_dim>{});

   constexpr void
   SetIfUnitSpeed(const bool is_unit_speed) noexcept { isUnitSpeed = is_unit_speed; }

 protected:
   friend Curve<Line<ambient_dim>, ambient_dim>;

   constexpr SVectorF<ambient_dim>
   ComputePoint(const SVectorF1& t) override;

   constexpr SVectorF<ambient_dim>
   ComputeTangent(const SVectorF1& t) override;

   constexpr SVectorF<ambient_dim>
   ComputeBitangent(const SVectorF1& t) override;

   constexpr SVectorF<ambient_dim>
   ComputeNormal(const SVectorF1& t) override;

   SVectorF<ambient_dim> Direction;
   SVectorF<ambient_dim> Coordinate0;
   Float DirectionMagnitude;
   Float Normaliser;
   bool isUnitSpeed{false};
};

/** Ray
***************************************************************************************************************************************************************/
template<size_t ambient_dim = 2>
class Ray final : public Line<ambient_dim>
{
 public:
   constexpr Ray(const SVectorF<ambient_dim>& direction, const SVectorF<ambient_dim>& _start = SVectorF<ambient_dim>{});

 private:
   constexpr SVectorF<ambient_dim>
   ComputePoint(const SVectorF1& t) override;
};

/** Segment
***************************************************************************************************************************************************************/
template<size_t ambient_dim = 2>
class Segment final : public Line<ambient_dim>
{
 public:
   constexpr Segment(const SVectorF<ambient_dim>& _start, const SVectorF<ambient_dim>& _end);

   constexpr Float Length() const noexcept { return this->DirectionMagnitude; }

 private:
   constexpr SVectorF<ambient_dim>
   ComputePoint(const SVectorF1& t) override;

   template<size_t dim> friend class SegmentChain;
};

/** Segment Chain
***************************************************************************************************************************************************************/
template<size_t ambient_dim = 2>
class SegmentChain final : public Curve<SegmentChain<ambient_dim>, ambient_dim>
{
 public:
   template<class D>
   SegmentChain(const Array<SVectorF<ambient_dim>, D>& vertices, const bool _is_closed = false);

   constexpr void
   SetIfUnitSpeed(const bool is_unit_speed) noexcept { isUnitSpeed = is_unit_speed; }

 private:
   constexpr SVectorF<ambient_dim>
   ComputePoint(const SVectorF1& t) override;

   constexpr SVectorF<ambient_dim>
   ComputeTangent(const SVectorF1& t) override;

   constexpr SVectorF<ambient_dim>
   ComputeBitangent(const SVectorF1& t) override;

   constexpr SVectorF<ambient_dim>
   ComputeNormal(const SVectorF1& t) override;

   DynamicArray<Segment<ambient_dim>> Segments;
   DynamicArray<Float> CumulativeLengths;
   Float ChainLength;
   bool isClosed;
   bool isUnitSpeed{false};
};

/***************************************************************************************************************************************************************
* Elliptical/Circular Curves
***************************************************************************************************************************************************************/

/** Circle
***************************************************************************************************************************************************************/
template<size_t ambient_dim = 2>
class Circle final : public Curve<Circle<ambient_dim>, ambient_dim>
{
 public:
   Circle(const Float _radius, const SVectorF<ambient_dim>& _centre = SVectorF<ambient_dim>{});

   constexpr void
   SetIfUnitSpeed(const bool is_unit_speed) noexcept { isUnitSpeed = is_unit_speed; }

 private:
   constexpr SVectorF<ambient_dim>
   ComputePoint(const SVectorF1& t) override;

   constexpr SVectorF<ambient_dim>
   ComputeTangent(const SVectorF1& t) override;

   constexpr SVectorF<ambient_dim>
   ComputeBitangent(const SVectorF1& t) override;

   constexpr SVectorF<ambient_dim>
   ComputeNormal(const SVectorF1& t) override;

   SVectorF<ambient_dim> Centre;
   Float Radius;
   Float Normaliser;
   bool isUnitSpeed{false};

   friend Curve<Circle<ambient_dim>, ambient_dim>;
};

/** Ellipse
***************************************************************************************************************************************************************/
template<size_t ambient_dim = 2>
class Ellipse final : public Curve<Ellipse<ambient_dim>, ambient_dim>
{
 public:
   Ellipse(const Float _x_radius, const Float _y_radius, const SVectorF<ambient_dim>& _centre = SVectorF<ambient_dim>{});

 private:
   friend Curve<Ellipse<ambient_dim>, ambient_dim>;

   SVectorF<ambient_dim> Centre;
   Float RadiusX;
   Float RadiusY;

   constexpr SVectorF<ambient_dim>
   ComputePoint(const SVectorF1& t) override;

   constexpr SVectorF<ambient_dim>
   ComputeTangent(const SVectorF1& t) override;

   constexpr SVectorF<ambient_dim>
   ComputeBitangent(const SVectorF1& t) override;

   constexpr SVectorF<ambient_dim>
   ComputeNormal(const SVectorF1& t) override;
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
