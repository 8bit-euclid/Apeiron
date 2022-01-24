#pragma once

#include "../../../include/Global.h"
#include "../../DataContainer/include/Array.h"
#include "../../DataContainer/include/Detail.h"

namespace Apeiron {

/***************************************************************************************************************************************************************
* Vector Abstract Base Class
***************************************************************************************************************************************************************/
template <typename T, class derived>
class Vector : Detail::NumericContainer<T, Vector<T, derived>>
{
protected:
  constexpr Vector() = default;

private:
  /** Derived Class Access */
  constexpr derived& Derived() noexcept { return static_cast<derived&>(*this); }

  constexpr const derived& Derived() const noexcept { return static_cast<const derived&>(*this); }
};

/***************************************************************************************************************************************************************
* Static Vector Class
***************************************************************************************************************************************************************/
template <typename T, std::size_t N>
class StaticVector : public StaticArray<T, N>,
                     public Vector<T, StaticVector<T, N>>
{
  using BaseArray = StaticArray<T, N>;

public:
  /** Constructors */
  constexpr StaticVector()
    : BaseArray() {}

  explicit constexpr StaticVector(const T& _value)
    : BaseArray(_value) {}

  explicit constexpr StaticVector(const std::initializer_list<T>& _list)
    : BaseArray(_list) {}

  template <class iter>
  constexpr StaticVector(const iter _first, const iter _last)
    : BaseArray(_first, _last) {}

private:
  friend Vector<T, BaseArray>;
};

/***************************************************************************************************************************************************************
* Dynamic Vector Class
***************************************************************************************************************************************************************/
template <typename T>
class DynamicVector : public DynamicArray<T>,
                      public Vector<T, DynamicVector<T>>
{
  using BaseArray = DynamicArray<T>;

public:
  /** Constructors */
  DynamicVector()
    : BaseArray() {}

  explicit DynamicVector(const std::size_t _size)
    : BaseArray(_size) {}

  DynamicVector(const std::size_t _size, const T& _value)
    : BaseArray(_size, _value) {}

  explicit DynamicVector(const std::initializer_list<T>& _list)
    : BaseArray(_list) {}

  template <class iter>
  DynamicVector(const iter _first, const iter _last)
    : BaseArray(_first, _last) {}

private:
  friend Vector<T, BaseArray>;
};

/***************************************************************************************************************************************************************
* Vector Aliases
***************************************************************************************************************************************************************/
using SVec2 = StaticVector<Float, 2>;
using SVec3 = StaticVector<Float, 3>;
using SVec4 = StaticVector<Float, 4>;

}

#include "../src/Vector.cpp"
