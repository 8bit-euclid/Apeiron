#pragma once

#include "Constants.h"
#include "Debug.h"
#include "Types.h"
#include "BasicMath.h"

namespace Apeiron{

constexpr Float RelativeTolerance = Small;
constexpr Float ZeroTolerance = Small;

/** Floating-point toleranced/exact equality check. */
template<bool is_exact_comparison = false>
constexpr bool isEqual(const Float& _a, const Float& _b, const Float& _relative_tolerance = RelativeTolerance,
                       const Float& _zero_tolerance = ZeroTolerance)
{
  return is_exact_comparison ? _a == _b : Abs(_a - _b) < (Min(Abs(_a), Abs(_b)) == Zero ? _zero_tolerance : _relative_tolerance*Max(_a, _b));
}

/** Floating-point toleranced/exact 'is lesser than' check. */
template<bool is_exact_comparison = false>
constexpr bool isLess(const Float& _a, const Float& _b, const Float& _relative_tolerance = RelativeTolerance,
                      const Float& _zero_tolerance = ZeroTolerance)
{
  return _a < _b && !isEqual(_a, _b, _relative_tolerance, _zero_tolerance);
}

/** Floating-point toleranced/exact 'is lesser than or equal to' check. */
template<bool is_exact_comparison = false>
constexpr bool isLessEqual(const Float& _a, const Float& _b, const Float& _relative_tolerance = RelativeTolerance,
                           const Float& _zero_tolerance = ZeroTolerance)
{
  return _a <= _b || (is_exact_comparison ? true : isEqual(_a, _b, _relative_tolerance, _zero_tolerance));
}

/** Floating-point toleranced/exact 'is larger than' check. */
template<bool is_exact_comparison = false>
constexpr bool isLarger(const Float& _a, const Float& _b, const Float& _relative_tolerance = RelativeTolerance,
                        const Float& _zero_tolerance = ZeroTolerance)
{
  return !isLessEqual<is_exact_comparison>(_a, _b, _relative_tolerance, _zero_tolerance);
}

/** Floating-point toleranced/exact 'is lesser than or equal to' check. */
template<bool is_exact_comparison = false>
constexpr bool isLargerEqual(const Float& _a, const Float& _b, const Float& _relative_tolerance = RelativeTolerance,
                             const Float& _zero_tolerance = ZeroTolerance)
{
  return !isLess<is_exact_comparison>(_a, _b, _relative_tolerance, _zero_tolerance);
}

/** Floating-point toleranced/exact boundedness check. */
template<bool is_left_included = true, bool is_right_included = false, bool is_exact_comparison = false, typename t_data_type>
constexpr bool isBounded(const t_data_type& _a, const t_data_type& _min, const t_data_type& _max, const t_data_type& _relative_tolerance = RelativeTolerance,
                         const Float& _zero_tolerance = ZeroTolerance)
{
  ASSERT_NUMBER(_a)

  if(!areTypesEqual<t_data_type, Float>())
  {
    switch(is_left_included)
    {
      case true:
        switch(is_right_included)
        {
          case true:
            return _min <= _a && _a <= _max;
          case false:
            return _min <= _a && _a < _max;
        }
      case false:
        switch(is_right_included)
        {
          case true:
            return _min < _a && _a <= _max;
          case false:
            return _min < _a && _a < _max;
        }
    }
  }
  else
  {
    switch(is_left_included)
    {
      case true:
        switch(is_right_included)
        {
          case true:
            return isLessEqual<is_exact_comparison>(_min, _a, _relative_tolerance, _zero_tolerance) &&
                   isLessEqual<is_exact_comparison>(_a, _max, _relative_tolerance, _zero_tolerance);
          case false:
            return isLessEqual<is_exact_comparison>(_min, _a, _relative_tolerance, _zero_tolerance) &&
                   isLess<is_exact_comparison>(_a, _max, _relative_tolerance, _zero_tolerance);
        }
      case false:
        switch(is_right_included)
        {
          case true:
            return isLess<is_exact_comparison>(_min, _a, _relative_tolerance, _zero_tolerance) &&
                   isLessEqual<is_exact_comparison>(_a, _max, _relative_tolerance, _zero_tolerance);
          case false:
            return isLess<is_exact_comparison>(_min, _a, _relative_tolerance, _zero_tolerance) &&
                   isLess<is_exact_comparison>(_a, _max, _relative_tolerance, _zero_tolerance);
        }
    }
  }
}

/** Container size equality check. */
constexpr bool areSizesEqual(const std::size_t& _size0, const std::size_t& _size1)
{
  return _size0 == _size1;
}

}
