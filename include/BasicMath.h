#pragma once

#include "Constants.h"

namespace Apeiron{

/***************************************************************************************************************************************************************
* Minimum, Maximum, and Bounding Functions
***************************************************************************************************************************************************************/

/** Minimum value. */
template <typename data_type>
constexpr data_type Min(const data_type& _a, const data_type& _b)
{
  return std::min(_a, _b);
}

/** Maximum value. */
template <typename data_type>
constexpr data_type Max(const data_type& _a, const data_type& _b)
{
  return std::max(_a, _b);
}

/** Bound value between a minimum and a maximum. */
template <typename data_type>
constexpr std::pair<data_type, data_type> MinMax(const data_type& _a, const data_type& _b)
{
  return std::minmax(_a, _b);
}

/** Bound value between a minimum and a maximum. */
template <typename data_type>
constexpr data_type Bound(const data_type& _a, const data_type& _min, const data_type& _max)
{
  return _min < _max ? Max(Min(_a, _max), _min) : throw std::logic_error("The minimum bound must be lesser than the maximum bound.");
}

/***************************************************************************************************************************************************************
* Signum and Absolute Value Functions
***************************************************************************************************************************************************************/

/** Signum function. */
template <typename data_type>
constexpr data_type Sgn(const data_type& _value)
{
  return (static_cast<data_type>(0) < _value) - (_value < static_cast<data_type>(0));
}

/** Absolute value. */
template <typename data_type>
constexpr data_type Abs(const data_type& _value)
{
  return _value < static_cast<data_type>(0) ? -_value : _value;
}

/***************************************************************************************************************************************************************
* Numerical Rounding Functions
***************************************************************************************************************************************************************/

/** Floor function. */
template <typename integer_type = uint64_t>
constexpr Float Floor(const Float& _value)
{
  return static_cast<integer_type>(_value) - (static_cast<integer_type>(_value) > _value);
}

/** Ceiling function. */
template <typename integer_type = uint64_t>
constexpr Float Ceil(const Float& _value)
{
  return static_cast<integer_type>(_value) + (static_cast<integer_type>(_value) < _value);
}

/** Rounding function. */
template <typename integer_type = uint64_t>
constexpr Float Round(const Float& _value)
{
  return _value < Floor(_value) + Half ? Floor(_value) : Ceil(_value);
}

}//Apeiron
