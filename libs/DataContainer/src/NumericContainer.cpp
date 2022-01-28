#pragma once

#include "../include/NumericContainer.h"

namespace Apeiron {
namespace Detail {

template<typename T, class derived>
constexpr derived& NumericContainer<T, derived>::operator+(const std::convertible_to<T> auto _scalar)
{
  FOR_EACH(entry, Derived()) entry += _scalar;
  return Derived();
}

template<typename T, class derived>
constexpr derived& NumericContainer<T, derived>::operator-(const std::convertible_to<T> auto _scalar)
{
  FOR_EACH(entry, Derived()) entry -= _scalar;
  return Derived();
}

template<typename T, class derived>
constexpr derived& NumericContainer<T, derived>::operator*(const std::convertible_to<T> auto _scalar)
{
  FOR_EACH(entry, Derived()) entry *= _scalar;
  return Derived();
}

template<typename T, class derived>
constexpr derived& NumericContainer<T, derived>::operator/(const std::convertible_to<T> auto _scalar)
{
  DEBUG_ASSERT(!isEqual(_scalar, Zero), "Cannot divide by zero.")
  FOR_EACH(entry, Derived()) entry /= _scalar;
  return Derived();
}

template<typename T, class derived>
constexpr derived& NumericContainer<T, derived>::operator+=(const std::convertible_to<T> auto _scalar)
{
  FOR_EACH(entry, Derived()) entry += _scalar;
  return Derived();
}

template<typename T, class derived>
constexpr derived& NumericContainer<T, derived>::operator-=(const std::convertible_to<T> auto _scalar)
{
  FOR_EACH(entry, Derived()) entry -= _scalar;
  return Derived();
}

template<typename T, class derived>
constexpr derived& NumericContainer<T, derived>::operator*=(const std::convertible_to<T> auto _scalar)
{
  FOR_EACH(entry, Derived()) entry *= _scalar;
  return Derived();
}

template<typename T, class derived>
constexpr derived& NumericContainer<T, derived>::operator/=(const std::convertible_to<T> auto _scalar)
{
  DEBUG_ASSERT(!isEqual(_scalar, Zero), "Cannot divide by zero.")
  FOR_EACH(entry, Derived()) entry /= _scalar;
  return Derived();
}

/** Element-wise arithmetic operator overloads. */
template<typename T, class derived>
constexpr derived& NumericContainer<T, derived>::operator+(const derived& _container)
{
  FOR(i, Derived().size()) Derived()[i] += _container[i];
  return Derived();
}

template<typename T, class derived>
constexpr derived& NumericContainer<T, derived>::operator-(const derived& _container)
{
  FOR(i, Derived().size()) Derived()[i] -= _container[i];
  return Derived();
}

template<typename T, class derived>
constexpr derived& NumericContainer<T, derived>::operator*(const derived& _container)
{
  FOR(i, Derived().size()) Derived()[i] *= _container[i];
  return Derived();
}

template<typename T, class derived>
constexpr derived& NumericContainer<T, derived>::operator/(const derived& _container)
{
  FOR(i, Derived().size())
  {
    DEBUG_ASSERT(!isEqual(_container[i], Zero), "Cannot divide by zero.")
    Derived()[i] /= _container[i];
  }
  return Derived();
}

template<typename T, class derived>
constexpr derived& NumericContainer<T, derived>::operator+=(const derived& _container)
{
  FOR(i, Derived().size()) Derived()[i] += _container[i];
  return Derived();
}

template<typename T, class derived>
constexpr derived& NumericContainer<T, derived>::operator-=(const derived& _container)
{
  FOR(i, Derived().size()) Derived()[i] -= _container[i];
  return Derived();
}

template<typename T, class derived>
constexpr derived& NumericContainer<T, derived>::operator*=(const derived& _container)
{
  FOR(i, Derived().size()) Derived()[i] *= _container[i];
  return Derived();
}

template<typename T, class derived>
constexpr derived& NumericContainer<T, derived>::operator/=(const derived& _container)
{
  FOR(i, Derived().size())
  {
    DEBUG_ASSERT(!isEqual(_container[i], Zero), "Cannot divide by zero.")
    Derived()[i] /= _container[i];
  }
  return Derived();
}

}

/** Stand-alone Operator overloads. */
template<typename T, class derived>
constexpr derived operator*(const std::convertible_to<T> auto _scalar, const Detail::NumericContainer<T, derived>& _container)
{
  derived out(_container.Derived());
  FOR(i, out.size()) out[i] *= _scalar;
  return out;
}

}