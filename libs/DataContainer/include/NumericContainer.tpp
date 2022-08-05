#pragma once

namespace aprn {
namespace detail {

/** Scalar arithmetic operator overloads. */
template<Arithmetic T, class derived>
constexpr derived
NumericContainer<T, derived>::operator+(const std::convertible_to<T> auto _scalar) const
{
  derived out(Derived());
  FOR_EACH(entry, out) entry += _scalar;
  return out;
}

template<Arithmetic T, class derived>
constexpr derived
NumericContainer<T, derived>::operator-(const std::convertible_to<T> auto _scalar) const
{
  derived out(Derived());
  FOR_EACH(entry, out) entry -= _scalar;
  return out;
}

template<Arithmetic T, class derived>
constexpr derived
NumericContainer<T, derived>::operator*(const std::convertible_to<T> auto _scalar) const
{
  derived out(Derived());
  FOR_EACH(entry, out) entry *= _scalar;
  return out;
}

template<Arithmetic T, class derived>
constexpr derived
NumericContainer<T, derived>::operator/(const std::convertible_to<T> auto _scalar) const
{
  DEBUG_ASSERT(!isEqual(_scalar, Zero), "Cannot divide by zero.")
  derived out(Derived());
  FOR_EACH(entry, out) entry /= _scalar;
  return out;
}

template<Arithmetic T, class derived>
constexpr derived&
NumericContainer<T, derived>::operator+=(const std::convertible_to<T> auto _scalar)
{
  FOR_EACH(entry, Derived()) entry += _scalar;
  return Derived();
}

template<Arithmetic T, class derived>
constexpr derived&
NumericContainer<T, derived>::operator-=(const std::convertible_to<T> auto _scalar)
{
  FOR_EACH(entry, Derived()) entry -= _scalar;
  return Derived();
}

template<Arithmetic T, class derived>
constexpr derived&
NumericContainer<T, derived>::operator*=(const std::convertible_to<T> auto _scalar)
{
  FOR_EACH(entry, Derived()) entry *= _scalar;
  return Derived();
}

template<Arithmetic T, class derived>
constexpr derived&
NumericContainer<T, derived>::operator/=(const std::convertible_to<T> auto _scalar)
{
  DEBUG_ASSERT(!isEqual(_scalar, Zero), "Cannot divide by zero.")
  FOR_EACH(entry, Derived()) entry /= _scalar;
  return Derived();
}

/** Entry-wise binary arithmetic operator overloads. */
template<Arithmetic T, class derived>
template<class D>
constexpr derived
NumericContainer<T, derived>::operator+(const NumericContainer<T, D>& _container) const
{
  derived out(Derived());
  FOR(i, out.size()) out[i] += _container.Derived()[i];
  return out;
}

template<Arithmetic T, class derived>
template<class D>
constexpr derived
NumericContainer<T, derived>::operator-(const NumericContainer<T, D>& _container) const
{
  derived out(Derived());
  FOR(i, out.size()) out[i] -= _container.Derived()[i];
  return out;
}

template<Arithmetic T, class derived>
template<class D>
constexpr derived
NumericContainer<T, derived>::operator*(const NumericContainer<T, D>& _container) const
{
  derived out(Derived());
  FOR(i, out.size()) out[i] *= _container.Derived()[i];
  return out;
}

template<Arithmetic T, class derived>
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

template<Arithmetic T, class derived>
template<class D>
constexpr derived&
NumericContainer<T, derived>::operator+=(const NumericContainer<T, D>& _container)
{
  FOR(i, Derived().size()) Derived()[i] += _container.Derived()[i];
  return Derived();
}

template<Arithmetic T, class derived>
template<class D>
constexpr derived&
NumericContainer<T, derived>::operator-=(const NumericContainer<T, D>& _container)
{
  FOR(i, Derived().size()) Derived()[i] -= _container.Derived()[i];
  return Derived();
}

template<Arithmetic T, class derived>
template<class D>
constexpr derived&
NumericContainer<T, derived>::operator*=(const NumericContainer<T, D>& _container)
{
  FOR(i, Derived().size()) Derived()[i] *= _container.Derived()[i];
  return Derived();
}

template<Arithmetic T, class derived>
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
template<Arithmetic T, class derived>
constexpr derived
NumericContainer<T, derived>::operator-() const { return -1 * Derived(); }

/** Entry randomisation. */
template<Arithmetic T, class derived>
void NumericContainer<T, derived>::Randomise() { FOR_EACH(entry, Derived()) entry = Randomiser(); }

template<Arithmetic T, class derived>
void NumericContainer<T, derived>::ResetRandomiser(const T min, const T max) { Randomiser.Reset(min, max); }

}//Detail

/** Stand-alone Operator overloads. */
template<Arithmetic T, class derived>
constexpr derived
operator*(const std::convertible_to<T> auto _scalar, const detail::NumericContainer<T, derived>& _container) { return _container.Derived() * _scalar; }

}//aprn