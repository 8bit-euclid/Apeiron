#pragma once

namespace Apeiron{

#define DOUBLE_PRECISION
//#define LONG_DOUBLE_PRECISION

// Machine epsion and defintion of floating point accuracy.
#if defined(DOUBLE_PRECISION)
typedef double Float;
#elif defined(LONG_DOUBLE_PRECISION)
typedef long double Float;
#else
  typedef double Float;
#error No numerical precision defined.
#endif

constexpr Float Epsilon(std::numeric_limits<Float>::epsilon());
constexpr Float FloatMin(std::numeric_limits<Float>::min());
constexpr Float FloatMax(std::numeric_limits<Float>::max());
constexpr Float FloatLowest(std::numeric_limits<Float>::lowest());
constexpr Float Infinity(std::numeric_limits<Float>::infinity());
constexpr Float QuietNaN(std::numeric_limits<Float>::quiet_NaN());
constexpr Float SignalNaN(std::numeric_limits<Float>::signaling_NaN());

}//Apeiron
