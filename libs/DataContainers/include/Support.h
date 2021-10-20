#pragma once

#include "../../../include/Global.h"

namespace Apeiron{
namespace DataContainersSupport{

/***************************************************************************************************************************************************************
* Static Array Support Functions
***************************************************************************************************************************************************************/

/** Initialise an std::array with a single value. */
template <class t_data_type, std::size_t t_array_size>
constexpr auto InitStaticArray(const t_data_type& _init_value)
{
  std::array<t_data_type, t_array_size> initialised_array;
  std::fill(initialised_array.begin(), initialised_array.end(), _init_value);
  return initialised_array;
}

/** Initialise an std::array with an initializer list. */
template <class t_data_type, std::size_t t_array_size>
constexpr auto InitStaticArray(const std::initializer_list<t_data_type>& _initialiser_list)
{
  areSizesEqual(t_array_size, _initialiser_list.size()) ? true : throw "The size of the initializer list must equal the array size.";
  std::array<t_data_type, t_array_size> initialised_array;
  std::copy(_initialiser_list.begin(), _initialiser_list.end(), initialised_array.begin());
  return initialised_array;
}

/** Initialise an std::array with a first and last iterator. */
template <class t_data_type, std::size_t t_array_size, class t_iterator>
constexpr auto InitStaticArray(const t_iterator _first, const t_iterator _last)
{
  areTypesEqual<t_data_type, typename std::iterator_traits<t_iterator>::value_type>() ? true : throw "The number of iterators must equal the array size.";
  areSizesEqual(t_array_size, std::distance(_first, _last)) ? true : throw "The number of iterators must equal the array size.";
  std::array<t_data_type, t_array_size> initialised_array;
  std::copy(_first, _last, initialised_array.begin());
  return initialised_array;
}

}
}