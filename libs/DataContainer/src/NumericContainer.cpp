#pragma once

#include "../include/NumericContainer.h"

namespace Apeiron {
namespace Detail {

template<typename T, class derived>
constexpr derived
NumericContainer<T, derived>::operator+(const std::convertible_to<T> auto _scalar) const
{
  derived out(Derived());
  FOR_EACH(entry, out) entry += _scalar;
  return out;
}

template<typename T, class derived>
constexpr derived
NumericContainer<T, derived>::operator-(const std::convertible_to<T> auto _scalar) const
{
  derived out(Derived());
  FOR_EACH(entry, out) entry -= _scalar;
  return out;
}

template<typename T, class derived>
constexpr derived
NumericContainer<T, derived>::operator*(const std::convertible_to<T> auto _scalar) const
{
  derived out(Derived());
  FOR_EACH(entry, out) entry *= _scalar;
  return out;
}

template<typename T, class derived>
constexpr derived
NumericContainer<T, derived>::operator/(const std::convertible_to<T> auto _scalar) const
{
  DEBUG_ASSERT(!isEqual(_scalar, Zero), "Cannot divide by zero.")
  derived out(Derived());
  FOR_EACH(entry, out) entry /= _scalar;
  return out;
}

template<typename T, class derived>
constexpr derived&
NumericContainer<T, derived>::operator+=(const std::convertible_to<T> auto _scalar)
{
  FOR_EACH(entry, Derived()) entry += _scalar;
  return Derived();
}

template<typename T, class derived>
constexpr derived&
NumericContainer<T, derived>::operator-=(const std::convertible_to<T> auto _scalar)
{
  FOR_EACH(entry, Derived()) entry -= _scalar;
  return Derived();
}

template<typename T, class derived>
constexpr derived&
NumericContainer<T, derived>::operator*=(const std::convertible_to<T> auto _scalar)
{
  FOR_EACH(entry, Derived()) entry *= _scalar;
  return Derived();
}

template<typename T, class derived>
constexpr derived&
NumericContainer<T, derived>::operator/=(const std::convertible_to<T> auto _scalar)
{
  DEBUG_ASSERT(!isEqual(_scalar, Zero), "Cannot divide by zero.")
  FOR_EACH(entry, Derived()) entry /= _scalar;
  return Derived();
}

/** Element-wise arithmetic operator overloads. */
template<typename T, class derived>
template<class D>
constexpr derived
NumericContainer<T, derived>::operator+(const NumericContainer<T, D>& _container) const
{
  derived out(Derived());
  FOR(i, out.size()) out[i] += _container.Derived()[i];
  return out;
}

template<typename T, class derived>
template<class D>
constexpr derived
NumericContainer<T, derived>::operator-(const NumericContainer<T, D>& _container) const
{
  derived out(Derived());
  FOR(i, out.size()) out[i] += _container.Derived()[i];
  return out;
}

template<typename T, class derived>
template<class D>
constexpr derived
NumericContainer<T, derived>::operator*(const NumericContainer<T, D>& _container) const
{
  derived out(Derived());
  FOR(i, out.size()) out[i] += _container.Derived()[i];
  return out;
}

template<typename T, class derived>
template<class D>
constexpr derived
NumericContainer<T, derived>::operator/(const NumericContainer<T, D>& _container) const
{
  derived out(Derived());

  FOR(i, Derived().size())
  {
    DEBUG_ASSERT(!isEqual(_container.Derived()[i], Zero), "Cannot divide by zero.")
    FOR(i, out.size()) out[i] += _container.Derived()[i];
  }
  return out;
}

template<typename T, class derived>
template<class D>
constexpr derived&
NumericContainer<T, derived>::operator+=(const NumericContainer<T, D>& _container)
{
  FOR(i, Derived().size()) Derived()[i] += _container.Derived()[i];
  return Derived();
}

template<typename T, class derived>
template<class D>
constexpr derived&
NumericContainer<T, derived>::operator-=(const NumericContainer<T, D>& _container)
{
  FOR(i, Derived().size()) Derived()[i] -= _container.Derived()[i];
  return Derived();
}

template<typename T, class derived>
template<class D>
constexpr derived&
NumericContainer<T, derived>::operator*=(const NumericContainer<T, D>& _container)
{
  FOR(i, Derived().size()) Derived()[i] *= _container.Derived()[i];
  return Derived();
}

template<typename T, class derived>
template<class D>
constexpr derived&
NumericContainer<T, derived>::operator/=(const NumericContainer<T, D>& _container)
{
  FOR(i, Derived().size())
  {
    DEBUG_ASSERT(!isEqual(_container.Derived()[i], Zero), "Cannot divide by zero.")
    Derived()[i] /= _container.Derived()[i];
  }
  return Derived();
}

}//Detail

template<typename T, class derived>
constexpr derived
operator*(const std::convertible_to<T> auto _scalar, const Detail::NumericContainer<T, derived>& _container)
{ return _container.Derived() * _scalar; }

}//Apeiron