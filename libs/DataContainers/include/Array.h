#pragma once

#include "../../../include/Global.h"

namespace Apeiron{

/***************************************************************************************************************************************************************
* Forward Declarations
***************************************************************************************************************************************************************/
template <class t_derived_class, class t_data_type>
class MultiArray;

/***************************************************************************************************************************************************************
* Array Abstract Base Class
***************************************************************************************************************************************************************/
template <class t_derived_class, class t_data_type>
class Array
{
protected:
  constexpr Array() = default;

public:
  constexpr ~Array() = default;

  /** Size and Index Range-checking ***************************************************************************************************************************/
  constexpr void IndexBoundCheck(const std::size_t _index) const;

  constexpr void SizeCheck(const std::size_t _size0, const std::size_t _size1) const;

  /** Subscript Operator Overloads ****************************************************************************************************************************/
  constexpr t_data_type& operator[](const std::size_t _index);

  constexpr const t_data_type& operator[](const std::size_t _index) const;

  /** Assignment Operator Overloads ***************************************************************************************************************************/
  template <class t_rhs_type>
  constexpr t_derived_class& operator=(const t_rhs_type& _value) noexcept;

  constexpr t_derived_class& operator=(const std::initializer_list<t_data_type>& _value_list) noexcept;

private:
  /** Derived Class Access ************************************************************************************************************************************/
  constexpr t_derived_class& Derived() noexcept;

  constexpr const t_derived_class& Derived() const noexcept;
};

/** Operator Overloads ****************************************************************************************************************************************/
template <class t_derived_class, class t_data_type>
std::ostream& operator<<(std::ostream& _output_stream, const Array<t_derived_class, t_data_type>& _array_base);

/***************************************************************************************************************************************************************
* Static Array Class
***************************************************************************************************************************************************************/
template <class t_data_type, std::size_t t_array_size>
class StaticArray : public std::array<t_data_type, t_array_size>, public Array<StaticArray<t_data_type, t_array_size>, t_data_type>
{
private:
  using Base = Array<StaticArray<t_data_type, t_array_size>, t_data_type>;
  friend Base;

public:
  constexpr StaticArray();

  constexpr StaticArray(const t_data_type& _init_value);

  constexpr StaticArray(const std::initializer_list<t_data_type>& _initialiser_list);

  template <class t_iterator>
  constexpr StaticArray(const t_iterator _first, const t_iterator _last);

  constexpr ~StaticArray() = default;

  using Base::operator[];
  using Base::operator=;
};

/***************************************************************************************************************************************************************
* Dynamic Array Class
***************************************************************************************************************************************************************/
template <class t_data_type>
class DynamicArray : public std::vector<t_data_type>, public Array<DynamicArray<t_data_type>, t_data_type>
{
  using Base = Array<DynamicArray<t_data_type>, t_data_type>;
  friend Base;

public:
  DynamicArray();

  DynamicArray(const std::size_t _array_size);

  DynamicArray(const std::size_t _t_array_size, const t_data_type& _init_value);

  DynamicArray(const std::initializer_list<t_data_type>& _initialiser_list);

  template <class t_iterator>
  DynamicArray(const t_iterator _first, const t_iterator _last);

  ~DynamicArray() = default;

  using Base::operator[];
  using Base::operator=;
};

}

#include "../src/Array.cpp"
