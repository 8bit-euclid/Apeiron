//
// Created by niran90 on 2021/09/26.
//

#ifndef COMPARATORS_H
#define COMPARATORS_H

#include "Debug.h"
#include "FloatingPoint.h"
#include "Math.h"

/** Floating-point toleranced/exact equality check. */
template <bool is_exact_comparison = false>
inline bool isEqual(const Float& _a, const Float& _b, const Float& _tolerance = Small)
{
  return is_exact_comparison ? _a == _b : Abs(_a - _b) < _tolerance*Max(One, Max(_a, _b));
}

/** Floating-point toleranced/exact 'is lesser than' check. */
template <bool is_exact_comparison = false>
inline bool isLess(const Float& _a, const Float& _b, const Float& _tolerance = Small)
{
  return _a < _b && !isEqual(_a, _b, _tolerance);
}

/** Floating-point toleranced/exact 'is lesser than or equal to' check. */
template <bool is_exact_comparison = false>
inline bool isLessEqual(const Float& _a, const Float& _b, const Float& _tolerance = Small)
{
  return _a <= _b || (is_exact_comparison ? true : isEqual(_a, _b, _tolerance));
}

/** Floating-point toleranced/exact 'is larger than' check. */
template <bool is_exact_comparison = false>
inline bool isLarg(const Float& _a, const Float& _b, const Float& _tolerance = Small)
{
  return !isLessEqual<is_exact_comparison>(_a, _b, _tolerance);
}

/** Floating-point toleranced/exact 'is lesser than or equal to' check. */
template <bool is_exact_comparison = false>
inline bool isLargEqual(const Float& _a, const Float& _b, const Float& _tolerance = Small)
{
  return !isLess<is_exact_comparison>(_a, _b, _tolerance);
}

/** Floating-point toleranced/exact boundedness check. */
template <bool is_left_included = true, bool is_right_included = false, bool is_exact_comparison = false, typename data_type>
inline bool isBounded(const data_type& _a, const data_type& _min, const data_type& _max, const data_type& _tolerance = Small)
{
  ASSERT_NUMBER(_a)

  if(typeid(data_type) != typeid(Float))
  {
    switch(is_left_included)
    {
      case true:
        switch(is_right_included)
        {
          case true: return _min <= _a && _a <= _max;
          case false: return _min <= _a && _a < _max;
        }
      case false:
        switch(is_right_included)
        {
          case true: return _min < _a && _a <= _max;
          case false: return _min < _a && _a < _max;
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
          case true: return isLessEqual<is_exact_comparison>(_min, _a) && isLessEqual<is_exact_comparison>(_a, _max);
          case false: return isLessEqual<is_exact_comparison>(_min, _a) && isLess<is_exact_comparison>(_a, _max);
        }
      case false:
        switch(is_right_included)
        {
          case true: return isLess<is_exact_comparison>(_min, _a) && isLessEqual<is_exact_comparison>(_a, _max);
          case false: return isLess<is_exact_comparison>(_min, _a) && isLess<is_exact_comparison>(_a, _max);
        }
    }
  }
}

#endif //COMPARATORS_H
