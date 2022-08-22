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
#include "Debug.h"
#include "Types.h"
#include "BasicMath.h"

namespace aprn {

constexpr Float RelativeTolerance = Small;
constexpr Float ZeroTolerance = Small;

/** Floating-point toleranced/exact equality check. */
template<bool exact_precision = false>
constexpr bool isEqual(const Float& a, const Float& b, const Float& _relative_tol = RelativeTolerance, const Float& _zero_tol = ZeroTolerance)
{ return exact_precision ? a == b : Abs(a - b) < (Min(Abs(a), Abs(b)) == Zero ? _zero_tol : _relative_tol*Max(Abs(a), Abs(b))); }

/** Floating-point toleranced/exact 'is lesser than' check. */
template<bool exact_precision = false>
constexpr bool isLess(const Float& a, const Float& b, const Float& _relative_tol = RelativeTolerance, const Float& _zero_tol = ZeroTolerance)
{ return a < b && !isEqual<exact_precision>(a, b, _relative_tol, _zero_tol); }

/** Floating-point toleranced/exact 'is lesser than or equal to' check. */
template<bool exact_precision = false>
constexpr bool isLessEqual(const Float& a, const Float& b, const Float& _relative_tol = RelativeTolerance, const Float& _zero_tol = ZeroTolerance)
{ return a <= b || (!exact_precision && isEqual(a, b, _relative_tol, _zero_tol)); }

/** Floating-point toleranced/exact 'is larger than' check. */
template<bool exact_precision = false>
constexpr bool isLarger(const Float& a, const Float& b, const Float& _relative_tol = RelativeTolerance, const Float& _zero_tol = ZeroTolerance)
{ return !isLessEqual<exact_precision>(a, b, _relative_tol, _zero_tol); }

/** Floating-point toleranced/exact 'is lesser than or equal to' check. */
template<bool exact_precision = false>
constexpr bool isLargerEqual(const Float& a, const Float& b, const Float& _relative_tol = RelativeTolerance, const Float& _zero_tol = ZeroTolerance)
{ return !isLess<exact_precision>(a, b, _relative_tol, _zero_tol); }

/** Container size equality check. */
constexpr bool areSizesEqual(const size_t _size0, const size_t _size1) { return _size0 == _size1; }

/** Floating-point toleranced/exact boundedness check. */
template<bool is_left_incl = true, bool is_right_incl = false, bool exact_precision = false, typename T>
requires Arithmetic<T>
constexpr bool isBounded(const T& a, const T& min, const T& max, const T& _relative_tol = RelativeTolerance, const Float& _zero_tol = ZeroTolerance)
{
  if constexpr(!isTypeSame<T, Float>()) return (is_left_incl ? min <= a : min < a) && (is_right_incl ? a <= max : a < max);
  else return (is_left_incl  ? isLessEqual<exact_precision>(min, a, _relative_tol, _zero_tol) : isLess<exact_precision>(min, a, _relative_tol, _zero_tol)) &&
              (is_right_incl ? isLessEqual<exact_precision>(a, max, _relative_tol, _zero_tol) : isLess<exact_precision>(a, max, _relative_tol, _zero_tol));
}

}
