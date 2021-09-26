#ifndef MATH_H
#define MATH_H

#include "FloatingPoint.h"

/***************************************************************************************************************************************************************
* Commonly Used Mathematical Functions
***************************************************************************************************************************************************************/

/** Absolute value. */
template <typename data_type>
data_type constexpr Abs(const data_type& _value)
{
  return _value < data_type(0) ? -_value : _value;
}

/** Minimum value. */
template <typename data_type>
data_type constexpr Min(const data_type& _a, const data_type& _b)
{
  return _a < _b ? _a : _b;
}

/** Maximum value. */
template <typename data_type>
data_type constexpr Max(const data_type& _a, const data_type& _b)
{
  return _a > _b ? _a : _b;
}

/** Bound value between a minimum and a maximum. */
template <typename data_type>
data_type constexpr MinMax(const data_type& _a, const data_type& _min, const data_type& _max)
{
  return Max(Min(_a, _max), _min);
}

/** Signum function. */
template <typename data_type>
data_type constexpr Sgn(const data_type& _value)
{
  return (data_type(0) < _value) - (_value < data_type(0));
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
* Commonly Used Mathematical Constants
***************************************************************************************************************************************************************/

constexpr Float Small(Epsilon);
constexpr Float Zero(0.0);
constexpr Float Half(0.5);
constexpr Float Third(1.0/3.0);
constexpr Float Quarter(1.0/4.0);
constexpr Float One(1.0);
constexpr Float Two(2.0);
constexpr Float Three(3.0);
constexpr Float Four(4.0);
constexpr Float Five(5.0);
constexpr Float Pi(3.14159265358979323846264338327950288419716939937510);
constexpr Float E(2.71828182845904523536028747135266249775724709369995);
constexpr Float Phi(Half*(One + Sqrt(Five)));

#endif //MATH_H
