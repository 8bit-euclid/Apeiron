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

//template <class t_data_type, int t_array_size>
//constexpr auto InitStaticArray(typename std::array<t_data_type, t_array_size>::iterator _first, typename std::array<t_data_type, t_array_size>::iterator _last)
//{
//  std::array<t_data_type, t_array_size> initialised_array;
//  std::fill(initialised_array.begin(), initialised_array.end(), _init_value);
//  return initialised_array;
//}

/** Initialise an std::array with an initializer list. */
template <class t_data_type, std::size_t t_array_size>
constexpr auto InitStaticArray(const std::initializer_list<t_data_type>& _initialiser_list)
{
  areSizesEqual(t_array_size, _initialiser_list.size()) ? true : throw "The size of the initializer list must equal the array size.";
  std::array<t_data_type, t_array_size> initialised_array;
  std::copy(_initialiser_list.begin(), _initialiser_list.end(), initialised_array.begin());
  return initialised_array;
}

//template <typename t_data_type, std::size_t t_array_size, typename... Ts>
//constexpr std::enable_if_t<(sizeof...(Ts) == t_array_size), std::array<t_data_type, t_array_size> >
//GetArrayFromInitializerList(const t_data_type* const beg, const t_data_type* const end, const Ts ...xs)
//{
//  return std::array<t_data_type, t_array_size>{xs...};
//}
//
//template <typename t_data_type, size_t t_array_size, typename... Ts>
//constexpr std::enable_if_t<(sizeof...(Ts) < t_array_size), std::array<t_data_type, t_array_size> >
//GetArrayFromInitializerList(const t_data_type *const beg, const t_data_type *const end, const Ts... xs)
//{
//  return GetArrayFromInitializerList<t_data_type, t_array_size>(beg + 1, end, *beg, xs...);
//}
//
//template <typename t_data_type, std::size_t t_array_size>
//constexpr auto InitStaticArray(std::initializer_list<t_data_type> _initialiser_list)
//{
//  return GetArrayFromInitializerList<t_data_type, t_array_size>(_initialiser_list.begin(), _initialiser_list.end());
//}

}
}