#pragma once

#include "Constants.h"

namespace Apeiron{

/***************************************************************************************************************************************************************
* Min, Max, and Bounding Functions
***************************************************************************************************************************************************************/

/** Min value. */
template<typename T>
constexpr T Min(const T& _a, const T& _b)
{
  return std::min(_a, _b);
}

/** Max value. */
template<typename T>
constexpr T Max(const T& _a, const T& _b)
{
  return std::max(_a, _b);
}

/** Bound value between a minimum and a maximum. */
template<typename T>
constexpr std::pair<T, T> MinMax(const T& _a, const T& _b)
{
  return std::minmax(_a, _b);
}

/** Bound value between a minimum and a maximum. */
template<typename T>
constexpr T Bound(const T& _value, const T& _min, const T& _max)
{
  return _min < _max ? Max(Min(_value, _max), _min) : throw std::invalid_argument("The minimum bound must be lesser than the maximum bound.");
}

/***************************************************************************************************************************************************************
* Signum and Absolute Value Functions
***************************************************************************************************************************************************************/

/** Signum function. */
template<typename T>
constexpr T Sgn(const T& _value, const int _zero_sign = 1)
{
  switch(_zero_sign)
  {
    case -1: return _value > static_cast<T>(0) ? static_cast<T>(1) : static_cast<T>(-1);
    case 0: return (static_cast<T>(0) < _value) - (_value < static_cast<T>(0));
    case 1: return _value >= static_cast<T>(0) ? static_cast<T>(1) : static_cast<T>(-1);
    default: EXIT("Unrecognised sign for zero.")
  }
}

/** Absolute value. */
template<typename T>
constexpr T Abs(const T& _value)
{
  return _value < static_cast<T>(0) ? -_value : _value;
}

/***************************************************************************************************************************************************************
* Numerical Rounding Functions
***************************************************************************************************************************************************************/

/** Floor function. */
template<typename integer_T = int64_t>
constexpr Float Floor(const Float& _value)
{
  return static_cast<integer_T>(_value) - (static_cast<integer_T>(_value) > _value);
}

/** Ceiling function. */
template<typename integer_T = int64_t>
constexpr Float Ceil(const Float& _value)
{
  return static_cast<integer_T>(_value) + (static_cast<integer_T>(_value) < _value);
}

/** Rounding function. */
template<typename integer_T = int64_t>
constexpr Float Round(const Float& _value)
{
  return _value < Floor(_value) + Half ? Floor(_value) : Ceil(_value);
}

/***************************************************************************************************************************************************************
* Mathematical Conversions
***************************************************************************************************************************************************************/

/** Convert degrees to radians. */
constexpr Float ToRadians(const Float& _angle_degrees)
{
  return _angle_degrees * Pi / 180.0;
}

/** Convert radians to degrees. */
constexpr Float ToDegrees(const Float& _angle_radians)
{
  return _angle_radians * 180.0 / Pi;
}

}
