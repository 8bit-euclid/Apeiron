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
#include "../../DataContainer/include/Array.h"
#include "../../DataContainer/include/NumericContainer.h"

namespace aprn {

/***************************************************************************************************************************************************************
* Vector Abstract Base Class
***************************************************************************************************************************************************************/
template<typename T, class derived>
class Vector : public detail::NumericContainer<T, derived>
{
 public:
   /** Subscript Operators */
   constexpr T& x() { return Derived()[0]; }

   constexpr T& y() { return Derived()[1]; }

   constexpr T& z() { return Derived()[2]; }

   constexpr T& w() { return Derived()[3]; }

   constexpr const T& x() const { return Derived()[0]; }

   constexpr const T& y() const { return Derived()[1]; }

   constexpr const T& z() const { return Derived()[2]; }

   constexpr const T& w() const { return Derived()[3]; }

   /** Derived Class Access */
   constexpr derived& Derived() noexcept { return static_cast<derived&>(*this); }

   constexpr const derived& Derived() const noexcept { return static_cast<const derived&>(*this); }

 protected:
   constexpr Vector() = default;
};

/***************************************************************************************************************************************************************
* Static Vector Class
***************************************************************************************************************************************************************/
template<typename T, size_t N>
class StaticVector final : public StaticArray<T, N>,
                           public Vector<T, StaticVector<T, N>>
{
   using BaseArray = StaticArray<T, N>;
   friend Vector<T, BaseArray>;

 public:
   /** Constructors */
   constexpr StaticVector()
     : BaseArray() {}

   explicit constexpr StaticVector(const T& value)
     : BaseArray(value) {}

   constexpr StaticVector(const std::initializer_list<T>& list)
     : BaseArray(list) {}

   template<class iter>
   constexpr StaticVector(const iter first, const iter last)
     : BaseArray(first, last) {}

   /** Operators */
   using BaseArray::operator[];
   using BaseArray::operator=;
};

/***************************************************************************************************************************************************************
* Dynamic Vector Class
***************************************************************************************************************************************************************/
template<typename T>
class DynamicVector final : public DynamicArray<T>,
                            public Vector<T, DynamicVector<T>>
{
   using BaseArray = DynamicArray<T>;
   friend Vector<T, BaseArray>;

 public:
   /** Constructors */
   DynamicVector()
     : BaseArray() {}

   explicit DynamicVector(const size_t size)
     : BaseArray(size) {}

   DynamicVector(const size_t size, const T& value)
     : BaseArray(size, value) {}

   DynamicVector(const std::initializer_list<T>& list)
     : BaseArray(list) {}

   template<class iter>
   DynamicVector(const iter first, const iter last)
     : BaseArray(first, last) {}

   /** Operators */
   using BaseArray::operator[];
   using BaseArray::operator=;
};

/***************************************************************************************************************************************************************
* Vector Aliases
***************************************************************************************************************************************************************/

/** Generic Vector Aliases
***************************************************************************************************************************************************************/
template<class D> using VectorB = Vector<Bool  , D>;
template<class D> using VectorU = Vector<size_t, D>;
template<class D> using VectorI = Vector<int   , D>;
template<class D> using VectorF = Vector<Real  , D>;

/** Static Vector Aliases
***************************************************************************************************************************************************************/
template<typename T, size_t N> using SVector = StaticVector<T, N>;

/** Sized vectors of arbitrary type */
template<typename T> using SVector2 = SVector<T, 2>;
template<typename T> using SVector3 = SVector<T, 3>;
template<typename T> using SVector4 = SVector<T, 4>;

/** Sized Real vectors */
template<size_t N> using SVectorR = SVector<Real, N>;
using SVectorR1 = SVectorR<1>;
using SVectorR2 = SVectorR<2>;
using SVectorR3 = SVectorR<3>;
using SVectorR4 = SVectorR<4>;

/** Coordinate Axes */
constexpr SVectorR2 xAxis2{ 1.0, 0.0 };
constexpr SVectorR2 yAxis2{ 0.0, 1.0 };
constexpr SVectorR3 xAxis3{ 1.0, 0.0, 0.0 };
constexpr SVectorR3 yAxis3{ 0.0, 1.0, 0.0 };
constexpr SVectorR3 zAxis3{ 0.0, 0.0, 1.0 };

/** Dynamic Vector Aliases
***************************************************************************************************************************************************************/
template<typename T> using DVector = DynamicVector<T>;

using DVectorB = DVector<Bool>;
using DVectorU = DVector<size_t>;
using DVectorI = DVector<int>;
using DVectorR = DVector<Real>;

/***************************************************************************************************************************************************************
* Static Vector Conversion
***************************************************************************************************************************************************************/
template<size_t N, size_t M, typename T>
constexpr SVector<T, N>
ConvertVector(const SVector<T, M>& from)
{
   SVector<T, N> to;
   std::copy(from.begin(), (from.begin() + Min(M, N)), to.begin());
   return to;
}

}
