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