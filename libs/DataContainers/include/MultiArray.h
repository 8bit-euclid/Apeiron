#pragma once

#include "../../../include/Global.h"
#include "Array.h"
#include "Support.h"

namespace Apeiron{

/***************************************************************************************************************************************************************
* Multi-dimensional Array Abstract Base Class
***************************************************************************************************************************************************************/
template <class t_derived_class, class t_data_type>
class MultiArray : public Array<t_derived_class, t_data_type>
{
protected:
  constexpr MultiArray() {}

public:
  constexpr ~MultiArray() = default;

  /*************************************************************************************************************************************************************
  * Multi-dimensional Subscript Indexing
  *************************************************************************************************************************************************************/
  constexpr std::size_t GetSingleIndex(const std::convertible_to<std::size_t> auto ..._multi_index) const
  {
//    DEBUG_ASSERT(areSizesEqual(sizeof...(_multi_index), this->Derived().Dimensions.size()), "Multi-index size mismatch.")

    const auto& dimensions = this->Derived().Dimensions;
    const auto& n_entries = this->Derived().nEntries;
    const auto n_dimensions = dimensions.size();
    const int multi_index[] = {_multi_index...};

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
  * Multi-dimensional Subscript Operator Overloads
  *************************************************************************************************************************************************************/
  constexpr const t_data_type& operator()(std::convertible_to<std::size_t> auto ..._multi_index) const
  {
//    DEBUG_ASSERT(areSizesEqual(sizeof...(_multi_index), this->Derived().nEntries), "Multi-index size mismatch.")
    return this->Derived()[GetSingleIndex(_multi_index...)];
  }

  constexpr t_data_type& operator()(std::convertible_to<std::size_t> auto ..._multi_index)
  {
//    DEBUG_ASSERT(areSizesEqual(sizeof...(_multi_index), this->Derived().nEntries), "Multi-index size mismatch.")
    return this->Derived()[GetSingleIndex(_multi_index...)];
  }
};

/***************************************************************************************************************************************************************
* Static Multi-dimensional Array Class
***************************************************************************************************************************************************************/
template <class t_data_type, std::size_t ...t_dimensions>
class StaticMultiArray : public std::array<t_data_type, (t_dimensions * ...)>,
                         public MultiArray<StaticMultiArray<t_data_type, t_dimensions...>, t_data_type>
{
  using BaseArray = Array<StaticMultiArray<t_data_type, t_dimensions...>, t_data_type>;
  using BaseMultiArray = MultiArray<StaticMultiArray<t_data_type, t_dimensions...>, t_data_type>;
  friend BaseArray;
  friend BaseMultiArray;

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

  using BaseArray::operator[];
  using BaseArray::operator=;
};

/***************************************************************************************************************************************************************
* Dynamic Multi-dimensional Array Class
***************************************************************************************************************************************************************/
template <class t_data_type>
class DynamicMultiArray : public std::vector<t_data_type>, public MultiArray<DynamicMultiArray<t_data_type>, t_data_type>
{
  using BaseArray = Array<DynamicMultiArray<t_data_type>, t_data_type>;
  using BaseMultiArray = MultiArray<DynamicMultiArray<t_data_type>, t_data_type>;
  friend BaseArray;
  friend BaseMultiArray;

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

  using BaseArray::operator[];
  using BaseArray::operator=;
};

}
