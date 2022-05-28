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
  constexpr Line(const SVectorF<ambient_dim>& _direction, const SVectorF<ambient_dim>& _coordinate = SVectorF<ambient_dim>{});

  constexpr void
  SetIfUnitSpeed(const bool _is_unit_speed) noexcept { isUnitSpeed = _is_unit_speed; }

protected:
  constexpr SVectorF<ambient_dim>
  ComputePoint(const SVectorF1& _t) override;

  constexpr SVectorF<ambient_dim>
  ComputeTangent(const SVectorF1& _t) override;

  constexpr SVectorF<ambient_dim>
  ComputeBitangent(const SVectorF1& _t) override;

  constexpr SVectorF<ambient_dim>
  ComputeNormal(const SVectorF1& _t) override;

  SVectorF<ambient_dim> Direction;
  SVectorF<ambient_dim> Coordinate0;
  Float DirectionMagnitude;
  Float Normaliser;
  bool isUnitSpeed{false};

  friend Curve<Line<ambient_dim>, ambient_dim>;
};

/** Ray
***************************************************************************************************************************************************************/
template<size_t ambient_dim = 2>
class Ray : public Line<ambient_dim>
{
public:
  constexpr Ray(const SVectorF<ambient_dim>& _direction, const SVectorF<ambient_dim>& _start = SVectorF<ambient_dim>{});

private:
  constexpr SVectorF<ambient_dim>
  ComputePoint(const SVectorF1& _t) override;
};

/** Segment
***************************************************************************************************************************************************************/
template<size_t ambient_dim = 2>
class Segment : public Line<ambient_dim>
{
public:
  constexpr Segment(const SVectorF<ambient_dim>& _start, const SVectorF<ambient_dim>& _end);

  constexpr Float Length() const noexcept { return this->DirectionMagnitude; }

private:
  constexpr SVectorF<ambient_dim>
  ComputePoint(const SVectorF1& _t) override;

  template<size_t dim> friend class SegmentChain;
};

/** Segment Chain
***************************************************************************************************************************************************************/
template<size_t ambient_dim = 2>
class SegmentChain : public Curve<SegmentChain<ambient_dim>, ambient_dim>
{
public:
  template<class D>
  SegmentChain(const Array<SVectorF<ambient_dim>, D>& _vertices, const bool _is_closed = false);

  constexpr void
  SetIfUnitSpeed(const bool _is_unit_speed) noexcept { isUnitSpeed = _is_unit_speed; }

private:
  constexpr SVectorF<ambient_dim>
  ComputePoint(const SVectorF1& _t) override;

  constexpr SVectorF<ambient_dim>
  ComputeTangent(const SVectorF1& _t) override;

  constexpr SVectorF<ambient_dim>
  ComputeBitangent(const SVectorF1& _t) override;

  constexpr SVectorF<ambient_dim>
  ComputeNormal(const SVectorF1& _t) override;

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
class Circle : public Curve<Circle<ambient_dim>, ambient_dim>
{
public:
  Circle(const Float _radius, const SVectorF<ambient_dim>& _centre = SVectorF<ambient_dim>{});

  constexpr void
  SetIfUnitSpeed(const bool _is_unit_speed) noexcept { isUnitSpeed = _is_unit_speed; }

private:
  constexpr SVectorF<ambient_dim>
  ComputePoint(const SVectorF1& _t) override;

  constexpr SVectorF<ambient_dim>
  ComputeTangent(const SVectorF1& _t) override;

  constexpr SVectorF<ambient_dim>
  ComputeBitangent(const SVectorF1& _t) override;

  constexpr SVectorF<ambient_dim>
  ComputeNormal(const SVectorF1& _t) override;

  SVectorF<ambient_dim> Centre;
  Float Radius;
  Float Normaliser;
  bool isUnitSpeed{false};

  friend Curve<Circle<ambient_dim>, ambient_dim>;
};

/** Ellipse
***************************************************************************************************************************************************************/
template<size_t ambient_dim = 2>
class Ellipse : public Curve<Ellipse<ambient_dim>, ambient_dim>
{
public:
  Ellipse(const Float _x_radius, const Float _y_radius, const SVectorF<ambient_dim>& _centre = SVectorF<ambient_dim>{});

private:
  SVectorF<ambient_dim> Centre;
  Float RadiusX;
  Float RadiusY;

  constexpr SVectorF<ambient_dim>
  ComputePoint(const SVectorF1& _t) override;

  constexpr SVectorF<ambient_dim>
  ComputeTangent(const SVectorF1& _t) override;

  constexpr SVectorF<ambient_dim>
  ComputeBitangent(const SVectorF1& _t) override;

  constexpr SVectorF<ambient_dim>
  ComputeNormal(const SVectorF1& _t) override;

  friend Curve<Ellipse<ambient_dim>, ambient_dim>;
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

#include "../src/Curve.cpp"
