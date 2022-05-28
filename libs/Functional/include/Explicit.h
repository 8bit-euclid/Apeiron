#pragma once

#include "../../../include/Global.h"
#include "../../LinearAlgebra/include/Vector.h"

namespace aprn::func {

/***************************************************************************************************************************************************************
* Functions from R -> R
***************************************************************************************************************************************************************/
constexpr Float
Linear(const Float _x, const Float _m, const Float _c) { return _m * _x + _c; }

/***************************************************************************************************************************************************************
* Functions from R -> R^n
***************************************************************************************************************************************************************/
constexpr SVectorF2
Ellipse(const SVectorF2& _radii, const Float _theta) { return { _radii[0]*Cos(_theta), _radii[1]*Sin(_theta) }; }

constexpr SVectorF2
Circle(const Float _radius, const Float _theta) { return Ellipse({_radius, _radius}, _theta); }

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