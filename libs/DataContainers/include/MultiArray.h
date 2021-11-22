#pragma once

#include "../../../include/Global.h"
#include "Array.h"
#include "Support.h"

namespace Apeiron{

/***************************************************************************************************************************************************************
* Multi-dimensional Array Abstract Base Class
***************************************************************************************************************************************************************/
template <class t_derived_class, class t_data_type>
class MultiArray
{
private:
  constexpr t_derived_class& Derived() noexcept { return static_cast<t_derived_class&>(*this); }

  constexpr const t_derived_class& Derived() const noexcept { return static_cast<const t_derived_class&>(*this); }

protected:
  constexpr MultiArray() {}

public:
  constexpr ~MultiArray() = default;

  /*************************************************************************************************************************************************************
  * Size and Index Range-checking
  *************************************************************************************************************************************************************/
  /** Check if linear index is in-range. */
  constexpr void IndexBoundCheck(const std::size_t _index) const
  {
    DEBUG_ASSERT(isBounded(_index, std::size_t(0), Derived().size()), "The array index ", _index, " must be in the range [0, ", Derived().size() - 1, "].")
  }

  /** Check that two array sizes are equal. */
  constexpr void SizeCheck(const std::size_t _size0, const std::size_t _size1) const
  {
    DEBUG_ASSERT(areSizesEqual(_size0, _size1), "The array sizes ", _size0, " and ", _size1, " must be equal.")
  }

  /*************************************************************************************************************************************************************
  * Multi-dimensional Subscript Indexing
  *************************************************************************************************************************************************************/
  constexpr std::size_t GetLinearIndex(const std::convertible_to<std::size_t> auto ..._multi_index) const
  {
//    DEBUG_ASSERT(areSizesEqual(sizeof...(_multi_index), this->Derived().Dimensions.size()), "Multi-index size mismatch.")

    const auto& dimensions = this->Derived().WindowDimensions;
    const auto& n_entries = this->Derived().nEntries;
    const auto n_dimensions = dimensions.size();
    const std::size_t multi_index[] = {static_cast<std::size_t>(_multi_index)...};

    std::size_t index(0), factor(1);
    FOR(i, n_dimensions)
    {
//      DEBUG_ASSERT(multi_index[i] < dimensions[i], "Multi index component ", multi_index[i]," must be lesser than ", dimensions[i],".")
      index += factor * multi_index[i];
      factor *= dimensions[i];
    }

//    DEBUG_ASSERT(isBounded(index, 0, n_entries), "The computed index ", index, " should be in the range [0, ", n_entries, "].")
    return index;
  }

//StaticArray<std::size_t, sizeof...(t_dimensions)> ComputeMultiIndex(std::size_t _index)
//{
//  DEBUG_ASSERT(isBounded(_index, 0, Multiply(t_dimensions...)), "The passed index ", _index, " should be in the range [0, ", Multiply(t_dimensions...), "].")
//
//  constexpr std::size_t dimensions[] = {t_dimensions...};
//  StaticArray<std::size_t, sizeof...(t_dimensions)> res;
//
//  std::size_t factor = Multiply(t_dimensions...);
//  for (size_t i = sizeof...(t_dimensions); i != 0; --i) {
//    factor /= dimensions[i - 1];
//    res[i - 1] = _index / factor;
//    assert(res[i - 1] < dimensions[i - 1]);
//    _index -= res[i - 1] * factor;
//  }
//  return res;
//}

  /*************************************************************************************************************************************************************
  * Linear Subscript Operator Overloads
  *************************************************************************************************************************************************************/
  constexpr t_data_type& operator[](const std::size_t _index)
  {
    IndexBoundCheck(_index);
    return *(Derived().begin() + _index);
  }

  constexpr const t_data_type& operator[](const std::size_t _index) const
  {
    IndexBoundCheck(_index);
    return *(Derived().begin() + _index);
  }

