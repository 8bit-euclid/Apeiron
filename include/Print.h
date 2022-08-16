#pragma once

#include "Debug.h"
#include "Loops.h"

#include <iomanip>

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
inline void SetPrecision(const int n) { std::cout << std::setprecision(n); }

/** Flush output stream. */
inline void Flush() { std::cout << std::flush; }

/** Suppress screen write-outs. */
inline void StopCout() { std::cout.setstate(std::ios_base::failbit); }

/** Reset screen write-outs. */
inline void ResetCout() { std::cout.clear(); }

/** Print an arbitrary number of arguments to screen separated by a prescribed separator. */
template<char sep = ' ', typename T, typename... Ts>
inline void Print(const T& data, const Ts&... trailing_data)
{
   // Write first data element to the appropriate output stream.
   std::cout << data;

   // Write trailing data and/or handle base case.
   if constexpr(sizeof...(trailing_data))
   {
      if constexpr(sep != '\0') std::cout << sep; // Add separator.
      Print<sep>(trailing_data...);
   }
   else std::cout << "\n";
}

/** Print an arbitrary number of arguments to screen separated by a prescribed separator. */
template<char sep = ' ', typename T, typename... Ts>
inline void WPrint(const T& data, const Ts&... trailing_data)
{
   // Write first data element to the appropriate output stream.
   std::wcout << data;

   // Write trailing data and/or handle base case.
   if constexpr(sizeof...(trailing_data))
   {
      if constexpr(sep != '\0') std::wcout << sep; // Add separator.
      WPrint<sep>(trailing_data...);
   }
   else std::wcout << "\n";
}

}//aprn
