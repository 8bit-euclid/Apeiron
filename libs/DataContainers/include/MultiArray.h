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
  /** Constructor. */
  constexpr MultiArray() {}

public:
  /** Destructor. */
  ~MultiArray() = default;

  /*************************************************************************************************************************************************************
  * Size and Index Range-checking
  *************************************************************************************************************************************************************/
  /** Check if an index is in-range. */
  constexpr void IndexBoundCheck(const std::size_t _index) const
  {
    DEBUG_ASSERT(isBounded(_index, std::size_t(0), this->Derived().size()), "The array index ", _index, " must be in the range [0, ", this->Derived().size() - 1, "].")
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
    return *(this->Derived().begin() + _index);
  }

  constexpr const t_data_type& operator[](const std::size_t _index) const
  {
    IndexBoundCheck(_index);
    return *(this->Derived().begin() + _index);
  }

  /*************************************************************************************************************************************************************
  * Assignment Operator Overloads
  *************************************************************************************************************************************************************/
  constexpr t_derived_class& operator=(const t_data_type& _value) noexcept
  {
    FOR_EACH(entry, this->Derived()) entry = _value;
    return this->Derived();
  }

  constexpr t_derived_class& operator=(const std::initializer_list<t_data_type>& _value_list)
  {
    SizeCheck(_value_list.size(), this->Derived().size());
    std::size_t index(0);
    FOR_EACH(entry, _value_list) this->Derived()[index++] = entry;
    return this->Derived();
  }
};

/***************************************************************************************************************************************************************
* Static Multi-dimensional Array Class
***************************************************************************************************************************************************************/
template <class t_data_type, std::size_t t_array_size>
class StaticMultiArray : public std::array<t_data_type, t_array_size>, public MultiArray<StaticMultiArray<t_data_type, t_array_size>, t_data_type>
{
private:
  using Base = MultiArray<StaticMultiArray<t_data_type, t_array_size>, t_data_type>;
  friend Base;

public:
  /** Default constructor. */
  constexpr StaticMultiArray() : StaticMultiArray(GetTypeInitValue<t_data_type>()) {}

  /** Constructor from single initial value. */
  constexpr StaticMultiArray(const t_data_type& _init_value) :
          std::array<t_data_type, t_array_size>(DataContainersSupport::InitStaticArray<t_data_type, t_array_size>(_init_value)) {}

  /** Constructor from an initializer list. */
  constexpr StaticMultiArray(const std::initializer_list<t_data_type>& _initialiser_list) :
          std::array<t_data_type, t_array_size>(DataContainersSupport::InitStaticArray<t_data_type, t_array_size>(_initialiser_list)) {};

  /** Constructor from a first and last iterator. */
  template <class t_iterator>
  constexpr StaticMultiArray(const t_iterator _first, const t_iterator _last) :
          std::array<t_data_type, t_array_size>(DataContainersSupport::InitStaticArray<t_data_type, t_array_size>(_first, _last)) {}

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

public:
  /** Default constructor. */
  DynamicMultiArray() : std::vector<t_data_type>() {}

  /** Constructor based on given size. */
  DynamicMultiArray(const std::size_t _array_size) : DynamicMultiArray(_array_size, GetTypeInitValue<t_data_type>()) {}

  /** Constructor based on given size single initial value. */
  DynamicMultiArray(const std::size_t _t_array_size, const t_data_type& _init_value) : std::vector<t_data_type>(_t_array_size, _init_value) {}

  /** Constructor from an initializer list. */
  DynamicMultiArray(const std::initializer_list<t_data_type>& _initialiser_list) : std::vector<t_data_type>(_initialiser_list) {};

  /** Constructor from a first and last iterator. */
  template <class t_iterator>
  DynamicMultiArray(const t_iterator _first, const t_iterator _last) : std::vector<t_data_type>(_first, _last) {}

  /** Default destructor. */
  ~DynamicMultiArray() = default;

  using Base::operator[];
  using Base::operator=;
};

}
