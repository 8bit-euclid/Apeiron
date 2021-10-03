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
template <typename data_type>
std::ostream& operator,(std::ostream& _out, const data_type& _data)
{
  _out<<_data;
  return _out;
}

/** Convert to a string. */
template <typename data_type>
inline std::string To_Str(const data_type& atype)
{
  std::stringstream str_buffer;
  str_buffer<<atype<<std::endl;
  std::string str;
  str_buffer>>str;
  return str;
}

/** Set print format. */
inline void SetFormat(const PrintFormat _print_format)
{
  switch(_print_format)
  {
    case PrintFormat::Fixed:
      std::cout<<std::fixed;
      break;
    case PrintFormat::Scientific:
      std::cout<<std::scientific;
      break;
    case PrintFormat::HexFloat:
      std::cout<<std::hexfloat;
      break;
    case PrintFormat::Default:
      std::cout<<std::defaultfloat;
      break;
    default:
      ERROR("Print format not recognised.")
  }
}

/** Set precision. */
inline void SetPrecision(const int _significant_figures)
{
  std::cout<<std::setprecision(_significant_figures);
}

/** Flush output stream. */
inline void Flush()
{
  std::cout<<std::flush;
}

/** Print a new line. */
template <char separator>
inline void Print()
{
  std::cout<<'\b',' ','\n';
}

/** Print an arbitrary number of arguments to screen. */
template <char separator = ' ', typename data_type, typename ...tail_data_type>
inline void Print(const data_type& _data, tail_data_type... _tail_data)
{
  std::cout<<_data<<separator;
  Print<separator>(_tail_data...);
}

/** Print the current stack trace to screen. */
inline void PrintStack(const int _stack_size)
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

}//Apeiron
