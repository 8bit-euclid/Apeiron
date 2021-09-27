#pragma once

#include "Comparators.h"
#include "Constants.h"
#include "Debug.h"
#include "FloatingPoint.h"

namespace Apeiron{

/***************************************************************************************************************************************************************
* General Arithmetic Functions
***************************************************************************************************************************************************************/

template <typename data_type>
constexpr Float Divide(const data_type& _numerator, const data_type& _denominator)
{
  return !isEqual(static_cast<Float>(_denominator), Zero) ? static_cast<Float>(_numerator)/static_cast<Float>(_denominator) :
                                                            throw std::logic_error("Denominator must be non-zero during division.");
}

template <typename data_type>
constexpr data_type Modulo(const data_type& _numerator, const data_type& _denominator, std::true_type, std::false_type)
{
  return _numerator % _denominator;
}

template <typename data_type>
constexpr data_type Modulo(const data_type& _numerator, const data_type& _denominator, std::false_type, std::true_type)
{
  return std::fmod(_numerator, _denominator);
}

template <typename data_type>
constexpr data_type Modulo(const data_type& _numerator, const data_type& _denominator)
{
  return Modulo<data_type>(_numerator, _denominator, std::is_integral<data_type>(), std::is_floating_point<data_type>());
}

/***************************************************************************************************************************************************************
* Combinatorial Functions
***************************************************************************************************************************************************************/

constexpr unsigned int Factorial(const unsigned int _value)
{
  return 0 < _value ? _value*Factorial(_value - 1) : 1;
}

constexpr unsigned int FactorialDivision(const unsigned int _numerator, const unsigned int _denominator)
{
  return _denominator <= _numerator ? (_denominator < _numerator ? _numerator*FactorialDivision(_numerator - 1, _denominator) : 1) :
         throw std::logic_error("Numerator must be larger than the denominator.");
}

constexpr unsigned int Choose(const unsigned int _n, const unsigned int _r)
{
  return _r <= _n ? FactorialDivision(_n, _n - _r)/Factorial(_r) : throw std::logic_error("Numerator must be larger than the denominator.");
}

/***************************************************************************************************************************************************************
* Power Functions
***************************************************************************************************************************************************************/

template <typename data_type>
constexpr data_type iPower(const data_type& _value, const unsigned int& _exponent)
{
  return _exponent == 0 ? static_cast<data_type>(1) : _value*iPower(_value, _exponent - 1);
}

/** Square of a value. */
constexpr Float Square(const Float& _value)
{
  return iPower(_value, 2);
}

/** Cube of a value. */
constexpr Float Cube(const Float& _value)
{
  return iPower(_value, 3);
}

/** Newton-Raphson for constexpr Sqrt function. */
constexpr Float SqrtNewtonRaphson(const Float& _value, const Float& _value_curr, const Float& _value_prev)
{
  return _value_curr == _value_prev ? _value_curr : SqrtNewtonRaphson(_value, 0.5*(_value_curr + _value/_value_curr), _value_curr);
}

/** Constexpr version of std::sqrt. */
constexpr Float Sqrt(const Float& _value)
{
  return 0.0 <= _value && _value < std::numeric_limits<Float>::infinity() ? SqrtNewtonRaphson(_value, _value, 0.0) : std::numeric_limits<Float>::quiet_NaN();
}

/***************************************************************************************************************************************************************
* Trigonometric Functions
***************************************************************************************************************************************************************/

}//Apeiron

