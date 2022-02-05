#pragma once

#include "../../../include/Global.h"

#include <array>
#include <initializer_list>
#include <vector>

namespace Apeiron {

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
  constexpr void IndexBoundCheck(const std::size_t _index) const;

  constexpr void SizeCheck(const std::size_t _size0, const std::size_t _size1) const;

  /** Subscript Operator Overloads */
  constexpr T& operator[](const std::size_t _index);

  constexpr const T& operator[](const std::size_t _index) const;

  /** Assignment Operator Overloads */
  constexpr derived& operator=(const std::convertible_to<T> auto _value) noexcept;

  constexpr derived& operator=(const std::initializer_list<T>& _value_list) noexcept;

  /** Comparison Operator Overloads */
  constexpr bool operator==(const Array<T, derived>& _other) noexcept;

  constexpr bool operator!=(const Array<T, derived>& _other) noexcept;

private:
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
template<typename T, std::size_t N>
class StaticArray : public std::array<T, N>,
                    public Array<T, StaticArray<T, N>>
{
  using Base = Array<T, StaticArray<T, N>>;

public:
  /** Constructors */
  constexpr StaticArray();

  explicit constexpr StaticArray(const T& _value);

  constexpr StaticArray(const std::initializer_list<T>& _list);

  template<class iter>
  constexpr StaticArray(const iter _first, const iter _last);

  /** Operators */
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
  using Base = Array<T, DynamicArray<T>>;

public:
  /** Constructors */
  DynamicArray();

  explicit DynamicArray(const std::size_t _size);

  DynamicArray(const std::size_t _size, const T& _value);

  DynamicArray(const std::initializer_list<T>& _list);

  template<class iter>
  DynamicArray(const iter _first, const iter _last);

  /** Operators */
  using Base::operator[];
  using Base::operator=;

  friend Base;
};

/***************************************************************************************************************************************************************
* Static Array Aliases
***************************************************************************************************************************************************************/
template<typename T> using SArray1 = StaticArray<T, 1>;
template<typename T> using SArray2 = StaticArray<T, 2>;
template<typename T> using SArray3 = StaticArray<T, 3>;
template<typename T> using SArray4 = StaticArray<T, 4>;

template<std::size_t N> using SArrayB = StaticArray<Bool, N>;
template<std::size_t N> using SArrayU = StaticArray<std::size_t, N>;
template<std::size_t N> using SArrayI = StaticArray<int, N>;
template<std::size_t N> using SArrayF = StaticArray<Float, N>;

/***************************************************************************************************************************************************************
* Dynamic Array Aliases
***************************************************************************************************************************************************************/
using DArrayB = DynamicArray<Bool>;
using DArrayU = DynamicArray<std::size_t>;
using DArrayI = DynamicArray<int>;
using DArrayF = DynamicArray<Float>;

}

#include "../src/Array.cpp"
