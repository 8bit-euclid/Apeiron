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

/***************************************************************************************************************************************************************
* Curve Class Definition
***************************************************************************************************************************************************************/
template<size_t ambient_dim = 3>
class Curve
{
   using Vector = SVectorR<ambient_dim>;

 public:
   virtual constexpr Vector Point(const Real param) const = 0;

   virtual constexpr Vector Tangent(const Real param) const = 0;

   virtual constexpr Vector Normal(const Real param) const = 0;

   constexpr Vector Binormal(const Vector& tangent, const Vector& normal) const;
};

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

   constexpr Vector Point(const Real t) const override;

   constexpr Vector Tangent(const Real t) const override;

   constexpr Vector Normal(const Real t) const override;

   constexpr void MakeUnitSpeed() noexcept { UnitSpeed = true; }

 protected:
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

   constexpr Vector Point(const Real t) const override;
};

/** Segment
***************************************************************************************************************************************************************/
template<size_t ambient_dim = 2>
class Segment final : public Line<ambient_dim>
{
   using Vector = SVectorR<ambient_dim>;

 public:
   constexpr Segment(const Vector& start, const Vector& end);

   constexpr Vector Point(const Real t) const override;

   constexpr Real Length() const noexcept { return this->DirectionNorm; }
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

   constexpr Vector Point(const Real t) const override;

   constexpr Vector Tangent(const Real t) const override;

   constexpr Vector Normal(const Real t) const override;

 private:
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

   constexpr Vector Point(const Real t) const override;

   constexpr Vector Tangent(const Real t) const override;

   constexpr Vector Normal(const Real t) const override;

   constexpr void MakeUnitSpeed() noexcept { UnitSpeed = true; }

 private:
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

   constexpr Vector Point(const Real t) const override;

   constexpr Vector Tangent(const Real t) const override;

   constexpr Vector Normal(const Real t) const override;

 private:
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

///** Curve Chain
//***************************************************************************************************************************************************************/
//template<size_t ambient_dim = 2>
//class CurveChain final : public Curve<ambient_dim>
//{
//   using Vector = SVectorR<ambient_dim>;
//
// public:
//   template<class D>
//   CurveChain(const Array<Vector, D>& vertices, bool is_closed = false);
//
//   constexpr void MakeUnitSpeed() noexcept { UnitSpeed = true; }
//
// private:
//   constexpr Vector ComputePoint(const Parameter& t) const override;
//
//   constexpr Vector ComputeTangent(const Parameter& t) const override;
//
//   constexpr Vector ComputeBitangent(const Parameter& t) const override;
//
//   constexpr Vector ComputeNormal(const Parameter& t) const override;
//
//   DArray<Curve<ambient_dim>> Curves;
//   DArray<Real>               CumulativeLengths;
//   Real                       ChainLength;
//   bool                       Closed;
//   bool                       UnitSpeed{false};
//};

}

#include "Curve.tpp"
