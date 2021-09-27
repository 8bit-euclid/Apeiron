#pragma once

namespace Apeiron{

#define DOUBLE_PRECISION
//#define LONG_DOUBLE_PRECISION

// Machine epsion and defintion of floating point accuracy.
#if defined(DOUBLE_PRECISION)
typedef double Float;
constexpr Float Epsilon(DBL_EPSILON);
#elif defined(LONG_DOUBLE_PRECISION)
typedef long double FLOAT;
  constexpr Float Epsilon(LDBL_EPSILON);
#else
  typedef double FLOAT;
  constexpr Float Epsilon(1);
#error No numerical precision defined.
#endif

}//Apeiron
