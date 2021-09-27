#pragma once

namespace Apeiron{

/***************************************************************************************************************************************************************
* Minimum, Maximum, Signum, Absolute Value, and Rounding Functions
***************************************************************************************************************************************************************/

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
  return (static_cast<data_type>(0) < _value) - (_value < static_cast<data_type>(0));
}

/** Absolute value. */
template <typename data_type>
data_type constexpr Abs(const data_type& _value)
{
  return _value < static_cast<data_type>(0) ? -_value : _value;
}

/** Floor function. */
template <typename integer_type = long long int>
constexpr long long int Floor(const Float& _value)
{
  return static_cast<integer_type>(_value) - (static_cast<integer_type>(_value) > _value);
}

/** Ceiling function. */
template <typename integer_type = long long int>
constexpr long long int Ceil(const Float& _value)
{
  return static_cast<integer_type>(_value) + (static_cast<integer_type>(_value) < _value);
}

}//Apeiron
