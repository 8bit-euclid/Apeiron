#pragma once

namespace Apeiron{

/** Type Categories. */
enum class TypeCategory
{
  Boolean,
  Integral,
  FloatingPoint,
  String,
  Other
};

/***************************************************************************************************************************************************************
* Boolean Type Wrapper (for std::vector)
***************************************************************************************************************************************************************/

class Bool
{
  private:
  bool Value;

  public:
  constexpr Bool() : Bool(false) {}
  constexpr Bool(const bool _value) : Value(_value) {}

  constexpr operator bool() const { return Value; }
};

constexpr Bool True(true);
constexpr Bool False(false);

/***************************************************************************************************************************************************************
* Integral Types
***************************************************************************************************************************************************************/

/** Char type definitions. */
typedef char Char;
typedef unsigned char UChar;

/** Signed integer type definitions. */
typedef int Int;
typedef long int LInt;
typedef long long int LLInt;
typedef int8_t Int8;
typedef int16_t Int16;
typedef int32_t Int32;
typedef int64_t Int64;

/** Unsigned integer type definitions. */
typedef unsigned int UInt;
typedef unsigned long int ULInt;
typedef unsigned long long int ULLInt;
typedef uint8_t UInt8;
typedef uint16_t UInt16;
typedef uint32_t UInt32;
typedef uint64_t UInt64;

/***************************************************************************************************************************************************************
* Floating-point Types
***************************************************************************************************************************************************************/

/** Machine epsion and definition of floating point accuracy. */
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

/** Check if a value is NaN. */
template<typename T>
constexpr bool isNaN(const T _value = T()) { return std::isnan(_value); }

/** Check if a value is infinity. */
template<typename T>
constexpr bool isInfinity(const T _value = T()) { return std::isinf(_value); }

/***************************************************************************************************************************************************************
* Type Checking
***************************************************************************************************************************************************************/

/** Check if two data types are the same. */
template<class T1, class T2>
constexpr bool isTypeEqual()
{
  return std::is_same_v<T1, T2>;
}

/** Check if the data types of a sequence of values are the same. */
template<typename T, class ...T_seq>
constexpr bool areAllTypesEqual()
{
  return (isTypeEqual<T, T_seq>() && ...);
}

/** Check if the data type is a boolean type. */
template<typename T>
constexpr bool isBoolean(const T& _value = T())
{
  return isTypeEqual<T, bool>() || isTypeEqual<T, Bool>();
}

/** Check if the data type is an integer type. Note: does not include booleans. */
template<typename T>
constexpr bool isIntegral(const T& _value = T())
{
  return std::is_integral_v<T> && !isBoolean(_value);
}

/** Check if the data type is a floating-point type. */
template<typename T>
constexpr bool isFloatingPoint(const T& _value = T())
{
  return std::is_floating_point_v<T>;
}

/** Check if the data type is a number type (floating-point or integer type). */
template<typename T>
constexpr bool isNumber(const T& _value = T())
{
  return isIntegral(_value) || isFloatingPoint(_value);
}

/** Check if the data type is a number type (floating-point or integer type). */
template<typename T>
constexpr bool isString(const T& _value = T())
{
  return isTypeEqual<T, char*>() || isTypeEqual<T, std::string>();
}

/** Get the type category of the given data type (integer, floating-point, etc.). */
template<typename T>
constexpr TypeCategory GetTypeCategory(const T& _value = T())
{
  return isBoolean<T>() ? TypeCategory::Boolean :
         isIntegral<T>() ? TypeCategory::Integral :
         isFloatingPoint<T>() ? TypeCategory::FloatingPoint :
         isString<T>() ? TypeCategory::String :
         TypeCategory::Other;
}

/** Get the initial value for each type category. */
template<typename T, TypeCategory category = GetTypeCategory<T>()>
constexpr T GetStaticInitValue()
{
  return category == TypeCategory::Boolean ? static_cast<T>(false) :
         category == TypeCategory::Integral ? static_cast<T>(-1) :
         category == TypeCategory::FloatingPoint ? static_cast<T>(0.0) :
         category == TypeCategory::Other ? T() :
         throw std::invalid_argument("The passed type does not qualify for static initialisation.");
}

/** Get the initial value for each type category. */
template<typename T, TypeCategory category = GetTypeCategory<T>()>
T GetDynamicInitValue()
{
  return category == TypeCategory::Boolean ? static_cast<T>(false) :
         category == TypeCategory::Integral ? static_cast<T>(-1) :
         category == TypeCategory::FloatingPoint ? static_cast<T>(0.0) :
         category == TypeCategory::String ? static_cast<T>('\0') :
         category == TypeCategory::Other ? T() :
         throw std::invalid_argument("The passed type does not qualify for dynamic initialisation.");
}

}
