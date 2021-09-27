#pragma once

#include "Constants.h"
#include "Debug.h"
#include "FloatingPoint.h"
#include "BasicMath.h"

namespace Apeiron{

constexpr Float RelativeTolerance = Small;
constexpr Float AbsoluteTolerance = TenthSmall;

/** Floating-point toleranced/exact equality check. */
template<bool is_exact_comparison = false>
constexpr bool isEqual(const Float& _a, const Float& _b, const Float& _relative_tolerance = RelativeTolerance,
                       const Float& _absolute_tolerance = AbsoluteTolerance)
{
  return is_exact_comparison ? _a == _b : Abs(_a - _b) < Max(_absolute_tolerance, _relative_tolerance*Max(_a, _b));
}

/** Floating-point toleranced/exact 'is lesser than' check. */
template<bool is_exact_comparison = false>
constexpr bool isLess(const Float& _a, const Float& _b, const Float& _relative_tolerance = RelativeTolerance,
                      const Float& _absolute_tolerance = AbsoluteTolerance)
{
  return _a < _b && !isEqual(_a, _b, _relative_tolerance, _absolute_tolerance);
}

/** Floating-point toleranced/exact 'is lesser than or equal to' check. */
template<bool is_exact_comparison = false>
constexpr bool isLessEqual(const Float& _a, const Float& _b, const Float& _relative_tolerance = RelativeTolerance,
                           const Float& _absolute_tolerance = AbsoluteTolerance)
{
  return _a <= _b || (is_exact_comparison ? true : isEqual(_a, _b, _relative_tolerance, _absolute_tolerance));
}

/** Floating-point toleranced/exact 'is larger than' check. */
template<bool is_exact_comparison = false>
constexpr bool isLarg(const Float& _a, const Float& _b, const Float& _relative_tolerance = RelativeTolerance,
                      const Float& _absolute_tolerance = AbsoluteTolerance)
{
  return !isLessEqual<is_exact_comparison>(_a, _b, _relative_tolerance, _absolute_tolerance);
}

/** Floating-point toleranced/exact 'is lesser than or equal to' check. */
template<bool is_exact_comparison = false>
constexpr bool isLargEqual(const Float& _a, const Float& _b, const Float& _relative_tolerance = RelativeTolerance,
                           const Float& _absolute_tolerance = AbsoluteTolerance)
{
  return !isLess<is_exact_comparison>(_a, _b, _relative_tolerance, _absolute_tolerance);
}

/** Floating-point toleranced/exact boundedness check. */
template<bool is_left_included = true, bool is_right_included = false, bool is_exact_comparison = false, typename data_type>
constexpr bool isBounded(const data_type& _a, const data_type& _min, const data_type& _max, const data_type& _relative_tolerance = RelativeTolerance,
                      const Float& _absolute_tolerance = AbsoluteTolerance)
{
  ASSERT_NUMBER(_a)

  if(typeid(data_type) != typeid(Float))
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
            return isLessEqual<is_exact_comparison>(_min, _a, _relative_tolerance, _absolute_tolerance) &&
                   isLessEqual<is_exact_comparison>(_a, _max, _relative_tolerance, _absolute_tolerance);
          case false:
            return isLessEqual<is_exact_comparison>(_min, _a, _relative_tolerance, _absolute_tolerance) &&
                   isLess<is_exact_comparison>(_a, _max, _relative_tolerance, _absolute_tolerance);
        }
      case false:
        switch(is_right_included)
        {
          case true:
            return isLess<is_exact_comparison>(_min, _a, _relative_tolerance, _absolute_tolerance) &&
                   isLessEqual<is_exact_comparison>(_a, _max, _relative_tolerance, _absolute_tolerance);
          case false:
            return isLess<is_exact_comparison>(_min, _a, _relative_tolerance, _absolute_tolerance) &&
                   isLess<is_exact_comparison>(_a, _max, _relative_tolerance, _absolute_tolerance);
        }
    }
  }
}

}//Apeiron
