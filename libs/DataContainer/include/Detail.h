#pragma once

#include "../../../include/Global.h"

namespace aprn::detail {

/***************************************************************************************************************************************************************
* Static Array Support Functions
***************************************************************************************************************************************************************/

/** Initialise an std::array with a single value. */
template<typename T, size_t size>
constexpr auto InitStaticArray(const T& value)
{
  std::array<T, size> initialised_array;
  std::fill(initialised_array.begin(), initialised_array.end(), value);
  return initialised_array;
}

/** Initialise an std::array with an initializer list. */
template<typename T, size_t size>
constexpr auto InitStaticArray(const std::initializer_list<T>& list)
{
  std::array<T, size> initialised_array;
  std::copy(list.begin(), list.end(), initialised_array.begin());
  return initialised_array;
}

/** Initialise an std::array with a first and last iterator. */
template<typename T, size_t size, class iter>
constexpr auto InitStaticArray(const iter first, const iter last)
{
  std::array<T, size> initialised_array;
  std::copy(first, last, initialised_array.begin());
  return initialised_array;
}

}//aprn::detail