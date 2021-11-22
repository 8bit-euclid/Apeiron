#pragma once

#include "Constants.h"

namespace Apeiron{

/***************************************************************************************************************************************************************
* Min, Max, and Bounding Functions
***************************************************************************************************************************************************************/

/** Min value. */
template <typename t_data_type>
constexpr t_data_type Min(const t_data_type& _a, const t_data_type& _b)
{
  return std::min(_a, _b);
}

/** Max value. */
template <typename t_data_type>
constexpr t_data_type Max(const t_data_type& _a, const t_data_type& _b)
{
  return std::max(_a, _b);
}

/** Bound value between a minimum and a maximum. */
template <typename t_data_type>
constexpr std::pair<t_data_type, t_data_type> MinMax(const t_data_type& _a, const t_data_type& _b)
{
  return std::minmax(_a, _b);
}

/** Bound value between a minimum and a maximum. */
template <typename t_data_type>
constexpr t_data_type Bound(const t_data_type& _a, const t_data_type& _min, const t_data_type& _max)
{
  return _min < _max ? Max(Min(_a, _max), _min) : throw std::logic_error("The minimum bound must be lesser than the maximum bound.");
}

/***************************************************************************************************************************************************************
* Signum and Absolute Value Functions
***************************************************************************************************************************************************************/

/** Signum function. */
template <typename t_data_type>
constexpr t_data_type Sgn(const t_data_type& _value, const int _zero_sign = 1)
{
  switch(_zero_sign)
  {
    case -1: return _value > static_cast<t_data_type>(0) ? static_cast<t_data_type>(1) : static_cast<t_data_type>(-1);
    case 0: return (static_cast<t_data_type>(0) < _value) - (_value < static_cast<t_data_type>(0));
    case 1: return _value >= static_cast<t_data_type>(0) ? static_cast<t_data_type>(1) : static_cast<t_data_type>(-1);
    default: EXIT("Unrecognised sign for zero.")
  }
}

/** Absolute value. */
template <typename t_data_type>
constexpr t_data_type Abs(const t_data_type& _value)
{
  return _value < static_cast<t_data_type>(0) ? -_value : _value;
}

/***************************************************************************************************************************************************************
* Numerical Rounding Functions
***************************************************************************************************************************************************************/

/** Floor function. */
template <typename integer_type = int64_t>
constexpr Float Floor(const Float& _value)
{
  return static_cast<integer_type>(_value) - (static_cast<integer_type>(_value) > _value);
}

/** Ceiling function. */
template <typename integer_type = int64_t>
constexpr Float Ceil(const Float& _value)
{
  return static_cast<integer_type>(_value) + (static_cast<integer_type>(_value) < _value);
}

/** Rounding function. */
template <typename integer_type = int64_t>
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
