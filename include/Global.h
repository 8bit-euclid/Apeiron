#ifndef GLOBAL_H
#define GLOBAL_H

/** Run-time macros. */
//#define PARALLEL

/** Debugging macros. */
#define DEBUG_MODE
#define STACK_TRACE_SIZE 10

#include "Standard_Library.h"

namespace Apeiron
{

/** Dimension. */
constexpr int nDIM(2);

/** Macro expansions. */
#define EXPAND(x) x
#define PP_ARG_N(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...) N
#define ARG_COUNT(...) EXPAND(PP_ARG_N(__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0))

/** General definition for any macro name */
#define _MACRO_(name, n) name##n
#define _MACRO(name, n) _MACRO_(name, n)
#define MACRO(func, ...) _MACRO(func, ARG_COUNT(__VA_ARGS__))(__VA_ARGS__)

/***************************************************************************************************************************************************************
* Floating-point number precision
***************************************************************************************************************************************************************/

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

constexpr Float Small(Epsilon);
constexpr Float Zero(0.0);
constexpr Float Half(0.5);
constexpr Float Third(1.0/3.0);
constexpr Float Quarter(1.0/4.0);
constexpr Float One(1.0);
constexpr Float Two(2.0);
constexpr Float Three(3.0);
constexpr Float Four(4.0);
constexpr Float Five(5.0);
constexpr Float Pi(3.14159265358979323846264338327950288419716939937510);

/** Get absolute value. */
#define Abs(_a) std::abs((_a))

/** Get minimum value. */
#define Min(_a, _b) std::min((_a), (_b))

/** Get minimum value. */
#define Max(_a, _b) std::max((_a), (_b))

/** Bound value between minimum and maximum. */
#define MinMax(_a, _min, _max) std::max(std::min(_a, _max), _min)

/***************************************************************************************************************************************************************
* Loops
***************************************************************************************************************************************************************/

/** For loop through dimensions. */
#define FOR1(_ic) for(int (_ic) = 0; (_ic) < Apeiron::nDIM; ++(_ic))

/** For loop from the first index. */
#define FOR2(_index, _n_index) for(int (_index) = 0; (_index) < (_n_index); ++(_index))

/** For loop from a starting index. */
#define FOR3(_index, _start_index, _n_index) for(int (_index) = (_start_index); (_index) < (_n_index); ++(_index))

/** Generic for loop. */
#define FOR(...) MACRO(FOR, __VA_ARGS__)

/** For loop in reverse from a starting index to the first index. */
#define FOR_REVERSE2(_index, _start_index) for(int _index = _start_index; _index >= 0; --_index)

/** For loop in reverse from a starting index. */
#define FOR_REVERSE3(_index, _start_index, _end_index) for(int _index = _start_index; _index >= _end_index; --_index)

/** Generic for loop in reverse. */
#define FOR_REVERSE(...) MACRO(FOR_REVERSE, __VA_ARGS__)

/** For loop over each item in a list. */
#define FOR_EACH(_item, _list) for(const auto& _item : _list)

/** For loop over each item in a list (modifiable version). */
#define FOR_EACH_REF(_item, _list) for(auto& _item : _list)

/***************************************************************************************************************************************************************
* Printing to Screen
***************************************************************************************************************************************************************/

/** Print a string. */
#define PRINT(_string) std::cout << _string <<"\n";

/** Get a string with file name and line number. */
#define FILE_AND_LINE() std::string("\n File: ") + std::string(__FILE__) + std::string("\n Line: ") + std::string(std::to_string(__LINE__))

/** Get a string with file name and line number. */
#define PING() {usleep(100); PRINT("\n PING from file:" + std::string(__FILE__) + " at line number: " + std::string(__LINE__) + "\n"); std::cout<<std::flush;};

/** Print the current stack trace. */
inline void PRINT_STACK(const int _stack_size)
{
  void *array[_stack_size];
  const int stack_size(backtrace(array, _stack_size));
  char **strings(backtrace_symbols(array, stack_size));

  if(strings != nullptr)
  {
    printf("Obtained %d stack frames.\n", stack_size);
    FOR(ii, stack_size) printf("%s\n", strings[ii]);
  }

  free(strings);
}

/***************************************************************************************************************************************************************
* Error Handling
***************************************************************************************************************************************************************/

#define ERROR(error_Message) {std::cerr << "\n ERROR: " << error_Message << FILE_AND_LINE() << "\n"; exit(-1);};

#define ASSERT(_condition, error_Message) if(!(_condition)) ERROR(error_Message);

#define STATIC_ASSERT(_condition, error_Message) static_assert(_condition, error_Message);

#ifdef DEBUG_MODE
#define DEBUG_ASSERT(_condition, error_Message) ASSERT(_condition, error_Message);
#else
#define DEBUG_ASSERT(_condition, error_Message) {};
#endif

/***************************************************************************************************************************************************************
* Number error checking
***************************************************************************************************************************************************************/

template <typename data_type>
constexpr bool isNumber(const data_type& _value)
{
  return (typeid(data_type) == typeid(int) || typeid(data_type) == typeid(Float) || typeid(data_type) == typeid(short int) ||
          typeid(data_type) == typeid(unsigned short int) || typeid(data_type) == typeid(long int) || typeid(data_type) == typeid(unsigned long int) ||
          typeid(data_type) == typeid(long long int) || typeid(data_type) == typeid(unsigned long long int));
}

#define ASSERT_NUMBER(_value) ASSERT(isNumber(_value), "The passed value is not a number.");

#define DEBUG_ASSERT_NUMBER(_value) DEBUG_ASSERT(isNumber(_value), "The passed value is not a number.");

#define ASSERT_VALID_NUMBER(_value) if(std::isnan(_value) || std::isinf(_value)) ERROR(std::fpclassify(ax) == 0 ? "NaN detected\n\t\t" : "Inf detected\n\t\t");

/***************************************************************************************************************************************************************
* Floating-point Comparison Functions
***************************************************************************************************************************************************************/

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

}//Apeiron

#endif //GLOBAL_H
