//
// Created by niran90 on 2021/09/26.
//

#ifndef DEBUG_H
#define DEBUG_H

/***************************************************************************************************************************************************************
* General Error Handling
***************************************************************************************************************************************************************/

/** Get a string with file name and line number. */
#define FILE_LINE "\n FILE: " + To_Str(__FILE__) + "\n LINE: " + To_Str(__LINE__) + "\n"

/** Print a Ping from the current file and line. */
#define PING {usleep(100); Print("\nPING from file:", __FILE__, "at line number:", __LINE__); Flush();};

/** Assert a static condition. If false, throw an error and exit. */
#define STATIC_ASSERT(_static_condition, error_Message) static_assert(_static_condition, error_Message);

/** Throw error and exit. */
#define ERROR(_args...)\
{\
  std::cerr<<"\n******************************************************************************",\
             "\n ERROR: ", _args,                                                               \
             FILE_LINE,                                                                         \
             "******************************************************************************\n";\
  exit(-1);\
}

/** Assert a condition. If false, throw an error and exit. */
#define ASSERT(_condition, _args...) if(!(_condition)) ERROR(_args);

/** Assert a condition in debug mode. If false, throw an error and exit. */
#ifdef DEBUG_MODE
#define DEBUG_ASSERT(_condition, _args...) ASSERT(_condition, _args);
#else
#define DEBUG_ASSERT(_condition, _args...) {};
#endif

/***************************************************************************************************************************************************************
* Numerical Error Handling
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

#endif //DEBUG_H
