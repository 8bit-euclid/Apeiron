#pragma once

#include "Debug.h"
#include "Loops.h"

namespace aprn {

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
template<typename T>
std::ostream& operator,(std::ostream& out, const T& data) { return out << data; }

/** Set print format. */
inline void SetFormat(const PrintFormat format)
{
  switch(format)
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
inline void SetPrecision(const int _significant_figures) { std::cout << std::setprecision(_significant_figures); }

/** Flush output stream. */
inline void Flush() { std::cout << std::flush; }

/** Print a new line. */
template<char sep = ' '>
inline void Print() { std::cout << '\b',' ','\n'; }

/** Print an arbitrary number of arguments to screen separated by a prescribed separator. */
template<char sep = ' ', typename T, typename... Ts>
inline void Print(const T& data, Ts... trailing_data)
{
  std::cout << data;
  if(sep != '\0') std::cout << sep;
  Print<sep>(trailing_data...);
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

}//aprn
