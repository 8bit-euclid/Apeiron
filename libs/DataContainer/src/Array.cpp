#pragma once

#include "../include/Array.h"
#include "../include/Detail.h"

namespace Apeiron{

/***************************************************************************************************************************************************************
* Array Abstract Base Class
***************************************************************************************************************************************************************/

/** Size and Index Range-checking */
template<typename T, class derived>
constexpr void
Array<T, derived>::IndexBoundCheck(const std::size_t _index) const
{
  DEBUG_ASSERT(!Derived().empty(), "The array has not yet been sized.")
  DEBUG_ASSERT(isBounded(_index, std::size_t(0), Derived().size()), "The array index ", _index, " must be in the range [0, ", Derived().size() - 1, "].")
}

template<typename T, class derived>
constexpr void
Array<T, derived>::SizeCheck(const std::size_t _size0, const std::size_t _size1) const
{
  DEBUG_ASSERT(areSizesEqual(_size0, _size1), "The array sizes ", _size0, " and ", _size1, " must be equal.")
}

/** Subscript Operator Overloads */
template<typename T, class derived>
constexpr T&
Array<T, derived>::operator[](const std::size_t _index)
{
  IndexBoundCheck(_index);
  return *(Derived().begin() + _index);
}

template<typename T, class derived>
constexpr const T&
Array<T, derived>::operator[](const std::size_t _index) const
{
  IndexBoundCheck(_index);
  return *(Derived().begin() + _index);
}

/** Assignment Operator Overloads */
template<typename T, class derived>
constexpr derived&
Array<T, derived>::operator=(const std::convertible_to<T> auto _value) noexcept
{
  FOR_EACH(entry, Derived()) entry = static_cast<T>(_value);
  return Derived();
}

template<typename T, class derived>
constexpr derived&
Array<T, derived>::operator=(const std::initializer_list<T>& _value_list) noexcept
{
  SizeCheck(_value_list.size(), Derived().size());
  std::size_t index(0);
  FOR_EACH(entry, _value_list) Derived()[index++] = entry;
  return Derived();
}

/** Non-member functions */
template<typename T, class derived>
std::ostream&
operator<<(std::ostream& _output_stream, const Array<T, derived>& _array_base)
{
  const derived& derived_class = static_cast<const derived&>(_array_base);
  FOR(i, derived_class.size()) _output_stream << derived_class[i] << (i == derived_class.size() - 1 ? "" : ", ");
  return _output_stream;
}

/***************************************************************************************************************************************************************
* Static Array Class
***************************************************************************************************************************************************************/
template<typename T, std::size_t t_array_size>
constexpr StaticArray<T, t_array_size>::StaticArray()
  : StaticArray(GetStaticInitValue<T>()) {}

template<typename T, std::size_t t_array_size>
constexpr StaticArray<T, t_array_size>::StaticArray(const T& _value)
  : std::array<T, t_array_size>(Detail::InitStaticArray<T, t_array_size>(_value)) {}

template<typename T, std::size_t t_array_size>
constexpr StaticArray<T, t_array_size>::StaticArray(const std::initializer_list<T>& _list)
  : std::array<T, t_array_size>(Detail::InitStaticArray<T, t_array_size>(_list)) {}

template<typename T, std::size_t t_array_size>
template<class iter>
constexpr StaticArray<T, t_array_size>::StaticArray(const iter _first, const iter _last)
  : std::array<T, t_array_size>(Detail::InitStaticArray<T, t_array_size>(_first, _last)) {}

/***************************************************************************************************************************************************************
* Dynamic Array Class
***************************************************************************************************************************************************************/
template<typename T>
DynamicArray<T>::DynamicArray()
  : std::vector<T>() {}

template<typename T>
DynamicArray<T>::DynamicArray(const std::size_t _size)
  : DynamicArray(_size, GetDynamicInitValue<T>()) {}

template<typename T>
DynamicArray<T>::DynamicArray(const std::size_t _size, const T& _value)
  : std::vector<T>(_size, _value) {}

template<typename T>
DynamicArray<T>::DynamicArray(const std::initializer_list<T>& _list)
  : std::vector<T>(_list) {}

template<typename T>
template<class iter>
DynamicArray<T>::DynamicArray(const iter _first, const iter _last)
  : std::vector<T>(_first, _last) {}

}

