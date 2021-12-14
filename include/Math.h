#pragma once

#include "Comparators.h"
#include "Constants.h"
#include "Debug.h"
#include "Types.h"

namespace Apeiron{

/***************************************************************************************************************************************************************
* Math Support Functions
***************************************************************************************************************************************************************/
namespace MathSupport {

/** Modulo function for integer types. */
template <typename t_data_type>
constexpr t_data_type Modulo(const t_data_type& _numerator, const t_data_type& _denominator, std::true_type, std::false_type)
{
  return _numerator % _denominator;
}

/** Modulo function for floating-point types. */
template <typename t_data_type>
constexpr t_data_type Modulo(const t_data_type& _numerator, const t_data_type& _denominator, std::false_type, std::true_type)
{
  return std::fmod(_numerator, _denominator);
}

/** Newton-Raphson for constexpr Sqrt function. */
constexpr Float SqrtNewtonRaphson(const Float& _value, const Float& _value_curr, const Float& _value_prev)
{
  return _value_curr == _value_prev ? _value_curr : SqrtNewtonRaphson(_value, Half*(_value_curr + _value/_value_curr), _value_curr);
}

/** Newton-Raphson for constexpr Cbrt function. */
constexpr Float CbrtNewtonRaphson(const Float& _value, const Float& _value_curr, const Float& _value_prev)
{
  return _value_curr == _value_prev ? _value_curr : CbrtNewtonRaphson(_value, Third*(Two*_value_curr + _value/(_value_curr*_value_curr)), _value_curr);
}

/** Taylor expansion for constexpr Exp function. */
constexpr Float Exp(const Float& _value, const Float& _sum, const Float& n, const int _iteration, const Float& _delta_value)
{
  return _delta_value/n == Zero ? _sum : Exp(_value, _sum + _delta_value/n, n*_iteration, _iteration + 1, _delta_value*_value);
}

template <typename t_data_type>
constexpr t_data_type TrigonometricSeries(t_data_type x, t_data_type sum, t_data_type n, int i, int s, t_data_type t)
{
  return t * s / n == Zero ? sum : TrigonometricSeries(x, sum + t * s / n, n * i * (i + 1), i + 2, -s, t * x * x);
}

}

/***************************************************************************************************************************************************************
* Sequences and Series
***************************************************************************************************************************************************************/
template <class t_type, t_type ...t_integer_sequence>
constexpr t_type GetInteger(std::integer_sequence<t_type, t_integer_sequence...>, std::size_t _index)
{
  constexpr t_type sequence_array[] = {t_integer_sequence...};
  return sequence_array[_index];
}

/***************************************************************************************************************************************************************
* General Arithmetic Functions
***************************************************************************************************************************************************************/
/** Sum the terms of a sequence together. */
template <typename ...t_data_type>
constexpr auto Sum(const t_data_type& ..._values)
{
  return (_values + ... + 0);
}

/** Sum the terms of a sequence between two iterators. */
template <class t_iterator>
constexpr auto Sum(const t_iterator _first, const t_iterator _last)
{
  typedef typename std::iterator_traits<t_iterator>::value_type data_type;
  return std::accumulate(_first, _last, static_cast<data_type>(0));
}

/** Product the terms of a sequence with each other. */
template <typename ...t_data_type>
constexpr auto Product(const t_data_type& ..._values)
{
  return (_values * ... * 1);
}

/** Product the terms of a sequence between two iterators. */
template <class t_iterator>
constexpr auto Product(const t_iterator _first, const t_iterator _last)
{
  typedef typename std::iterator_traits<t_iterator>::value_type data_type;
  return std::accumulate(_first, _last, static_cast<data_type>(1), std::multiplies<data_type>());
}

/** Division function. */
template <typename t_data_type>
constexpr Float Divide(const t_data_type& _numerator, const t_data_type& _denominator)
{
  return !isEqual(static_cast<Float>(_denominator), Zero) ? static_cast<Float>(_numerator)/static_cast<Float>(_denominator) :
                                                            throw std::logic_error("Denominator must be non-zero during division.");
}

/** Modulo function. */
template <typename t_data_type>
constexpr t_data_type Modulo(const t_data_type& _numerator, const t_data_type& _denominator)
{
  return !isEqual(static_cast<Float>(_denominator), Zero) ?
    MathSupport::Modulo<t_data_type>(_numerator, _denominator, std::is_integral<t_data_type>(), std::is_floating_point<t_data_type>()) :
    throw std::logic_error("Denominator must be non-zero during modulo operation.");
}

/***************************************************************************************************************************************************************
* Combinatorial Functions
***************************************************************************************************************************************************************/
/** Factorial function. */
constexpr unsigned int Factorial(const unsigned int _value)
{
  return _value <= 20 ? (0 < _value ? _value*Factorial(_value - 1) : 1) :
         throw std::logic_error("Cannot currently compute the factorial of a number larger than 20.");
}

/** Factorial quotient function. */
constexpr unsigned int FactorialQuotient(const unsigned int _numerator, const unsigned int _denominator)
{
  return _numerator <= 20 && _denominator <= 20 ?
         (_denominator <= _numerator ? (_denominator < _numerator ? _numerator * FactorialQuotient(_numerator - 1, _denominator) : 1) :
         throw std::logic_error("Numerator must be larger than the denominator.")) :
         throw std::logic_error("Cannot currently compute the factorial of a number larger than 20.");
}

/** Compination function ("n choose r"). */
constexpr unsigned int Choose(const unsigned int _n, const unsigned int _r)
{
  return _r <= _n ? FactorialQuotient(_n, _n - _r) / Factorial(_r) : throw std::logic_error("Numerator must be larger than the denominator.");
}

/***************************************************************************************************************************************************************
* Power Functions
***************************************************************************************************************************************************************/
template <typename t_data_type>
constexpr t_data_type iPow(const t_data_type& _value, const unsigned int& _exponent)
{
  return _exponent <= 30 ? (_exponent == 0 ? static_cast<t_data_type>(1) : _value*iPow(_value, _exponent - 1)) :
         throw std::logic_error("Cannot currently compute the power with an exponent larger than 30.");
}

/** Square of a value. */
template <typename t_data_type>
constexpr t_data_type Square(const t_data_type& _value)
{
  return iPow(_value, 2);
}

/** Cube of a value. */
template <typename t_data_type>
constexpr t_data_type Cube(const t_data_type& _value)
{
  return iPow(_value, 3);
}

/** Constexpr version of std::sqrt. */
constexpr Float Sqrt(const Float& _value)
{
  return Zero <= _value && _value < Infinity ? MathSupport::SqrtNewtonRaphson(_value, _value, Zero) : QuietNaN;
}

constexpr Float Cbrt(const Float& _value)
{
  return MathSupport::CbrtNewtonRaphson(_value, One, Zero);
}

/** Constexpr version of std::sqrt. */
template <typename t_data_type>
constexpr Float Hypot(const t_data_type& _value0, const t_data_type& _value1)
{
  return Sqrt(Square(_value0) + Square(_value1));
}

constexpr Float Exp(Float _value)
{
  return MathSupport::Exp(_value, 1.0, 1.0, 2, _value);
}

/***************************************************************************************************************************************************************
* Trigonometric Functions
***************************************************************************************************************************************************************/
template <typename t_data_type>
constexpr Float Sin(const t_data_type _value)
{
  return MathSupport::TrigonometricSeries(_value, _value, static_cast<Float>(6), 4, -1, iPow(_value, 3));
}

template <typename t_data_type>
constexpr Float Cos(t_data_type _value)
{
  return Sin(_value + HalfPi);
}

template <typename t_data_type>
constexpr Float Tan(t_data_type _value)
{
  return !isEqual(Cos(_value), Zero) ? sin(_value) / cos(_value) : throw std::invalid_argument("Cannot compute tan(x) as cos(x) is 0.");
}

}//Apeiron

