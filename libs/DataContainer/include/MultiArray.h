#pragma once

#include "../../../include/Global.h"
#include "Array.h"

namespace aprn{

/***************************************************************************************************************************************************************
* Multi-dimensional Array Abstract Base Class
***************************************************************************************************************************************************************/
template<typename T, class derived>
class MultiArray
{
protected:
  constexpr MultiArray() = default;

public:
  /** Size and index range-checking. */
  constexpr void
  MultiIndexBoundCheck(const std::convertible_to<size_t> auto... _multi_index) const;

  /** Multi-dimensional subscript index toggling. */
  constexpr size_t
  ComputeLinearIndex(const std::convertible_to<size_t> auto... _multi_index) const;

  constexpr auto
  ComputeMultiIndex(size_t index) const;

  /** Operator overloads. */
  constexpr T&
  operator()(std::convertible_to<size_t> auto... _multi_index);

  constexpr const T&
  operator()(const std::convertible_to<size_t> auto... _multi_index) const;

  constexpr derived&
  operator=(const std::initializer_list<T>& _value_array) noexcept;

  constexpr derived&
  operator=(const std::initializer_list<std::initializer_list<T>>& _value_matrix) noexcept;

  /** Iterators */
  constexpr auto
  begin() { return Derived().Entries.begin(); }

  constexpr const auto
  begin() const { return Derived().Entries.begin(); }

  constexpr auto
  end() { return Derived().Entries.end(); }

  constexpr const auto
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
template<typename T, size_t ...dims>
class StaticMultiArray : public MultiArray<T, StaticMultiArray<T, dims...>>
{
public:
  /** Constructors. */
  constexpr StaticMultiArray();

  explicit constexpr StaticMultiArray(const T value);

private:
  constexpr static StaticArray<size_t, sizeof...(dims)> Dimensions{dims...};
  constexpr static size_t nEntries{Product(dims...)};
  StaticArray<T, nEntries> Entries;

  friend MultiArray<T, StaticMultiArray<T, dims...>>;
};

/***************************************************************************************************************************************************************
* Dynamic Multi-dimensional Array Class
***************************************************************************************************************************************************************/
template<typename T>
class DynamicMultiArray : public MultiArray<T, DynamicMultiArray<T>>
{
public:
  /** Constructors. */
  DynamicMultiArray();

  DynamicMultiArray(const std::convertible_to<size_t> auto... _dimensions);

  /** Multi-array resize. */
  void Resize(const std::convertible_to<size_t> auto... _dimensions);

private:
  DynamicArray<size_t> Dimensions;
  size_t nEntries;
  DynamicArray<T> Entries;

  friend MultiArray<T, DynamicMultiArray<T>>;
};

}

#include "../src/MultiArray.cpp"
