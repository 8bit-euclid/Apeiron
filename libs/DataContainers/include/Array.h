#pragma once

#include "../../../include/Global.h"

namespace Apeiron
{

/***************************************************************************************************************************************************************
* Array Abstract Base Class
***************************************************************************************************************************************************************/

/** Array base class. */
template <class t_derived_class, class t_data_type>
class Array
{
private:
  t_derived_class& Derived() noexcept
  {
    return static_cast<t_derived_class&>(*this);
  }

  const t_derived_class& Derived() const noexcept
  {
    return static_cast<const t_derived_class&>(*this);
  }

public:
  /** Default constructor. */
  Array() {}

  /** Pure virtual destructor. */
  virtual ~Array() {};

  void Init()
  {
    Derived() = InitialiseType<t_data_type>();
  }

  /*************************************************************************************************************************************************************
  * Size and Index Range-checking
  *************************************************************************************************************************************************************/

  /** Check if an index is in-range. */
  inline void IndexBoundCheck(const std::size_t _index) const
  {
    DEBUG_ASSERT(isBounded(_index, std::size_t(0), Derived().size()), "The array index ", _index, " must be in the range [0, ", Derived().size() - 1, "].")
  }

  /** Check that two array sizes are equal. */
  inline void SizeCheck(const std::size_t _size0, const std::size_t _size1) const
  {
    DEBUG_ASSERT(_size0 == _size1, "The array sizes ", _size0, " and ", _size1, " must be equal.")
  }

  /*************************************************************************************************************************************************************
  * Subscript Operator Overloads
  *************************************************************************************************************************************************************/

  t_data_type& operator[](const std::size_t _index)
  {
    IndexBoundCheck(_index);
    return *(Derived().begin() + _index);
  }

  const t_data_type& operator[](const std::size_t _index) const
  {
    IndexBoundCheck(_index);
    return *(Derived().begin() + _index);
  }

  /*************************************************************************************************************************************************************
  * Assignment Operator Overloads
  *************************************************************************************************************************************************************/

  t_derived_class& operator=(const t_data_type& _value) noexcept
  {
    FOR_EACH(entry, Derived()) entry = _value;
    return Derived();
  }

  t_derived_class& operator=(const std::initializer_list<t_data_type>& _value_list)
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

namespace StaticArraySupport
{
template <class t_data_type, int array_size>
constexpr auto InitializeStaticArray(const t_data_type& _init_value)
{
  std::array<t_data_type, array_size> initialised_array;
  std::fill(initialised_array.begin(), initialised_array.end(), _init_value);
//  initialised_array.fill(_init_value);
//  for (int i = 0; i < len; i++)
//    initialised_array[i] = val;
  return initialised_array;
}
}

template <class t_data_type, int array_size>
class StaticArray : public std::array<t_data_type, array_size>, public Array<StaticArray<t_data_type, array_size>, t_data_type>
{
  using Base = Array<StaticArray<t_data_type, array_size>, t_data_type>;
  friend Base;

  public:
//  StaticArray() : std::array<t_data_type, array_size>() {}
  StaticArray() : StaticArray(InitialiseType<t_data_type>()) {}

  StaticArray(const t_data_type& _init_value) : std::array<t_data_type, array_size>() { this->fill(_init_value); }

  StaticArray(const StaticArray<t_data_type, array_size>& _array_to_copy) : std::array<t_data_type, array_size>(_array_to_copy) {}

  StaticArray(typename std::array<t_data_type, array_size>::iterator _first, typename std::array<t_data_type, array_size>::iterator _last)
    : std::array<t_data_type, array_size>(_first, _last) {}

  StaticArray(const std::initializer_list<t_data_type>& _initialiser_list) : std::array<t_data_type, array_size>{_initialiser_list} {};

  /** Default destructor. */
  ~StaticArray() = default;

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
  DynamicArray() : std::vector<t_data_type>() {}

  DynamicArray(const std::size_t _array_size) : std::vector<t_data_type>(_array_size) {}

  DynamicArray(const std::size_t _array_size, const t_data_type& _init_value) : std::vector<t_data_type>(_array_size, _init_value) {}

  DynamicArray(const DynamicArray<t_data_type>& _array_to_copy) : std::vector<t_data_type>(_array_to_copy) {}

  DynamicArray(typename std::vector<t_data_type>::iterator _first, typename std::vector<t_data_type>::iterator _last) : std::vector<t_data_type>(_first, _last) {}

  DynamicArray(const std::initializer_list<t_data_type>& _initialiser_list) : std::vector<t_data_type>(_initialiser_list) {};

  /** Default destructor. */
  ~DynamicArray() = default;

  using Base::operator[];
  using Base::operator=;
};

template <class t_data_type>
class TestClass
{
public:
  t_data_type Value;

  TestClass()
  {
    Value = InitialiseType<t_data_type>();
//    if(std::is_same_v<t_data_type, bool>) Value = false;
//    else if(std::is_same_v<t_data_type, int>) Value = -1;
//    else if(std::is_same_v<t_data_type, double>) Value = 0.0;
//    else throw "Unrecognised data type.";
  }

  ~TestClass() = default;
};

}
