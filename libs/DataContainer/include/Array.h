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

   constexpr derived& operator=(const std::initializer_list<T>& _value_list) noexcept;

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
   using Base     = Array<T, StaticArray<T, N>>;
   using Iterator = typename StaticArray<T, N>::iterator;

 public:
   constexpr StaticArray();

   explicit constexpr StaticArray(const T& value);

   constexpr StaticArray(const std::initializer_list<T>& list);

   constexpr StaticArray(Iterator first, Iterator last);

   using Base::operator[];
   using Base::operator=;

   friend Base;
};

/***************************************************************************************************************************************************************
* Dynamic Array Class
***************************************************************************************************************************************************************/
template<typename T>
class DynamicArray : public std::vector<T>,
                     public Array<T, DynamicArray<T>>
{
   using Base     = Array<T, DynamicArray<T>>;
   using Iterator = typename DynamicArray<T>::iterator;

 public:
   DynamicArray();

   explicit DynamicArray(const size_t size);

   DynamicArray(const size_t size, const T& value);

   DynamicArray(const std::initializer_list<T>& list);

   DynamicArray(Iterator first, Iterator last);

   void Append(const T& value);

   void Append(T&& value) noexcept;

   void Append(const DynamicArray<T>& other);

   void Append(DynamicArray<T>&& other) noexcept;

   void Append(Iterator first, Iterator last, const bool move_all = false);

   using Base::operator[];
   using Base::operator=;

   friend Base;
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
