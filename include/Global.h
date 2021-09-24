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

#define EXPAND(x) x
#define PP_ARG_N(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...) N
#define ARGUMENT_COUNT(...) EXPAND(PP_ARG_N(__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0))

/** General definition for any function name */
#define _VFUNC_(name, n) name##n
#define _VFUNC(name, n) _VFUNC_(name, n)
#define VFUNC(func, ...) _VFUNC(func, ARGUMENT_COUNT(__VA_ARGS__))(__VA_ARGS__)

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

constexpr Float Small(10.0*Epsilon);
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
#define FOR(...) VFUNC(FOR, __VA_ARGS__)

/** For loop in reverse from a starting index to the first index. */
#define FOR_REVERSE2(_index, _start_index) for(int _index = _start_index; _index >= 0; --_index)

/** For loop in reverse from a starting index. */
#define FOR_REVERSE3(_index, _start_index, _end_index) for(int _index = _start_index; _index >= _end_index; --_index)

/** Generic for loop in reverse. */
#define FOR_REVERSE(...) VFUNC(FOR_REVERSE, __VA_ARGS__)

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

#define ASSERT(_condition, error_Message) if(!(_condition)) ERROR(error_Message)

#define STATIC_ASSERT(_condition, error_Message) static_assert(_condition, error_Message);

#ifdef DEBUG_MODE
#define DEBUG_ASSERT(_condition, error_Message) ASSERT(_condition, error_Message)
#else
#define DEBUG_ASSERT(_condition, error_Message) {};
#endif

#define ASSE_NOTNANINF(ax, aerrorStri) \
  if(std::isnan(ax) || std::isinf(ax))  ERROR_EXIT((std::fpclassify(ax) == 0 ? "NAN detected\n\t\t" : "INF detected\n\t\t") + ToStri(aerrorStri));

}//Apeiron

#endif //GLOBAL_H
