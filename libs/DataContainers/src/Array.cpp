#pragma once

#include "../../../include/Global.h"
#include "../include/Array.h"
#include "../include/Support.h"

namespace Apeiron{

/***************************************************************************************************************************************************************
* Array Abstract Base Class
***************************************************************************************************************************************************************/

/** Size and Index Range-checking *****************************************************************************************************************************/
template <class t_derived_class, class t_data_type>
constexpr void
Array<t_derived_class, t_data_type>::IndexBoundCheck(const std::size_t _index) const
{
  DEBUG_ASSERT(Derived().size(), "The array has not yet been sized.")
  DEBUG_ASSERT(isBounded(_index, std::size_t(0), Derived().size()), "The array index ", _index, " must be in the range [0, ", Derived().size() - 1, "].")
}

template <class t_derived_class, class t_data_type>
constexpr void
Array<t_derived_class, t_data_type>::SizeCheck(const std::size_t _size0, const std::size_t _size1) const
{
  DEBUG_ASSERT(areSizesEqual(_size0, _size1), "The array sizes ", _size0, " and ", _size1, " must be equal.")
}

/** Subscript Operator Overloads ******************************************************************************************************************************/
template <class t_derived_class, class t_data_type>
constexpr t_data_type&
Array<t_derived_class, t_data_type>::operator[](const std::size_t _index)
{
  IndexBoundCheck(_index);
  return *(Derived().begin() + _index);
}

template <class t_derived_class, class t_data_type>
constexpr const t_data_type&
Array<t_derived_class, t_data_type>::operator[](const std::size_t _index) const
{
  IndexBoundCheck(_index);
  return *(Derived().begin() + _index);
}

/** Assignment Operator Overloads *****************************************************************************************************************************/
template <class t_derived_class, class t_data_type>
template <class t_rhs_type>
constexpr t_derived_class&
Array<t_derived_class, t_data_type>::operator=(const t_rhs_type& _value) noexcept
{
  STATIC_ASSERT(GetTypeCategory<t_data_type>() == GetTypeCategory<t_rhs_type>(), "Attempting to assign to an incompatible type.")
  FOR_EACH(entry, Derived()) entry = _value;
  return Derived();
}

template <class t_derived_class, class t_data_type>
constexpr t_derived_class&
Array<t_derived_class, t_data_type>::operator=(const std::initializer_list<t_data_type>& _value_list) noexcept
{
  SizeCheck(_value_list.size(), Derived().size());
  std::size_t index(0);
  FOR_EACH(entry, _value_list) Derived()[index++] = entry;
  return Derived();
}

/** Derived Class Access **************************************************************************************************************************************/
template <class t_derived_class, class t_data_type>
constexpr t_derived_class&
Array<t_derived_class, t_data_type>::Derived() noexcept
{
  return static_cast<t_derived_class&>(*this);
}

template <class t_derived_class, class t_data_type>
constexpr const t_derived_class&
Array<t_derived_class, t_data_type>::Derived() const noexcept
{
  return static_cast<const t_derived_class&>(*this);
}

/** Operator Overloads ****************************************************************************************************************************************/
template <class t_derived_class, class t_data_type>
std::ostream& operator<<(std::ostream& _output_stream, const Array<t_derived_class, t_data_type>& _array_base)
{
  const t_derived_class& derived = static_cast<t_derived_class&>(_array_base);
  FOR(i, derived.size()) _output_stream<<derived[i]<<(i == derived.size() - 1 ? "" : ", ");
  return _output_stream;
}

/***************************************************************************************************************************************************************
* Static Array Class
***************************************************************************************************************************************************************/
template <class t_data_type, std::size_t t_array_size>
constexpr StaticArray<t_data_type, t_array_size>::StaticArray()
  : StaticArray(GetStaticInitValue<t_data_type>()) {}

template <class t_data_type, std::size_t t_array_size>
constexpr StaticArray<t_data_type, t_array_size>::StaticArray(const t_data_type& _init_value)
  : std::array<t_data_type, t_array_size>(DataContainersSupport::InitStaticArray<t_data_type, t_array_size>(_init_value)) {}

template <class t_data_type, std::size_t t_array_size>
constexpr StaticArray<t_data_type, t_array_size>::StaticArray(const std::initializer_list<t_data_type>& _initialiser_list)
  : std::array<t_data_type, t_array_size>(DataContainersSupport::InitStaticArray<t_data_type, t_array_size>(_initialiser_list)) {}

template <class t_data_type, std::size_t t_array_size>
template <class t_iterator>
constexpr StaticArray<t_data_type, t_array_size>::StaticArray(const t_iterator _first, const t_iterator _last)
  : std::array<t_data_type, t_array_size>(DataContainersSupport::InitStaticArray<t_data_type, t_array_size>(_first, _last)) {}

/***************************************************************************************************************************************************************
* Dynamic Array Class
***************************************************************************************************************************************************************/
template <class t_data_type>
DynamicArray<t_data_type>::DynamicArray()
  : std::vector<t_data_type>() {}

template <class t_data_type>
DynamicArray<t_data_type>::DynamicArray(const std::size_t _array_size)
  : DynamicArray(_array_size, GetDynamicInitValue<t_data_type>()) {}

template <class t_data_type>
DynamicArray<t_data_type>::DynamicArray(const std::size_t _t_array_size, const t_data_type& _init_value)
  : std::vector<t_data_type>(_t_array_size, _init_value) {}

template <class t_data_type>
DynamicArray<t_data_type>::DynamicArray(const std::initializer_list<t_data_type>& _initialiser_list)
  : std::vector<t_data_type>(_initialiser_list) {}

template <class t_data_type>
template <class t_iterator>
DynamicArray<t_data_type>::DynamicArray(const t_iterator _first, const t_iterator _last)
  : std::vector<t_data_type>(_first, _last) {}

}

