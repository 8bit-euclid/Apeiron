#pragma once

#include "../../../include/Global.h"
#include "../../LinearAlgebra/include/Vector.h"

namespace Apeiron{

constexpr SVectorF2
Circle(const Float _radius, const Float _theta);

constexpr SVectorF2
Ellipse(const SVectorF2& _radii, const Float _theta);

constexpr SVectorF3
Sphere(const Float _radius, const Float _theta, const Float _phi);

constexpr SVectorF3
Ellipsoid(const SVectorF3& _radii, const Float _theta, const Float _phi);

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