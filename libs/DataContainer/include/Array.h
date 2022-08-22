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

#include <array>
#include <initializer_list>
#include <vector>

namespace aprn {

/***************************************************************************************************************************************************************
* Array Abstract Base Class
***************************************************************************************************************************************************************/
template<typename T, class derived>
class Array
{
 protected:
   constexpr Array() = default;

 public:
   /** Size and Index Range-checking */
   constexpr void IndexBoundCheck(const size_t index) const;

   constexpr void SizeCheck(const size_t _size0, const size_t _size1) const;

   /** Subscript Operator Overloads */
   constexpr T& operator[](const size_t index);

   constexpr const T& operator[](const size_t index) const;

   /** Assignment Operator Overloads */
   constexpr derived& operator=(const std::convertible_to<T> auto value) noexcept;

   constexpr derived& operator=(const std::initializer_list<T>& value_list) noexcept;

   /** Comparison Operator Overloads */
   constexpr bool operator==(const Array<T, derived>& _other) noexcept;

   constexpr bool operator!=(const Array<T, derived>& _other) noexcept;

   /** Derived Class Access */
   constexpr derived& Derived() noexcept { return static_cast<derived&>(*this); }

   constexpr const derived& Derived() const noexcept { return static_cast<const derived&>(*this); }
};

/** Non-member functions */
template<typename T, class D>
std::ostream& operator<<(std::ostream& _output_stream, const Array<T, D>& _array_base);

/***************************************************************************************************************************************************************
* Static Array Class
***************************************************************************************************************************************************************/
template<typename T, size_t N>
class StaticArray : public std::array<T, N>,
                    public Array<T, StaticArray<T, N>>
{
   using Base = Array<T, StaticArray<T, N>>;

 public:
   constexpr StaticArray();

   explicit constexpr StaticArray(const T& value);

   constexpr StaticArray(const std::initializer_list<T>& list);

   template<class Iter>
   constexpr StaticArray(Iter first, Iter last);

   using Base::operator[];
   using Base::operator=;
};

/***************************************************************************************************************************************************************
* Dynamic Array Class
***************************************************************************************************************************************************************/
template<typename T>
class DynamicArray : public std::vector<T>,
                     public Array<T, DynamicArray<T>>
{
   using Base     = Array<T, DynamicArray<T>>;

 public:
   DynamicArray();

   explicit DynamicArray(const size_t size);

   DynamicArray(const size_t size, const T& value);

   DynamicArray(const std::initializer_list<T>& list);

   template<class Iter>
   DynamicArray(Iter first, Iter last);

   void Append(const T& value);

   void Append(T&& value) noexcept;

   void Append(const DynamicArray<T>& other);

   void Append(DynamicArray<T>&& other) noexcept;

   template<class Iter>
   void Append(Iter first, Iter last, const bool move_all = false);

   void Erase();

   using Base::operator[];
   using Base::operator=;
};

/***************************************************************************************************************************************************************
* Static Array Aliases
***************************************************************************************************************************************************************/
template<typename T, size_t N> using SArray = StaticArray<T, N>;

template<typename T> using SArray1 = SArray<T, 1>;
template<typename T> using SArray2 = SArray<T, 2>;
template<typename T> using SArray3 = SArray<T, 3>;
template<typename T> using SArray4 = SArray<T, 4>;

template<size_t N> using SArrayB = SArray<Bool, N>;
template<size_t N> using SArrayU = SArray<size_t, N>;
template<size_t N> using SArrayI = SArray<int, N>;
template<size_t N> using SArrayF = SArray<Float, N>;

/***************************************************************************************************************************************************************
* Dynamic Array Aliases
***************************************************************************************************************************************************************/
template<typename T> using DArray = DynamicArray<T>;

using DArrayB = DArray<Bool>;
using DArrayU = DArray<size_t>;
using DArrayI = DArray<int>;
using DArrayF = DArray<Float>;

}

#include "Array.tpp"
