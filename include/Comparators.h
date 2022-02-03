#pragma once

#include "Constants.h"
#include "Debug.h"
#include "Types.h"
#include "BasicMath.h"

namespace Apeiron{

constexpr Float RelativeTolerance = Small;
constexpr Float ZeroTolerance = Small;

/** Floating-point toleranced/exact equality check. */
template<bool exact_precision = false>
constexpr bool isEqual(const Float& _a, const Float& _b, const Float& _relative_tol = RelativeTolerance, const Float& _zero_tol = ZeroTolerance)
{ return exact_precision ? _a == _b : Abs(_a - _b) < (Min(Abs(_a), Abs(_b)) == Zero ? _zero_tol : _relative_tol*Max(Abs(_a), Abs(_b))); }

/** Floating-point toleranced/exact 'is lesser than' check. */
template<bool exact_precision = false>
constexpr bool isLess(const Float& _a, const Float& _b, const Float& _relative_tol = RelativeTolerance, const Float& _zero_tol = ZeroTolerance)
{ return _a < _b && !isEqual<exact_precision>(_a, _b, _relative_tol, _zero_tol); }

/** Floating-point toleranced/exact 'is lesser than or equal to' check. */
template<bool exact_precision = false>
constexpr bool isLessEqual(const Float& _a, const Float& _b, const Float& _relative_tol = RelativeTolerance, const Float& _zero_tol = ZeroTolerance)
{ return _a <= _b || (!exact_precision && isEqual(_a, _b, _relative_tol, _zero_tol)); }

/** Floating-point toleranced/exact 'is larger than' check. */
template<bool exact_precision = false>
constexpr bool isLarger(const Float& _a, const Float& _b, const Float& _relative_tol = RelativeTolerance, const Float& _zero_tol = ZeroTolerance)
{ return !isLessEqual<exact_precision>(_a, _b, _relative_tol, _zero_tol); }

/** Floating-point toleranced/exact 'is lesser than or equal to' check. */
template<bool exact_precision = false>
constexpr bool isLargerEqual(const Float& _a, const Float& _b, const Float& _relative_tol = RelativeTolerance, const Float& _zero_tol = ZeroTolerance)
{ return !isLess<exact_precision>(_a, _b, _relative_tol, _zero_tol); }

/** Container size equality check. */
constexpr bool areSizesEqual(const std::size_t _size0, const std::size_t _size1) { return _size0 == _size1; }

/** Floating-point toleranced/exact boundedness check. */
template<bool is_left_incl = true, bool is_right_incl = false, bool exact_precision = false, typename T>
requires Arithmetic<T>
constexpr bool isBounded(const T& _a, const T& _min, const T& _max, const T& _relative_tol = RelativeTolerance, const Float& _zero_tol = ZeroTolerance)
{
  if constexpr(!isTypeEqual<T, Float>()) return (is_left_incl ? _min <= _a : _min < _a) && (is_right_incl ? _a <= _max : _a < _max);
  else return (is_left_incl  ? isLessEqual<exact_precision>(_min, _a, _relative_tol, _zero_tol) : isLess<exact_precision>(_min, _a, _relative_tol, _zero_tol)) &&
              (is_right_incl ? isLessEqual<exact_precision>(_a, _max, _relative_tol, _zero_tol) : isLess<exact_precision>(_a, _max, _relative_tol, _zero_tol));
}

}
