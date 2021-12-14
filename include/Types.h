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
template <class t_data_type>
constexpr bool isNaN(const t_data_type _value = t_data_type())
{
  return std::isnan(_value);
}

/** Check if a value is infinity. */
template <class t_data_type>
constexpr bool isInfinity(const t_data_type _value = t_data_type())
{
  return std::isinf(_value);
}

/***************************************************************************************************************************************************************
* Type Checking
***************************************************************************************************************************************************************/

/** Check if two data types are the same. */
template <class data_type_a, class data_type_b>
constexpr bool isTypeEqual()
{
  return std::is_same_v<data_type_a, data_type_b>;
}

/** Check if the data types of a sequence of values are the same. */
template <class t_data_type, class ...t_values>
constexpr bool areAllTypesEqual()
{
  return (isTypeEqual<t_data_type, t_values>() && ...);
}

/** Check if the data type is a boolean type. */
template<class t_data_type>
constexpr bool isBoolean(const t_data_type& _value = t_data_type())
{
  return isTypeEqual<t_data_type, bool>() || isTypeEqual<t_data_type, Bool>();
}

/** Check if the data type is an integer type. Note: does not include booleans. */
template<class t_data_type>
constexpr bool isIntegral(const t_data_type& _value = t_data_type())
{
  return std::is_integral_v<t_data_type> && !isBoolean(_value);
}

/** Check if the data type is a floating-point type. */
template<class t_data_type>
constexpr bool isFloatingPoint(const t_data_type& _value = t_data_type())
{
  return std::is_floating_point_v<t_data_type>;
}

/** Check if the data type is a number type (floating-point or integer type). */
template<class t_data_type>
constexpr bool isNumber(const t_data_type& _value = t_data_type())
{
  return isIntegral(_value) || isFloatingPoint(_value);
}

/** Check if the data type is a number type (floating-point or integer type). */
template<class t_data_type>
constexpr bool isString(const t_data_type& _value = t_data_type())
{
  return isTypeEqual<t_data_type, char*>() || isTypeEqual<t_data_type, std::string>();
}

/** Get the type category of the given data type (integer, floating-point, etc.). */
template<class t_data_type>
constexpr TypeCategory GetTypeCategory(const t_data_type& _value = t_data_type())
{
  return isBoolean<t_data_type>() ? TypeCategory::Boolean :
         isIntegral<t_data_type>() ? TypeCategory::Integral :
         isFloatingPoint<t_data_type>() ? TypeCategory::FloatingPoint :
         isString<t_data_type>() ? TypeCategory::String :
         TypeCategory::Other;
}

/** Get the initial value for each type category. */
template <class t_data_type, TypeCategory t_type_category = GetTypeCategory<t_data_type>()>
constexpr t_data_type GetStaticInitValue()
{
  return t_type_category == TypeCategory::Boolean ? static_cast<t_data_type>(false) :
         t_type_category == TypeCategory::Integral ? static_cast<t_data_type>(-1) :
         t_type_category == TypeCategory::FloatingPoint ? static_cast<t_data_type>(0.0) :
         t_type_category == TypeCategory::Other ? t_data_type() :
         throw std::invalid_argument("The passed type does not qualify for static initialisation.");
}

/** Get the initial value for each type category. */
template <class t_data_type, TypeCategory t_type_category = GetTypeCategory<t_data_type>()>
t_data_type GetDynamicInitValue()
{
  return t_type_category == TypeCategory::Boolean ? static_cast<t_data_type>(false) :
         t_type_category == TypeCategory::Integral ? static_cast<t_data_type>(-1) :
         t_type_category == TypeCategory::FloatingPoint ? static_cast<t_data_type>(0.0) :
         t_type_category == TypeCategory::String ? static_cast<t_data_type>('\0') :
         t_type_category == TypeCategory::Other ? t_data_type() :
         throw std::invalid_argument("The passed type does not qualify for dynamic initialisation.");
}

}