  /*************************************************************************************************************************************************************
  * Multi-dimensional Subscript Operator Overloads
  *************************************************************************************************************************************************************/
  constexpr const t_data_type& operator()(std::convertible_to<std::size_t> auto ..._multi_index) const
  {
//    DEBUG_ASSERT(areSizesEqual(sizeof...(_multi_index), this->Derived().nEntries), "Multi-index size mismatch.")
    return this->operator[](GetLinearIndex(_multi_index...));
  }

  constexpr t_data_type& operator()(std::convertible_to<std::size_t> auto ..._multi_index)
  {
//    DEBUG_ASSERT(areSizesEqual(sizeof...(_multi_index), this->Derived().nEntries), "Multi-index size mismatch.")
    return this->operator[](GetLinearIndex(_multi_index...));
  }

  /*************************************************************************************************************************************************************
  * Assignment Operator Overloads
  *************************************************************************************************************************************************************/
  constexpr t_derived_class& operator=(const t_data_type& _value) noexcept
  {
    FOR_EACH(entry, Derived()) entry = _value;
    return Derived();
  }

  constexpr t_derived_class& operator=(const std::initializer_list<t_data_type>& _value_list)
  {
    SizeCheck(_value_list.size(), Derived().size());
    std::size_t index(0);
    FOR_EACH(entry, _value_list) Derived()[index++] = entry;
    return Derived();
  }
};

/***************************************************************************************************************************************************************
* Static Multi-dimensional Array Class
***************************************************************************************************************************************************************/
template <class t_data_type, std::size_t ...t_dimensions>
class StaticMultiArray : public std::array<t_data_type, (t_dimensions * ...)>,
                         public MultiArray<StaticMultiArray<t_data_type, t_dimensions...>, t_data_type>
{
  using Base = MultiArray<StaticMultiArray<t_data_type, t_dimensions...>, t_data_type>;
  friend Base;

private:
  constexpr static StaticArray<std::size_t, sizeof...(t_dimensions)> Dimensions{t_dimensions...};
  constexpr static std::size_t nEntries = Multiply(Dimensions.begin(), Dimensions.end());

public:
  /** Default constructor. */
  constexpr StaticMultiArray() : StaticMultiArray(GetTypeInitValue<t_data_type>()) {}

  /** Constructor from single initial value. */
  constexpr StaticMultiArray(const t_data_type& _init_value) :
    std::array<t_data_type, nEntries>(DataContainersSupport::InitStaticArray<t_data_type, nEntries>(_init_value)) {}

  /** Default destructor. */
  constexpr ~StaticMultiArray() = default;

  using Base::operator[];
  using Base::operator=;
};

/***************************************************************************************************************************************************************
* Dynamic Multi-dimensional Array Class
***************************************************************************************************************************************************************/
template <class t_data_type>
class DynamicMultiArray : public std::vector<t_data_type>, public MultiArray<DynamicMultiArray<t_data_type>, t_data_type>
{
  using Base = MultiArray<DynamicMultiArray<t_data_type>, t_data_type>;
  friend Base;

private:
  DynamicArray<std::size_t> Dimensions;
  std::size_t nEntries;

public:
  /** Default constructor. */
  DynamicMultiArray() : std::vector<t_data_type>() {}

  /** Constructor based on given size. */
  template <std::size_t ...t_dimensions>
  DynamicMultiArray(const std::size_t _array_size) : DynamicMultiArray(_array_size, GetTypeInitValue<t_data_type>()) {}

  /** Constructor based on given size single initial value. */
  template <std::size_t ...t_dimensions>
  DynamicMultiArray(const std::size_t _t_array_size, const t_data_type& _init_value) :
    nEntries((t_dimensions * ...)), std::vector<t_data_type>(nEntries, _init_value) {}

  /** Default destructor. */
  ~DynamicMultiArray() = default;

  using Base::operator[];
  using Base::operator=;
};

}
