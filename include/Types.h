#pragma once

namespace Apeiron{

enum class TypeCategory
{
  Integer,
  FloatingPoint
};

// Signed integer type definitions.
typedef int Int;
typedef long int LInt;
typedef long long int LLInt;
typedef int8_t Int8;
typedef int16_t Int16;
typedef int32_t Int32;
typedef int64_t Int64;

// Unsigned integer type definitions.
typedef unsigned int UInt;
typedef unsigned long int ULInt;
typedef unsigned long long int ULLInt;
typedef uint8_t UInt8;
typedef uint16_t UInt16;
typedef uint32_t UInt32;
typedef uint64_t UInt64;

// Machine epsion and definition of floating point accuracy.
#define DOUBLE_PRECISION
//#define LONG_DOUBLE_PRECISION

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

template <typename data_type_a, typename data_type_b>
constexpr bool isTypeSame()
{
  return std::is_same_v<data_type_a, data_type_b>;
}

template<typename data_type>
constexpr bool isUnsignedInteger(const data_type& _value = data_type())
{
  return isTypeSame<data_type, unsigned int>() || isTypeSame<data_type, unsigned short int>() ||
         isTypeSame<data_type, unsigned long int>() || isTypeSame<data_type, unsigned long long int>();
}

template<typename data_type>
constexpr bool isSignedInteger(const data_type& _value = data_type())
{
  return isTypeSame<data_type, int>() || isTypeSame<data_type, short int>() || isTypeSame<data_type, long int>() || isTypeSame<data_type, long long int>();
}

template<typename data_type>
constexpr bool isInteger(const data_type& _value = data_type())
{
  return isSignedInteger(_value) || isUnsignedInteger(_value);
}

template<typename data_type>
constexpr bool isFloat(const data_type& _value = data_type())
{
  return isTypeSame<data_type, Float>() || isTypeSame<data_type, float>() || isTypeSame<data_type, double>() || isTypeSame<data_type, long double>();
}

template<typename data_type>
constexpr bool isNumber(const data_type& _value = data_type())
{
  return isInteger(_value) || isFloat(_value);
}

template<typename data_type>
constexpr TypeCategory GetTypeCategory(const data_type& _value = data_type())
{
  return isInteger(_value) ? TypeCategory::Integer : isFloat(_value) ? TypeCategory::FloatingPoint : throw "Unrecognised type passed in.";
}

}//Apeiron
