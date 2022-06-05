#pragma once

#include "../../../include/Global.h"
#include "../../LinearAlgebra/include/Vector.h"

namespace aprn::func {

/***************************************************************************************************************************************************************
* Functions from R -> R
***************************************************************************************************************************************************************/
constexpr Float
Linear(const Float x, const Float c0, const Float c1) { return c0 + c1*x; }

constexpr Float
Quadratic(const Float x, const Float c0, const Float c1, const Float c2) { return c0 + c1*x + c2*x*x; }

constexpr Float
Cubic(const Float x, const Float c0, const Float c1, const Float c2, const Float c3) { return c0 + c1*x + c2*x*x + c3*x*x*x; }

/***************************************************************************************************************************************************************
* Functions from R -> R^n
***************************************************************************************************************************************************************/
constexpr SVectorF2
Ellipse(const SVectorF2& radii, const Float theta) { return {radii[0] * Cos(theta), radii[1] * Sin(theta)}; }

constexpr SVectorF2
Circle(const Float radius, const Float theta) { return Ellipse({radius, radius}, theta); }

constexpr SVectorF3
Ellipsoid(const SVectorF3& _radii, const Float _theta, const Float _phi)
{
   return { _radii[0]*Cos(_theta)*Sin(_phi), _radii[1]*Sin(_theta)*Sin(_phi), _radii[2]*Cos(_phi) };
}

constexpr SVectorF3
Sphere(const Float _radius, const Float _theta, const Float _phi) { return Ellipsoid({_radius, _radius, _radius}, _theta, _phi); }

//template<int p, int dimension = 1, typename T>
//constexpr T Polynomial(const StaticArray<T, dimension>& point, )
//{
//  STATIC_ASSERT(0 <= degree, "A polynomial cannot have a negative degree")
//  STATIC_ASSERT(0 < dimension, "The dimension of the polynomial must be at least 1")
//}
//
//template<int degree, int dimension = 1, typename T>
//constexpr T Polynomial(const StaticArray<T, dimension>& point, )
//{
//  STATIC_ASSERT(0 <= degree, "A polynomial cannot have a negative degree")
//  STATIC_ASSERT(0 < dimension, "The dimension of the polynomial must be at least 1")
//}

}