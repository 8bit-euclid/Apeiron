#pragma once

#include "../../../include/Global.h"
#include "Array.h"

namespace Apeiron{

/***************************************************************************************************************************************************************
* Multi-dimensional Array Abstract Base Class
***************************************************************************************************************************************************************/
template <typename T, class derived>
class MultiArray
{
protected:
  constexpr MultiArray() = default;

public:
  /** Size and index range-checking. */
  constexpr void
  MultiIndexBoundCheck(const std::convertible_to<std::size_t> auto ..._multi_index) const;

  /** Multi-dimensional subscript index toggling. */
  constexpr std::size_t
  ComputeLinearIndex(const std::convertible_to<std::size_t> auto ..._multi_index) const;

  constexpr auto
  ComputeMultiIndex(std::size_t _index) const;

  /** Operator overloads. */
  constexpr T&
  operator()(std::convertible_to<std::size_t> auto ..._multi_index);

  constexpr const T&
  operator()(const std::convertible_to<std::size_t> auto ..._multi_index) const;

  constexpr derived&
  operator=(const std::initializer_list<T>& _value_array) noexcept;

  constexpr derived&
  operator=(const std::initializer_list<std::initializer_list<T>>& _value_matrix) noexcept;

  /** Iterators */
  constexpr auto
  begin() { return Derived().Entries.begin(); }

  constexpr auto
  begin() const { return Derived().Entries.begin(); }

  constexpr auto
  end() { return Derived().Entries.end(); }

  constexpr auto
  end() const { return Derived().Entries.end(); }

private:
  /** Derived class access. */
  constexpr derived&
  Derived() noexcept { return static_cast<derived&>(*this); }

  constexpr const derived&
  Derived() const noexcept { return static_cast<const derived&>(*this); }
};

/***************************************************************************************************************************************************************
* Static Multi-dimensional Array Class
***************************************************************************************************************************************************************/
template <typename T, std::size_t ...dimensions>
class StaticMultiArray : public MultiArray<T, StaticMultiArray<T, dimensions...>>
{
public:
  /** Constructors. */
  constexpr StaticMultiArray();

  constexpr StaticMultiArray(const T& _value);

private:
  constexpr static StaticArray<std::size_t, sizeof...(dimensions)> Dimensions{dimensions...};
  constexpr static std::size_t nEntries{Product(dimensions...)};
  StaticArray<T, nEntries> Entries;

  friend MultiArray<T, StaticMultiArray<T, dimensions...>>;
};

/***************************************************************************************************************************************************************
* Dynamic Multi-dimensional Array Class
***************************************************************************************************************************************************************/
template <typename T>
class DynamicMultiArray : public MultiArray<T, DynamicMultiArray<T>>
{
public:
  /** Constructors. */
  DynamicMultiArray();

  DynamicMultiArray(const std::convertible_to<std::size_t> auto ..._dimensions);

  /** Multi-array resize. */
  void Resize(const std::convertible_to<std::size_t> auto ..._dimensions);

private:
  DynamicArray<std::size_t> Dimensions;
  std::size_t nEntries;
  DynamicArray<T> Entries;

  friend MultiArray<T, DynamicMultiArray<T>>;
};

}

#include "../src/MultiArray.cpp"
