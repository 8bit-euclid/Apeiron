#pragma once

#include "../../../include/Global.h"
#include "Support.h"

namespace Apeiron{

template <class t_derived_class, class t_data_type>
class MultiArray;

/***************************************************************************************************************************************************************
* Array Abstract Base Class
***************************************************************************************************************************************************************/
template <class t_derived_class, class t_data_type>
class Array
{
private:
  constexpr t_derived_class& Derived() noexcept { return static_cast<t_derived_class&>(*this); }

  constexpr const t_derived_class& Derived() const noexcept { return static_cast<const t_derived_class&>(*this); }

protected:
  constexpr Array() {}

public:
  constexpr ~Array() = default;

  /*************************************************************************************************************************************************************
  * Size and Index Range-checking
  *************************************************************************************************************************************************************/
  /** Check if an index is in-range. */
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
  * Subscript Operator Overloads
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
* Ostream Operator Overloads
***************************************************************************************************************************************************************/
//template <class t_derived_class, class t_data_type>
//inline std::ostream& operator<<(std::ostream& _output_stream, const Array<t_derived_class, t_data_type>& _array_base)
//{
//  const t_derived_class& derived = static_cast<t_derived_class&>(_array_base);
//  FOR(i, derived.size()) _output_stream<<derived[i]<<(i == derived.size() - 1 ? "" : ", ");
//  return _output_stream;
//}

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
  /** Default constructor. */
  constexpr StaticArray() : StaticArray(GetTypeInitValue<t_data_type>()) {}

  /** Constructor from single initial value. */
  constexpr StaticArray(const t_data_type& _init_value) :
    std::array<t_data_type, t_array_size>(DataContainersSupport::InitStaticArray<t_data_type, t_array_size>(_init_value)) {}

  /** Constructor from an initializer list. */
  constexpr StaticArray(const std::initializer_list<t_data_type>& _initialiser_list) :
    std::array<t_data_type, t_array_size>(DataContainersSupport::InitStaticArray<t_data_type, t_array_size>(_initialiser_list)) {};

  /** Constructor from a first and last iterator. */
  template <class t_iterator>
  constexpr StaticArray(const t_iterator _first, const t_iterator _last) :
    std::array<t_data_type, t_array_size>(DataContainersSupport::InitStaticArray<t_data_type, t_array_size>(_first, _last)) {}

  /** Default destructor. */
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
  /** Default constructor. */
  DynamicArray() : std::vector<t_data_type>() {}

  /** Constructor based on given size. */
  DynamicArray(const std::size_t _array_size) : DynamicArray(_array_size, GetTypeInitValue<t_data_type>()) {}

  /** Constructor based on given size single initial value. */
  DynamicArray(const std::size_t _t_array_size, const t_data_type& _init_value) : std::vector<t_data_type>(_t_array_size, _init_value) {}

  /** Constructor from an initializer list. */
  DynamicArray(const std::initializer_list<t_data_type>& _initialiser_list) : std::vector<t_data_type>(_initialiser_list) {};

  /** Constructor from a first and last iterator. */
  template <class t_iterator>
  DynamicArray(const t_iterator _first, const t_iterator _last) : std::vector<t_data_type>(_first, _last) {}

  /** Default destructor. */
  ~DynamicArray() = default;

  using Base::operator[];
  using Base::operator=;
};

}
