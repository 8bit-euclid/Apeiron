#pragma once

#include "../../../include/Global.h"
#include "../../DataContainer/include/Array.h"
#include "../../DataContainer/include/Detail.h"

namespace Apeiron {

/***************************************************************************************************************************************************************
* Vector Abstract Base Class
***************************************************************************************************************************************************************/
template<typename T, class derived>
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
template<typename T, std::size_t N>
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

  constexpr StaticVector(const std::initializer_list<T>& _list)
    : BaseArray(_list) {}

  template<class iter>
  constexpr StaticVector(const iter _first, const iter _last)
    : BaseArray(_first, _last) {}

private:
  friend Vector<T, BaseArray>;
};

/***************************************************************************************************************************************************************
* Dynamic Vector Class
***************************************************************************************************************************************************************/
template<typename T>
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

  DynamicVector(const std::initializer_list<T>& _list)
    : BaseArray(_list) {}

  template<class iter>
  DynamicVector(const iter _first, const iter _last)
    : BaseArray(_first, _last) {}

private:
  friend Vector<T, BaseArray>;
};

/***************************************************************************************************************************************************************
* Vector Aliases
***************************************************************************************************************************************************************/
template<typename T> using SVector2 = StaticVector<T, 2>;
template<typename T> using SVector3 = StaticVector<T, 3>;
template<typename T> using SVector4 = StaticVector<T, 4>;

template<std::size_t N> using SVectorF = StaticVector<Float, N>;
using SVectorF2 = SVectorF<2>;
using SVectorF3 = SVectorF<3>;
using SVectorF4 = SVectorF<4>;

}

#include "../src/Vector.cpp"
