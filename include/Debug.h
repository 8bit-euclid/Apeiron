#pragma once

namespace Apeiron{

/***************************************************************************************************************************************************************
* General Error Handling
***************************************************************************************************************************************************************/

/** Get a string with file name and line number. */
#define FILE_LINE(_file, _line) "\n FILE: " + To_Str(_file) + "\n LINE: " + To_Str(_line) + "\n"

/** Print a Ping from the current file and line. */
#define PING usleep(100); Print("\nPing from file:", __FILE__, "at line number:", __LINE__); Flush();

/** Throw error without exiting from a given file and line. */
#define ERROR_FROM(_file, _line, _args...)\
{\
  std::cerr<<"\n*************************************************************************************",\
             "\n ERROR: ", _args,                                                               \
             FILE_LINE(_file, _line),                                                                         \
             "*************************************************************************************\n";\
}

/** Throw error without exiting from the current line and file. */
#define ERROR(_args...) ERROR_FROM(__FILE__, __LINE__, _args)

/** Throw error and exit. */
#define EXIT(_args...) { ERROR(_args); exit(-1); }

/** Throw error from a given file and line, and exit. */
#define EXIT_FROM(_file, _line, _args...) { ERROR_FROM(_file, _line, _args); exit(-1); }

/** Assert a static condition. If false, throw an error and exit. */
#define STATIC_ASSERT(_static_condition, error_Message) static_assert((_static_condition), error_Message);

/** Assert a condition. If false, throw an error and exit. */
#define ASSERT(_condition, _args...) if(!(_condition)) EXIT(_args);

/** Assert a condition in debug mode. If false, throw an error and exit. */
#ifdef DEBUG_MODE
  #define DEBUG_ASSERT(_condition, _args...) ASSERT(_condition, _args);
#else
  #define DEBUG_ASSERT(_condition, _args...) {};
#endif

/***************************************************************************************************************************************************************
* Numerical Error Handling
***************************************************************************************************************************************************************/

#define ASSERT_NUMBER(_value) ASSERT(isNumber(_value), "The passed value is not a number.");

#define DEBUG_ASSERT_NUMBER(_value) DEBUG_ASSERT(isNumber(_value), "The passed value is not a number.");

#define ASSERT_VALID_NUMBER(_value) if(std::isnan(_value) || std::isinf(_value)) EXIT(std::fpclassify(ax) == 0 ? "NaN detected\n\t\t" : "Inf detected\n\t\t");

}