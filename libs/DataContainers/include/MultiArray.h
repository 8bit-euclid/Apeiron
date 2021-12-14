#pragma once

#include "../../../include/Global.h"
#include "Array.h"

namespace Apeiron{

/***************************************************************************************************************************************************************
* Multi-dimensional Array Abstract Base Class
***************************************************************************************************************************************************************/
template <class t_derived_class, class t_data_type>
class MultiArray
  : public Array<t_derived_class, t_data_type>
{
protected:
  constexpr MultiArray() = default;

public:
  constexpr ~MultiArray() = default;

  /** Size and Index Range-checking ***************************************************************************************************************************/
  constexpr void MultiIndexBoundCheck(const std::convertible_to<std::size_t> auto ..._multi_index) const;

  /** Multi-dimensional Subscript Index Toggling **************************************************************************************************************/
  constexpr std::size_t ComputeLinearIndex(const std::convertible_to<std::size_t> auto ..._multi_index) const;

  auto ComputeMultiIndex(std::size_t _index) const;

  /** Multi-dimensional Subscript Operator Overloads **********************************************************************************************************/
  constexpr const t_data_type& operator()(const std::convertible_to<std::size_t> auto ..._multi_index) const;

  constexpr t_data_type& operator()(std::convertible_to<std::size_t> auto ..._multi_index);

  /** Assignment Operator Overloads ***************************************************************************************************************************/
  constexpr t_derived_class& operator=(const std::initializer_list<std::initializer_list<t_data_type>>& _value_matrix) noexcept;
};

/***************************************************************************************************************************************************************
* Static Multi-dimensional Array Class
***************************************************************************************************************************************************************/
template <class t_data_type, std::size_t ...t_dimensions>
class StaticMultiArray
  : public std::array<t_data_type, Product(t_dimensions...)>, public MultiArray<StaticMultiArray<t_data_type, t_dimensions...>, t_data_type>
{
  using Base = MultiArray<StaticMultiArray<t_data_type, t_dimensions...>, t_data_type>;
  friend Base;

private:
  constexpr static StaticArray<std::size_t, sizeof...(t_dimensions)> Dimensions{t_dimensions...};
  constexpr static std::size_t nEntries{Product(Dimensions.begin(), Dimensions.end())};

public:
  constexpr StaticMultiArray();

  constexpr StaticMultiArray(const t_data_type& _init_value);

  constexpr ~StaticMultiArray() = default;

  using Base::operator[];
  using Base::operator();
  using Base::operator=;
};

/***************************************************************************************************************************************************************
* Dynamic Multi-dimensional Array Class
***************************************************************************************************************************************************************/
template <class t_data_type>
class DynamicMultiArray
  : public std::vector<t_data_type>, public MultiArray<DynamicMultiArray<t_data_type>, t_data_type>
{
  using Base = MultiArray<DynamicMultiArray<t_data_type>, t_data_type>;
  friend Base;

private:
  DynamicArray<std::size_t> Dimensions;
  std::size_t nEntries;

public:
  /** Constructors/Destructors ********************************************************************************************************************************/
  DynamicMultiArray();

  DynamicMultiArray(const std::convertible_to<std::size_t> auto ..._dimensions);

  ~DynamicMultiArray() = default;

  /** Multi-array Resize Functions ****************************************************************************************************************************/
  void Resize(const std::convertible_to<std::size_t> auto ..._dimensions);

  using Base::operator[];
  using Base::operator();
  using Base::operator=;
};

}

#include "../src/MultiArray.cpp"
