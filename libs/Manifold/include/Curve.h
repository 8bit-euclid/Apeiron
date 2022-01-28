#pragma once

#include "Manifold.h"

namespace Apeiron {
namespace Manifold {

/***************************************************************************************************************************************************************
* Curve Alias Template
***************************************************************************************************************************************************************/
template<class derived, std::size_t ambient_dim = 2>
using Curve = Manifold<derived, 1, ambient_dim>;

/***************************************************************************************************************************************************************
* Linear/Piecewise Linear Curves
***************************************************************************************************************************************************************/
template<std::size_t ambient_dim = 2>
class Line : public Curve<Line<ambient_dim>, ambient_dim>
{
public:
  Line(const SVectorF<ambient_dim>& _unit_direction, const SVectorF<ambient_dim>& _coordinate = ConvertVector<ambient_dim>({Zero, Zero, Zero}));

protected:
  SVectorF<ambient_dim> Coordinate0;
  SVectorF<ambient_dim> Direction;

  constexpr SVectorF<ambient_dim>
  ComputePoint(const SVectorF1& _t) override;

  constexpr SVectorF<ambient_dim>
  ComputeTangent(const SVectorF1& _t) override;

  constexpr SVectorF<ambient_dim>
  ComputeBitangent(const SVectorF1& _t) override;

  constexpr SVectorF<ambient_dim>
  ComputeNormal(const SVectorF1& _t) override;

  friend Curve<Line<ambient_dim>, ambient_dim>;
};

template<std::size_t ambient_dim = 2>
class Ray : public Line<ambient_dim>
{
public:
  Ray(const SVectorF<ambient_dim>& _unit_direction, const SVectorF<ambient_dim>& _start = ConvertVector<ambient_dim>({Zero, Zero, Zero}));
};

template<std::size_t ambient_dim = 2>
class Segment : public Line<ambient_dim>
{
public:
  Segment(const SVectorF<ambient_dim>& _start, const SVectorF<ambient_dim>& _end);

private:
  SVectorF<ambient_dim> Coordinate1;
};

template<std::size_t ambient_dim = 2>
class SegmentChain
{
public:
  template<class D>
  SegmentChain(const Array<SVectorF<ambient_dim>, D>& _vertices, const bool _is_closed = false);

};

/***************************************************************************************************************************************************************
* Elliptical/Circular Curves
***************************************************************************************************************************************************************/
template<std::size_t ambient_dim = 2>
class Ellipse : public Curve<Ellipse<ambient_dim>, ambient_dim>
{
public:
  Ellipse(const Float _x_radius, const Float _y_radius, const SVectorF<ambient_dim>& _centre = ConvertVector<ambient_dim>({Zero, Zero, Zero}));

private:
  SVectorF<ambient_dim> Centre;
  Float RadiusX;
  Float RadiusY;

  constexpr SVectorF<ambient_dim>
  ComputePoint(const SVectorF1& _theta) override;

  constexpr SVectorF<ambient_dim>
  ComputeTangent(const SVectorF1& _theta) override;

  constexpr SVectorF<ambient_dim>
  ComputeBitangent(const SVectorF1& _theta) override;

  constexpr SVectorF<ambient_dim>
  ComputeNormal(const SVectorF1& _theta) override;

  friend Curve<Ellipse<ambient_dim>, ambient_dim>;
};

template<std::size_t ambient_dim = 2>
class Circle : public Curve<Circle<ambient_dim>, ambient_dim>
{
public:
  Circle(const Float _radius, const SVectorF<ambient_dim>& _centre = ConvertVector<ambient_dim>({Zero, Zero, Zero}));

private:
  const Float Radius;

  constexpr SVectorF<ambient_dim>
  ComputePoint(const SVectorF1& _theta) override;

  constexpr SVectorF<ambient_dim>
  ComputeTangent(const SVectorF1& _theta) override;

  constexpr SVectorF<ambient_dim>
  ComputeBitangent(const SVectorF1& _theta) override;

  constexpr SVectorF<ambient_dim>
  ComputeNormal(const SVectorF1& _theta) override;

  friend Curve<Ellipse<ambient_dim>, ambient_dim>;
};

/***************************************************************************************************************************************************************
* Polynomial Curves
***************************************************************************************************************************************************************/

/***************************************************************************************************************************************************************
* Trigonometric Curves
***************************************************************************************************************************************************************/

/***************************************************************************************************************************************************************
* Arbitrary Parametric Curves
***************************************************************************************************************************************************************/

}
}
