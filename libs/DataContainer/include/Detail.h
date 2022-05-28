#pragma once

#include "../../../include/Global.h"

namespace aprn::detail {

/***************************************************************************************************************************************************************
* Static Array Support Functions
***************************************************************************************************************************************************************/

/** Initialise an std::array with a single value. */
template<typename T, size_t size>
constexpr auto InitStaticArray(const T& _value)
{
  std::array<T, size> initialised_array;
  std::fill(initialised_array.begin(), initialised_array.end(), _value);
  return initialised_array;
}

/** Initialise an std::array with an initializer list. */
template<typename T, size_t size>
constexpr auto InitStaticArray(const std::initializer_list<T>& _list)
{
  std::array<T, size> initialised_array;
  std::copy(_list.begin(), _list.end(), initialised_array.begin());
  return initialised_array;
}

/** Initialise an std::array with a first and last iterator. */
template<typename T, size_t size, class iter>
constexpr auto InitStaticArray(const iter _first, const iter _last)
{
  std::array<T, size> initialised_array;
  std::copy(_first, _last, initialised_array.begin());
  return initialised_array;
}

}//aprn::detail