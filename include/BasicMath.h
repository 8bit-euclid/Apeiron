/***************************************************************************************************************************************************************
* GPL-3.0 License
* Copyright (C) 2022 Niran A. Ilangakoon
*
* This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as
* published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
* of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program.
* If not, see <https://www.gnu.org/licenses/>.
***************************************************************************************************************************************************************/

#pragma once

#include "Constants.h"
#include "Types.h"

namespace aprn{

/***************************************************************************************************************************************************************
* Counting Functions
***************************************************************************************************************************************************************/
/** Count the number of digits in a natural number. */
constexpr size_t nDigits(std::convertible_to<size_t> auto number)
{
   // Zero will have 1 digit.
   if(!number) return 1;

   size_t n_digits(0);
   while(number) { number /= static_cast<size_t>(10); ++n_digits; }
   return n_digits;
}

/***************************************************************************************************************************************************************
* Min, Max, and Bounding Functions
***************************************************************************************************************************************************************/
/** Get minimum value. */
template<typename T>
constexpr T
Min(const T& a, const T& b) { return std::min(a, b); }

/** Get maximum value. */
template<typename T>
constexpr T
Max(const T& a, const T& b) { return std::max(a, b); }

/** Minimise value. */
template<typename T>
constexpr void
Minimise(T& value, const T& other) { value = Min(value, other); }

/** Maximise value. */
template<typename T>
constexpr void
Maximise(T& value, const T& other) { value = Max(value, other); }

/** Min/Max values in a pair of values. */
template<typename T>
constexpr std::pair<T, T>
MinMax(const T& a, const T& b) { return std::minmax(a, b); }

/** Get the clipped value between a minimum and a maximum. */
template<typename T>
constexpr T
Clipped(const T& value, const T& min, const T& max)
{
   return min < max ? std::clamp(value, min, max) : throw std::invalid_argument("The minimum bound must be lesser than the maximum bound.");
}

/** Clip a value between a minimum and a maximum. */
template<typename T>
constexpr void
Clip(T& value, const T& min, const T& max) { value = Clipped(value, min, max); }

/** Min value, given a first and last iterator. */
template<class iter>
constexpr IterType<iter>
MinEntry(const iter first, const iter last) { return *std::min_element(first, last); }

/** Max value, given a first and last iterator. */
template<class iter>
constexpr IterType<iter>
MaxEntry(const iter first, const iter last) { return *std::max_element(first, last); }

/** Minmax values in a pair, given a first and last iterator. */
template<class iter>
constexpr Pair<IterType<iter>>
MinMaxEntries(const iter first, const iter last)
{
  auto [min_iter, max_iter] = std::minmax_element(first, last);
  return { *min_iter, *max_iter };
}

/** Clipped values from a first to a last iterator between a minimum and a maximum. */
template<class iter, typename T>
constexpr void
BoundEntries(iter first, iter last, const T& min, const T& max) { FOR_ITER(it, first, last) Clip(*it, min, max); }

/***************************************************************************************************************************************************************
* Signum and Absolute Value Functions
***************************************************************************************************************************************************************/
/** Signum function. */
template<typename T>
constexpr T
Sgn(const T& value, const int zero_sign = 1)
{
  switch(zero_sign)
  {
    case -1: return value > static_cast<T>(0) ? static_cast<T>(1) : static_cast<T>(-1);
    case  0: return (static_cast<T>(0) < value) - (value < static_cast<T>(0));
    case  1: return value >= static_cast<T>(0) ? static_cast<T>(1) : static_cast<T>(-1);
    default: EXIT("Unrecognised sign for zero.")
  }
}

/** Init if a value is positive. */
template<typename T>
constexpr bool
isPositive(const T& value, const int zero_sign = 1)
{
   return zero_sign != 0 ? Sgn(value, zero_sign) > 0 : throw std::invalid_argument("Zero must be either positive or negative.");
}

/** Init if a value is negative. */
template<typename T>
constexpr bool
isNegative(const T& value, const int zero_sign = 1)
{
   return zero_sign != 0 ? Sgn(value, zero_sign) < 0 : throw std::invalid_argument("Zero must be either positive or negative.");
}

/** Absolute value. */
template<typename T>
constexpr T
Abs(const T& value) { return value < static_cast<T>(0) ? -value : value; }

/***************************************************************************************************************************************************************
* Numerical Rounding Functions
***************************************************************************************************************************************************************/
/** Floor function. */
template<typename integer_T = int64_t>
constexpr Float
Floor(const Float& value) { return static_cast<integer_T>(value) - (static_cast<integer_T>(value) > value); }

/** Ceiling function. */
template<typename integer_T = int64_t>
constexpr Float
Ceil(const Float& value) { return static_cast<integer_T>(value) + (static_cast<integer_T>(value) < value); }

/** Rounding function. */
template<typename integer_T = int64_t>
constexpr Float
Round(const Float& value) { return value < Floor(value) + Half ? Floor(value) : Ceil(value); }

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
