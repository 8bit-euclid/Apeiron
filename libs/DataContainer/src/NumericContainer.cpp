#ifndef NUMERIC_CONT_TEMPLATE_DEF
#define NUMERIC_CONT_TEMPLATE_DEF

#include "../include/NumericContainer.h"

namespace aprn {
namespace detail {

/** Scalar arithmetic operator overloads. */
template<typename T, class derived>
requires Arithmetic<T>
constexpr derived
NumericContainer<T, derived>::operator+(const std::convertible_to<T> auto _scalar) const
{
  derived out(Derived());
  FOR_EACH(entry, out) entry += _scalar;
  return out;
}

template<typename T, class derived>
requires Arithmetic<T>
constexpr derived
NumericContainer<T, derived>::operator-(const std::convertible_to<T> auto _scalar) const
{
  derived out(Derived());
  FOR_EACH(entry, out) entry -= _scalar;
  return out;
}

template<typename T, class derived>
requires Arithmetic<T>
constexpr derived
NumericContainer<T, derived>::operator*(const std::convertible_to<T> auto _scalar) const
{
  derived out(Derived());
  FOR_EACH(entry, out) entry *= _scalar;
  return out;
}

template<typename T, class derived>
requires Arithmetic<T>
constexpr derived
NumericContainer<T, derived>::operator/(const std::convertible_to<T> auto _scalar) const
{
  DEBUG_ASSERT(!isEqual(_scalar, Zero), "Cannot divide by zero.")
  derived out(Derived());
  FOR_EACH(entry, out) entry /= _scalar;
  return out;
}

template<typename T, class derived>
requires Arithmetic<T>
constexpr derived&
NumericContainer<T, derived>::operator+=(const std::convertible_to<T> auto _scalar)
{
  FOR_EACH(entry, Derived()) entry += _scalar;
  return Derived();
}

template<typename T, class derived>
requires Arithmetic<T>
constexpr derived&
NumericContainer<T, derived>::operator-=(const std::convertible_to<T> auto _scalar)
{
  FOR_EACH(entry, Derived()) entry -= _scalar;
  return Derived();
}

template<typename T, class derived>
requires Arithmetic<T>
constexpr derived&
NumericContainer<T, derived>::operator*=(const std::convertible_to<T> auto _scalar)
{
  FOR_EACH(entry, Derived()) entry *= _scalar;
  return Derived();
}

template<typename T, class derived>
requires Arithmetic<T>
constexpr derived&
NumericContainer<T, derived>::operator/=(const std::convertible_to<T> auto _scalar)
{
  DEBUG_ASSERT(!isEqual(_scalar, Zero), "Cannot divide by zero.")
  FOR_EACH(entry, Derived()) entry /= _scalar;
  return Derived();
}

/** Entry-wise binary arithmetic operator overloads. */
template<typename T, class derived>
requires Arithmetic<T>
template<class D>
constexpr derived
NumericContainer<T, derived>::operator+(const NumericContainer<T, D>& _container) const
{
  derived out(Derived());
  FOR(i, out.size()) out[i] += _container.Derived()[i];
  return out;
}

template<typename T, class derived>
requires Arithmetic<T>
template<class D>
constexpr derived
NumericContainer<T, derived>::operator-(const NumericContainer<T, D>& _container) const
{
  derived out(Derived());
  FOR(i, out.size()) out[i] -= _container.Derived()[i];
  return out;
}

template<typename T, class derived>
requires Arithmetic<T>
template<class D>
constexpr derived
NumericContainer<T, derived>::operator*(const NumericContainer<T, D>& _container) const
{
  derived out(Derived());
  FOR(i, out.size()) out[i] *= _container.Derived()[i];
  return out;
}

template<typename T, class derived>
requires Arithmetic<T>
template<class D>
constexpr derived
NumericContainer<T, derived>::operator/(const NumericContainer<T, D>& _container) const
{
  derived out(Derived());

  FOR(i, out.size())
  {
    DEBUG_ASSERT(!isEqual(_container.Derived()[i], Zero), "Cannot divide by zero.")
    out[i] /= _container.Derived()[i];
  }
  return out;
}

template<typename T, class derived>
requires Arithmetic<T>
template<class D>
constexpr derived&
NumericContainer<T, derived>::operator+=(const NumericContainer<T, D>& _container)
{
  FOR(i, Derived().size()) Derived()[i] += _container.Derived()[i];
  return Derived();
}

template<typename T, class derived>
requires Arithmetic<T>
template<class D>
constexpr derived&
NumericContainer<T, derived>::operator-=(const NumericContainer<T, D>& _container)
{
  FOR(i, Derived().size()) Derived()[i] -= _container.Derived()[i];
  return Derived();
}

template<typename T, class derived>
requires Arithmetic<T>
template<class D>
constexpr derived&
NumericContainer<T, derived>::operator*=(const NumericContainer<T, D>& _container)
{
  FOR(i, Derived().size()) Derived()[i] *= _container.Derived()[i];
  return Derived();
}

template<typename T, class derived>
requires Arithmetic<T>
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

/** Entry-wise unary operator overloads. */
template<typename T, class derived>
requires Arithmetic<T>
constexpr derived
NumericContainer<T, derived>::operator-() const { return -1 * Derived(); }

/** Entry randomisation. */
template<typename T, class derived>
requires Arithmetic<T>
void NumericContainer<T, derived>::Randomise() { FOR_EACH(entry, Derived()) entry = Randomiser(); }

template<typename T, class derived>
requires Arithmetic<T>
void NumericContainer<T, derived>::ResetRandomiser(const T _min, const T _max) { Randomiser.Reset(_min, _max); }

template<typename T, class D>
requires Arithmetic<T>
Random<T> NumericContainer<T, D>::Randomiser = Random<T>();

}//Detail

/** Stand-alone Operator overloads. */
template<typename T, class derived>
requires Arithmetic<T>
constexpr derived
operator*(const std::convertible_to<T> auto _scalar, const detail::NumericContainer<T, derived>& _container)
{ return _container.Derived() * _scalar; }

}//aprn

#endif