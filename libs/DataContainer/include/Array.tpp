/***************************************************************************************************************************************************************
* GPL-3.0 License
* Copyright (C) 2022 Niran A. Ilangakoon
*
* This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as
* published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
* of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program.
* If not, see <https://www.gnu.org/licenses/>.
***************************************************************************************************************************************************************/

#pragma once

#include "Detail.h"

namespace aprn {

/***************************************************************************************************************************************************************
* Array Abstract Base Class
***************************************************************************************************************************************************************/

/** Size and Index Range-checking */
template<typename T, class derived>
constexpr void
Array<T, derived>::IndexBoundCheck(const size_t index) const
{
  DEBUG_ASSERT(!Derived().empty(), "The array has not yet been sized.")
  DEBUG_ASSERT(isBounded(index, size_t(0), Derived().size()), "The array index ", index, " must be in the range [0, ", Derived().size() - 1, "].")
}

template<typename T, class derived>
constexpr void
Array<T, derived>::SizeCheck(const size_t _size0, const size_t _size1) const
{
  DEBUG_ASSERT(areSizesEqual(_size0, _size1), "The array sizes ", _size0, " and ", _size1, " must be equal.")
}

/** Subscript Operator Overloads */
template<typename T, class derived>
constexpr T&
Array<T, derived>::operator[](const size_t index)
{
  IndexBoundCheck(index);
  return *(Derived().begin() + index);
}

template<typename T, class derived>
constexpr const T&
Array<T, derived>::operator[](const size_t index) const
{
  IndexBoundCheck(index);
  return *(Derived().begin() + index);
}

/** Assignment Operator Overloads */
template<typename T, class derived>
constexpr derived&
Array<T, derived>::operator=(const std::convertible_to<T> auto value) noexcept
{
  FOR_EACH(entry, Derived()) entry = static_cast<T>(value);
  return Derived();
}

template<typename T, class derived>
template<std::convertible_to<T> T2>
constexpr derived&
Array<T, derived>::operator=(const std::initializer_list<T2>& value_list) noexcept
{
   if constexpr(isTypeSame<derived, DynamicArray<T>>()) Derived().resize(value_list.size());
   size_t index(0);
   FOR_EACH(entry, value_list) Derived()[index++] = entry;
   return Derived();
}

/** Comparison Operator Overloads */
template<typename T, class derived>
constexpr bool Array<T, derived>::operator==(const Array<T, derived>& _other) noexcept
{
  if(_other.Derived().size() != Derived().size()) return false;
  FOR(i, _other.Derived().size()) if(Derived()[i] != _other[i]) return false;
  return true;
}

template<typename T, class derived>
constexpr bool Array<T, derived>::operator!=(const Array<T, derived>& _other) noexcept { return !(Derived() == _other); }

/** Non-member functions */
template<typename T, class derived>
std::ostream&
operator<<(std::ostream& _output_stream, const Array<T, derived>& _array_base)
{
  const derived& derived_class = static_cast<const derived&>(_array_base);

  _output_stream << "(";
  FOR(i, derived_class.size()) _output_stream << derived_class[i] << (i == derived_class.size() - 1 ? "" : ", ");
  _output_stream << ")";

  return _output_stream;
}

/***************************************************************************************************************************************************************
* Static Array Class
***************************************************************************************************************************************************************/
template<typename T, size_t size>
constexpr StaticArray<T, size>::StaticArray()
  : StaticArray(GetStaticInitValue<T>()) {}

template<typename T, size_t size>
constexpr StaticArray<T, size>::StaticArray(const std::convertible_to<T> auto& value)
  : std::array<T, size>(detail::InitStaticArray<T, size>(value)) {}

template<typename T, size_t size>
template<std::convertible_to<T> T2>
constexpr StaticArray<T, size>::StaticArray(const std::initializer_list<T2>& list)
  : std::array<T, size>(detail::InitStaticArray<T, size>(list))
{
  ASSERT(size == list.size(), "The initializer list should be of size ", size, ".")
}

template<typename T, size_t size>
template<class It>
constexpr StaticArray<T, size>::StaticArray(It first, It last)
  : std::array<T, size>(detail::InitStaticArray<T, size>(first, last))
{
  STATIC_ASSERT((isTypeSame<T, typename std::iterator_traits<It>::value_type>()), "Mismatch in the iterator data type.")
  ASSERT(size == std::distance(first, last), "The number of iterators must equal the array size ", size, ".")
}

/***************************************************************************************************************************************************************
* Dynamic Array Class
***************************************************************************************************************************************************************/
template<typename T>
DynamicArray<T>::DynamicArray() : std::vector<T>() {}

template<typename T>
DynamicArray<T>::DynamicArray(const size_t size) : DynamicArray(size, GetDynamicInitValue<T>()) {}

template<typename T>
DynamicArray<T>::DynamicArray(const size_t size, const std::convertible_to<T> auto& value) : std::vector<T>(size, value) {}

template<typename T>
template<std::convertible_to<T> T2>
DynamicArray<T>::DynamicArray(const std::initializer_list<T2>& list) : std::vector<T>(list) {}

template<typename T>
template<class It>
DynamicArray<T>::DynamicArray(It first, It last) : std::vector<T>(first, last) {}

template<typename T>
void
DynamicArray<T>::Append(const T& value) { this->push_back(value); }

template<typename T>
void
DynamicArray<T>::Append(T&& value) noexcept { this->emplace_back(std::move(value)); }

template<typename T>
void
DynamicArray<T>::Append(const DynamicArray<T>& other) { Append(other.cbegin(), other.cend(), false); }

template<typename T>
void
DynamicArray<T>::Append(DynamicArray<T>&& other) noexcept { Append(other.begin(), other.end(), true); }

template<typename T>
template<class It>
void
DynamicArray<T>::Append(It first, It last, const bool move_all)
{
   this->reserve(this->size() + std::distance(first, last));
   if(!move_all) this->insert(this->end(), first, last);
   else this->insert(this->end(), std::make_move_iterator(first), std::make_move_iterator(last));
}

template<typename T>
void
DynamicArray<T>::Erase()
{
   this->clear();
   this->shrink_to_fit();
}

}

