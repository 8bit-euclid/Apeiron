#pragma once

#include "../../../include/Global.h"
#include "../../Tensor/include/Tensor.h"

namespace Apeiron{

/***************************************************************************************************************************************************************
* Functional Support Functions
***************************************************************************************************************************************************************/

namespace FunctionalSupport{

}

constexpr StaticArray<Float, 2> Circle(const Float _radius, const Float _theta);

constexpr StaticArray<Float, 2> Ellipse(const StaticArray<Float, 2>& _radii, const Float _theta);

constexpr StaticArray<Float, 3> Sphere(const Float _radius, const Float _theta, const Float _phi);

constexpr StaticArray<Float, 3> Ellipsoid(const StaticArray<Float, 3>& _radii, const Float _theta, const Float _phi);

//template <int p, int dimension = 1, typename t_data_type>
//constexpr t_data_type Polynomial(const StaticArray<t_data_type, dimension>& point, )
//{
//  STATIC_ASSERT(0 <= degree, "A polynomial cannot have a negative degree")
//  STATIC_ASSERT(0 < dimension, "The dimension of the polynomial must be at least 1")
//}
//
//template <int degree, int dimension = 1, typename t_data_type>
//constexpr t_data_type Polynomial(const StaticArray<t_data_type, dimension>& point, )
//{
//  STATIC_ASSERT(0 <= degree, "A polynomial cannot have a negative degree")
//  STATIC_ASSERT(0 < dimension, "The dimension of the polynomial must be at least 1")
//}

}