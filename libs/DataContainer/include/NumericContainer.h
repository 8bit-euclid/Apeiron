#pragma once

#include "../../../include/Global.h"

namespace Apeiron {
namespace Detail {

/***************************************************************************************************************************************************************
* Numeric Data Container Class and Common Operations
***************************************************************************************************************************************************************/
template<typename T, class derived>
class NumericContainer
{
protected:
  constexpr NumericContainer() { STATIC_ASSERT(isArithmetic<T>(), "Numeric containers can only be populated with numerical values.") }

public:
  /** Arithmetic operator overloads with scalars. */
  constexpr derived operator+(const std::convertible_to<T> auto _scalar) const;

  constexpr derived operator-(const std::convertible_to<T> auto _scalar) const;

  constexpr derived operator*(const std::convertible_to<T> auto _scalar) const;

  constexpr derived operator/(const std::convertible_to<T> auto _scalar) const;

  constexpr derived& operator+=(const std::convertible_to<T> auto _scalar);

  constexpr derived& operator-=(const std::convertible_to<T> auto _scalar);

  constexpr derived& operator*=(const std::convertible_to<T> auto _scalar);

  constexpr derived& operator/=(const std::convertible_to<T> auto _scalar);

  /** Element-wise arithmetic operator overloads. */
  template<class D>
  constexpr derived operator+(const NumericContainer<T, D>& _container) const;

  template<class D>
  constexpr derived operator-(const NumericContainer<T, D>& _container) const;

  template<class D>
  constexpr derived operator*(const NumericContainer<T, D>& _container) const;

  template<class D>
  constexpr derived operator/(const NumericContainer<T, D>& _container) const;

  template<class D>
  constexpr derived& operator+=(const NumericContainer<T, D>& _container);

  template<class D>
  constexpr derived& operator-=(const NumericContainer<T, D>& _container);

  template<class D>
  constexpr derived& operator*=(const NumericContainer<T, D>& _container);

  template<class D>
  constexpr derived& operator/=(const NumericContainer<T, D>& _container);

  /** Derived class access. */
  constexpr derived&
  Derived() noexcept { return static_cast<derived&>(*this); }

  constexpr const derived&
  Derived() const noexcept { return static_cast<const derived&>(*this); }
};

}//Detail

/** Stand-alone Operator overloads. */
template<typename T, class derived>
constexpr derived operator*(const std::convertible_to<T> auto _scalar, const Detail::NumericContainer<T, derived>& _container);

}//Apeiron

#include "../src/NumericContainer.cpp"
