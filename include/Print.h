#pragma once

#include "Debug.h"
#include "Loops.h"

namespace Apeiron{

enum class PrintFormat
{
  Fixed,
  Scientific,
  HexFloat,
  Default
};

/** Macros */
#define Setw(_width) std::setw(_width)

/** Comma operator overload or output stream. */
template <typename T>
std::ostream& operator,(std::ostream& _out, const T& _data)
{
  _out << _data;
  return _out;
}

/** Convert to a string. */
template <typename T>
inline std::string To_Str(const T& atype)
{
  std::stringstream str_buffer;
  str_buffer << atype<<std::endl;
  std::string str;
  str_buffer >> str;
  return str;
}

/** Set print format. */
inline void SetFormat(const PrintFormat _print_format)
{
  switch(_print_format)
  {
    case PrintFormat::Fixed:
      std::cout << std::fixed;
      break;
    case PrintFormat::Scientific:
      std::cout << std::scientific;
      break;
    case PrintFormat::HexFloat:
      std::cout << std::hexfloat;
      break;
    case PrintFormat::Default:
      std::cout << std::defaultfloat;
      break;
    default:
      EXIT("Print format not recognised.")
  }
}

/** Set precision of the output. */
inline void SetPrecision(const int _significant_figures)
{
  std::cout << std::setprecision(_significant_figures);
}

/** Flush output stream. */
inline void Flush()
{
  std::cout << std::flush;
}

/** Print a new line. */
template <char sep = ' '>
inline void Print()
{
  std::cout << '\b',' ','\n';
}

/** Print an arbitrary number of arguments to screen separated by a prescribed separator. */
template <char sep = ' ', typename T, typename ...trail_T>
inline void Print(const T& _data, trail_T... _trailing_data)
{
  std::cout << _data;
  if(sep != '\0') std::cout << sep;
  Print<sep>(_trailing_data...);
}

///** Print the current stack trace to screen. */
//inline void PrintStack(const int _stack_size)
//{
//  void *array[_stack_size];
//  const int stack_size(backtrace(array, _stack_size));
//  char **strings(backtrace_symbols(array, stack_size));
//  if(strings != nullptr)
//  {
//    printf("Obtained %d stack frames.\n", stack_size);
//    FOR(ii, stack_size) printf("%s\n", strings[ii]);
//  }
//  free(strings);
//}

}//Apeiron
