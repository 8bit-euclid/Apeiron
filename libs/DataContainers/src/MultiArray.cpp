#pragma once

#include "../../../include/Global.h"
#include "../include/MultiArray.h"
#include "../include/Support.h"

namespace Apeiron {

/***************************************************************************************************************************************************************
* Multi-dimensional Array Abstract Base Class
***************************************************************************************************************************************************************/

/** Size and Index Range-checking *****************************************************************************************************************************/
template <class t_derived_class, class t_data_type>
constexpr void
MultiArray<t_derived_class, t_data_type>::MultiIndexBoundCheck(const std::convertible_to<std::size_t> auto ..._multi_index) const
{
#ifdef DEBUG_MODE
  ASSERT(areSizesEqual(sizeof...(_multi_index), this->Derived().Dimensions.size()), "Multi-index size mismatch.")

  constexpr auto& dimensions = this->Derived().Dimensions;
  const std::size_t multi_index[] = {static_cast<std::size_t>(_multi_index)...};
  FOR(i, dimensions.size()) ASSERT(multi_index[i] < dimensions[i], "Multi index component ", multi_index[i]," must be lesser than ", dimensions[i],".")
#endif
}

/** Multi-dimensional Subscript Index Toggling ****************************************************************************************************************/
template <class t_derived_class, class t_data_type>
constexpr std::size_t
MultiArray<t_derived_class, t_data_type>::ComputeLinearIndex(const std::convertible_to<std::size_t> auto ..._multi_index) const
{
  MultiIndexBoundCheck(_multi_index...);

  constexpr auto& dimensions = this->Derived().Dimensions;
  const std::size_t multi_index[] = {static_cast<std::size_t>(_multi_index)...};

  std::size_t index(0), factor(1);
  FOR(i, dimensions.size())
  {
    index += factor * multi_index[i];
    factor *= dimensions[i];
  }
  this->IndexBoundCheck(index);

  return index;
}

template <class t_derived_class, class t_data_type>
auto
MultiArray<t_derived_class, t_data_type>::ComputeMultiIndex(std::size_t _index) const
{
  constexpr auto& dimensions = this->Derived().Dimensions;
  constexpr auto& n_entries = this->Derived().nEntries;

  DEBUG_ASSERT(isBounded(_index, 0, n_entries), "The passed index ", _index, " should be in the range [0, ", n_entries, "].")

  StaticArray<std::size_t, dimensions.size()> res;

//  std::size_t factor = Multiply(t_dimensions...);
//  for (size_t i = sizeof...(t_dimensions); i != 0; --i) {
//    factor /= dimensions[i - 1];
//    res[i - 1] = _index / factor;
//    assert(res[i - 1] < dimensions[i - 1]);
//    _index -= res[i - 1] * factor;
//  }
  return res;
}

/** Multi-dimensional Subscript Operator Overloads ************************************************************************************************************/
template <class t_derived_class, class t_data_type>
constexpr const t_data_type&
MultiArray<t_derived_class, t_data_type>::operator()(const std::convertible_to<std::size_t> auto ..._multi_index) const
{
  return this->operator[](ComputeLinearIndex(_multi_index...));
}

template <class t_derived_class, class t_data_type>
constexpr t_data_type&
MultiArray<t_derived_class, t_data_type>::operator()(std::convertible_to<std::size_t> auto ..._multi_index)
{
  return this->operator[](ComputeLinearIndex(_multi_index...));
}

/** Assignment Operator Overloads *****************************************************************************************************************************/
template <class t_derived_class, class t_data_type>
constexpr t_derived_class&
MultiArray<t_derived_class, t_data_type>::operator=(const std::initializer_list<std::initializer_list<t_data_type>>& _value_matrix) noexcept
{
  SizeCheck(this->Derived().Dimensions.size(), 2);
  SizeCheck(_value_matrix.size(), this->Derived().Dimensions[0].size());

  FOR(i, _value_matrix.size())
  {
    SizeCheck(_value_matrix[i].size(), this->Derived().Dimensions[1].size());
    std::size_t index(0);
    FOR_EACH(entry, _value_matrix[i]) this->Derived()[index++] = entry;
  }

  return this->Derived();
}

/***************************************************************************************************************************************************************
* Static Multi-dimensional Array Class
***************************************************************************************************************************************************************/
template <class t_data_type, std::size_t ...t_dimensions>
constexpr StaticMultiArray<t_data_type, t_dimensions...>::StaticMultiArray()
  : StaticMultiArray(GetStaticInitValue<t_data_type>())
{

}

template <class t_data_type, std::size_t ...t_dimensions>
constexpr StaticMultiArray<t_data_type, t_dimensions...>::StaticMultiArray(const t_data_type& _init_value)
  : std::array<t_data_type, nEntries>(DataContainersSupport::InitStaticArray<t_data_type, nEntries>(_init_value))
{

}

/***************************************************************************************************************************************************************
* Dynamic Multi-dimensional Array Class
***************************************************************************************************************************************************************/

/** Constructors/Destructors **********************************************************************************************************************************/
template <class t_data_type>
DynamicMultiArray<t_data_type>::DynamicMultiArray()
  : DynamicMultiArray(0)
{

}

template <class t_data_type>
DynamicMultiArray<t_data_type>::DynamicMultiArray(const std::convertible_to<std::size_t> auto ..._dimensions)
  : nEntries(Product(_dimensions...)), std::vector<t_data_type>(nEntries, GetDynamicInitValue<t_data_type>())
{

}

/** Multi-array Resize Functions ******************************************************************************************************************************/
template <class t_data_type>
void
DynamicMultiArray<t_data_type>::Resize(const std::convertible_to<std::size_t> auto ..._dimensions)
{
  nEntries = Product(_dimensions...);
  this->resize(nEntries, GetDynamicInitValue<t_data_type>());
}

}
