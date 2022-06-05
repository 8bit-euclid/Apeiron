#pragma once

#include "Constants.h"
#include "Types.h"

namespace aprn{

/***************************************************************************************************************************************************************
* Counting Functions
***************************************************************************************************************************************************************/
/** Count the number of digits in a natural number. */
constexpr size_t nDigits(std::convertible_to<size_t> auto _number)
{
   // Zero will have 1 digit.
   if(!_number) return 1;

   size_t n_digits(0);
   while(_number) { _number /= static_cast<size_t>(10); ++n_digits; }
   return n_digits;
}

/***************************************************************************************************************************************************************
* Min, Max, and Bounding Functions
***************************************************************************************************************************************************************/
/** Min value. */
template<typename T>
constexpr T Min(const T& _a, const T& _b) { return std::min(_a, _b); }

/** Max value. */
template<typename T>
constexpr T Max(const T& _a, const T& _b) { return std::max(_a, _b); }

/** Minmax values in a pair. */
template<typename T>
constexpr std::pair<T, T> MinMax(const T& _a, const T& _b) { return std::minmax(_a, _b); }

/** Bound value between a minimum and a maximum. */
template<typename T>
constexpr T Bound(const T& _value, const T& _min, const T& _max)
{ return _min < _max ? Max(Min(_value, _max), _min) : throw std::invalid_argument("The minimum bound must be lesser than the maximum bound."); }

/** Min value, given a first and last iterator. */
template<class iter>
constexpr IterType<iter>
MinEntry(const iter _first, const iter _last) { return *std::min_element(_first, _last); }

/** Max value, given a first and last iterator. */
template<class iter>
constexpr IterType<iter>
MaxEntry(const iter _first, const iter _last) { return *std::max_element(_first, _last); }

/** Minmax values in a pair, given a first and last iterator. */
template<class iter>
constexpr std::pair<IterType<iter>, IterType<iter>>
MinMaxEntries(const iter _first, const iter _last)
{
  auto [min_iter, max_iter] = std::minmax_element(_first, _last);
  return {*min_iter, *max_iter};
}

/** Bound values from a first to a last iterator between a minimum and a maximum. */
template<class iter, typename T>
constexpr void
BoundEntries(iter _first, iter _last, const T& _min, const T& _max) { for(auto it = _first; it != _last; ++it) *it = Bound(*it, _min, _max); }

/***************************************************************************************************************************************************************
* Signum and Absolute Value Functions
***************************************************************************************************************************************************************/
/** Signum function. */
template<typename T>
constexpr T
Sgn(const T& _value, const int _zero_sign = 1)
{
  switch(_zero_sign)
  {
    case -1: return _value > static_cast<T>(0) ? static_cast<T>(1) : static_cast<T>(-1);
    case  0: return (static_cast<T>(0) < _value) - (_value < static_cast<T>(0));
    case  1: return _value >= static_cast<T>(0) ? static_cast<T>(1) : static_cast<T>(-1);
    default: EXIT("Unrecognised sign for zero.")
  }
}

/** Check if a value is positive. */
template<typename T>
constexpr bool
isPositive(const T& _value, const int _zero_sign = 1)
{ return _zero_sign != 0 ? Sgn(_value, _zero_sign) > 0 : throw std::invalid_argument("Zero must be either positive or negative."); }

/** Check if a value is negative. */
template<typename T>
constexpr bool
isNegative(const T& _value, const int _zero_sign = 1)
{ return _zero_sign != 0 ? Sgn(_value, _zero_sign) < 0 : throw std::invalid_argument("Zero must be either positive or negative."); }

/** Absolute value. */
template<typename T>
constexpr T
Abs(const T& _value) { return _value < static_cast<T>(0) ? -_value : _value; }

/***************************************************************************************************************************************************************
* Numerical Rounding Functions
***************************************************************************************************************************************************************/
/** Floor function. */
template<typename integer_T = int64_t>
constexpr Float
Floor(const Float& _value) { return static_cast<integer_T>(_value) - (static_cast<integer_T>(_value) > _value); }

/** Ceiling function. */
template<typename integer_T = int64_t>
constexpr Float
Ceil(const Float& _value) { return static_cast<integer_T>(_value) + (static_cast<integer_T>(_value) < _value); }

/** Rounding function. */
template<typename integer_T = int64_t>
constexpr Float
Round(const Float& _value) { return _value < Floor(_value) + Half ? Floor(_value) : Ceil(_value); }

/***************************************************************************************************************************************************************
* Mathematical Conversions
***************************************************************************************************************************************************************/
/** Convert degrees to radians. */
constexpr Float
DegToRad(const Float& angle_degrees) { return angle_degrees * Pi / 180.0; }

/** Convert radians to degrees. */
constexpr Float
RadToDeg(const Float& angle_radians) { return angle_radians * 180.0 / Pi; }

}
