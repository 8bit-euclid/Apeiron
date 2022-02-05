#pragma once

#include "../../../include/Global.h"

namespace Apeiron {
namespace Detail {

/***************************************************************************************************************************************************************
* Numeric Data Container Class and Arithmetic Operations
***************************************************************************************************************************************************************/
template<typename T, class derived>
requires Arithmetic<T>
class NumericContainer
{
protected:
  constexpr NumericContainer() {}

public:
  /** Scalar arithmetic operator overloads. */
  constexpr derived operator+(const std::convertible_to<T> auto _scalar) const;

  constexpr derived operator-(const std::convertible_to<T> auto _scalar) const;

  constexpr derived operator*(const std::convertible_to<T> auto _scalar) const;

  constexpr derived operator/(const std::convertible_to<T> auto _scalar) const;

  constexpr derived& operator+=(const std::convertible_to<T> auto _scalar);

  constexpr derived& operator-=(const std::convertible_to<T> auto _scalar);

  constexpr derived& operator*=(const std::convertible_to<T> auto _scalar);

  constexpr derived& operator/=(const std::convertible_to<T> auto _scalar);

  /** Entry-wise binary arithmetic operator overloads. */
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

  /** Entry-wise unary operator overloads. */
  constexpr derived operator-() const;

  /** Entry randomisation. */
  void Randomise();

  static void ResetRandomiser(const T _min, const T _max);

  /** Derived class access. */
  constexpr derived&
  Derived() noexcept { return static_cast<derived&>(*this); }

  constexpr const derived&
  Derived() const noexcept { return static_cast<const derived&>(*this); }

private:
  static Random<T> Randomiser;
};

}//Detail

/** Stand-alone Operator overloads. */
template<typename T, class derived>
requires Arithmetic<T>
constexpr derived operator*(const std::convertible_to<T> auto _scalar, const Detail::NumericContainer<T, derived>& _container);

}//Apeiron

#include "../src/NumericContainer.cpp"
