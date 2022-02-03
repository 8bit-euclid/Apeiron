#pragma once

#include "Comparators.h"
#include "Constants.h"
#include "Debug.h"
#include "Types.h"

#include <functional>
#include <numeric>

namespace Apeiron{

/***************************************************************************************************************************************************************
* Math Support Functions
***************************************************************************************************************************************************************/
namespace Detail {

/** Modulo function for integer types. */
template<typename T>
constexpr T
Modulo(const T _numerator, const T _denominator, std::true_type, std::false_type) { return _numerator % _denominator; }

/** Modulo function for floating-point types. */
template<typename T>
constexpr T
Modulo(const T _numerator, const T _denominator, std::false_type, std::true_type) { return std::fmod(_numerator, _denominator); }

/** Newton-Raphson for constexpr Sqrt function. */
constexpr Float
SqrtNewtonRaphson(const Float _x, const Float _curr, const Float _prev)
{ return isEqual<true>(_curr, _prev) ? _curr : SqrtNewtonRaphson(_x, Half * (_curr + _x / _curr), _curr); }

/** Newton-Raphson for constexpr Cbrt function. */
constexpr Float
CbrtNewtonRaphson(const Float _x, const Float _curr, const Float _prev)
{ return isEqual<true>(_curr, _prev) ? _curr : CbrtNewtonRaphson(_x, Third * (Two * _curr + _x / (_curr * _curr)), _curr); }

/** Taylor expansion for constexpr Exp function. */
constexpr Float
Exp(const Float _x, const Float _sum, const Float n, const int _iteration, const Float _delta)
{ return isEqual<true>(_delta/n, Zero) ? _sum : Exp(_x, _sum + _delta / n, n * _iteration, _iteration + 1, _delta * _x); }

template<typename T>
constexpr Float
TrigonometricSeries(const T x, const T sum, const T n, const int i, const int s, const T t)
{ return isEqual<true>(t*s/n, Zero) ? sum : TrigonometricSeries(x, sum + t * s / n, n * i * (i + 1), i + 2, -s, t * x * x); }

template<typename T>
constexpr Float
InverseTrigonometricSeries(const T x, const T sum, const int n, const T t)
{
  return isEqual<true>(sum, sum + t * static_cast<Float>(n) / (n + 2.0)) ? sum :
         InverseTrigonometricSeries(x, sum + t * static_cast<Float>(n)/(n + 2.0), n + 2, t * x * x * static_cast<Float>(n) / (n + 3.0));
}

}

/***************************************************************************************************************************************************************
* Sequences and Series
***************************************************************************************************************************************************************/
template<typename T, T... sequence>
constexpr T
GetInteger(std::integer_sequence<T, sequence...>, std::size_t _index)
{
  constexpr T sequence_array[] = {sequence...};
  return sequence_array[_index];
}

/***************************************************************************************************************************************************************
* General Arithmetic Functions
***************************************************************************************************************************************************************/
/** Sum the terms of a sequence together. */
template<typename... T>
constexpr auto
Sum(const T... _values) { return (_values + ... + 0); }

/** Sum the terms of a sequence between two iterators. */
template<class iter>
constexpr auto
Sum(const iter _first, const iter _last)
{
  typedef typename std::iterator_traits<iter>::value_type data_type;
  return std::accumulate(_first, _last, static_cast<data_type>(0));
}

/** Product the terms of a sequence with each other. */
template<typename... T>
constexpr auto
Product(const T... _values) { return (_values * ... * 1); }

/** Product the terms of a sequence between two iterators. */
template<class iter>
constexpr auto
Product(const iter _first, const iter _last)
{
  typedef typename std::iterator_traits<iter>::value_type data_type;
  return std::accumulate(_first, _last, static_cast<data_type>(1), std::multiplies<data_type>());
}

/** Division function. */
template<typename T>
constexpr Float
Divide(const T _numerator, const T _denominator)
{
  return !isEqual(static_cast<Float>(_denominator), Zero) ? static_cast<Float>(_numerator)/static_cast<Float>(_denominator) :
                                                            throw std::logic_error("Denominator must be non-zero during division.");
}

/** Modulo function. */
template<typename T>
constexpr T
Modulo(const T _numerator, const T _denominator)
{
  return !isEqual(static_cast<Float>(_denominator), Zero) ?
         Detail::Modulo<T>(_numerator, _denominator, std::is_integral<T>(), std::is_floating_point<T>()) :
         throw std::logic_error("Denominator must be non-zero during modulo operation.");
}

/***************************************************************************************************************************************************************
* Combinatorial Functions
***************************************************************************************************************************************************************/
/** Factorial function (i.e. n!) */
constexpr unsigned int
Factorial(const unsigned int _x)
{
  return _x <= 20 ? (0 < _x ? _x*Factorial(_x - 1) : 1) :
         throw std::logic_error("Cannot currently compute the factorial of a number larger than 20.");
}

/** Factorial quotient function (i.e. n!/m!) */
constexpr unsigned int
FactorialQuotient(const unsigned int _numerator, const unsigned int _denominator)
{
  return _numerator <= 20 && _denominator <= 20 ?
         (_denominator <= _numerator ? (_denominator < _numerator ? _numerator * FactorialQuotient(_numerator - 1, _denominator) : 1) :
         throw std::logic_error("Numerator must be larger than the denominator.")) :
         throw std::logic_error("Cannot currently compute the factorial of a number larger than 20.");
}

/** Compination function ("n choose r"). */
constexpr unsigned int
Choose(const unsigned int _n, const unsigned int _r) {
  return _r <= _n ? FactorialQuotient(_n, _n - _r) / Factorial(_r) : throw std::logic_error("Numerator must be larger than the denominator.");
}

/***************************************************************************************************************************************************************
* Power Functions
***************************************************************************************************************************************************************/
/** Constexpr power function with integral exponents. */
template<typename T>
constexpr T
iPow(const T _x, const unsigned int _exponent)
{
  return _exponent <= 30 ? (_exponent == 0 ? static_cast<T>(1) : _x*iPow(_x, _exponent - 1)) :
         throw std::logic_error("Cannot currently compute the power with an exponent larger than 30.");
}

/** Square of a value. */
template<typename T>
constexpr T
Square(const T _x) { return iPow(_x, 2); }

/** Cube of a value. */
template<typename T>
constexpr T
Cube(const T _x) { return iPow(_x, 3); }

/** Constexpr version of std::sqrt. */
constexpr Float
Sqrt(const Float _x) { return isBounded<true, false, true>(_x, Zero, InfFloat<>) ? Detail::SqrtNewtonRaphson(_x, _x, Zero) : QuietNaN<>; }

/** Constexpr version of std::cbrt. */
constexpr Float
Cbrt(const Float _x) { return Detail::CbrtNewtonRaphson(_x, One, Zero); }

/** Constexpr version of std::hypot. */
constexpr Float
Hypot(const Float _x, const Float _y) { return Sqrt(Square(_x) + Square(_y)); }

/** Constexpr version of std::exp. */
constexpr Float
Exp(const Float _x) { return Detail::Exp(_x, 1.0, 1.0, 2, _x); }

/***************************************************************************************************************************************************************
* Trigonometric/Inverse-Trigonometric Functions
***************************************************************************************************************************************************************/
/** Constexpr sine function. */
constexpr Float
Sin(const Float _x) { return Detail::TrigonometricSeries(_x, _x, static_cast<Float>(6), 4, -1, iPow(_x, 3)); }

/** Constexpr cosine function. */
constexpr Float
Cos(const Float _x) { return Sin(_x + HalfPi); }

/** Constexpr tan function. */
constexpr Float
Tan(const Float _x)
{
  const auto denom = Cos(_x);
  return !isEqual(denom, Zero) ? Sin(_x) / denom : throw std::invalid_argument("Cannot compute tan(x) as cos(x) is 0.");
}

/** Constexpr arcsine function. */
constexpr Float
Arcsin(const Float _x)
{
  return isBounded<false, false, true>(static_cast<Float>(_x), -1.0, 1.0) ?
         Detail::InverseTrigonometricSeries(_x, _x, 1, _x * _x * _x / 2.0) :
         Abs(_x) == 1.0 ? Sgn(_x) * HalfPi : throw std::domain_error("The value " + ToStr(_x) + " is out of the arcsin domain bounds.");
}

/** Constexpr arccos function. */
constexpr Float
Arccos(const Float _x)
{
  return isBounded<false, false, true>(static_cast<Float>(_x), -1.0, 1.0) ? HalfPi - Arcsin(_x) : Abs(_x) == -1.0 ? Pi : Abs(_x) == 1.0 ? Zero :
                                                           throw std::domain_error("The value " + ToStr(_x) + " is out of the arccos domain bounds.");
}

/***************************************************************************************************************************************************************
* Hyperbolic/Inverse-Hyperbolic Functions
***************************************************************************************************************************************************************/

}//Apeiron

