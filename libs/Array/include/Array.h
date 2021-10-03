#pragma once

#include "../../../include/Global.h"

namespace Apeiron
{

template <class data_type>
class DynamicArray : public std::vector<data_type>
{
  private:
  /** Check if an index is in-range. */
  inline void Index_Range_Check(const int _index) const
  {
    DEBUG_ASSERT(isBounded(_index, 0, this->size()), "The array index ", _index, " must be in the range [0, ", this->size(), ").")
  }

  public:
  DynamicArray() : std::vector<data_type>() {}

  DynamicArray(const std::size_t _array_size) : std::vector<data_type>(_array_size) {}

  DynamicArray(const std::size_t _array_size, const data_type& _init_value) : std::vector<data_type>(_array_size, _init_value) {}

  DynamicArray(const DynamicArray<data_type>& _array_to_copy) : std::vector<data_type>(_array_to_copy) {}

  DynamicArray(typename std::vector<data_type>::iterator _first, typename std::vector<data_type>::iterator _last) : std::vector<data_type>(_first, _last) {}

  DynamicArray(const std::initializer_list<data_type>& _initialiser_list) : std::vector<data_type>(_initialiser_list) {};

  /** Default destructor. */
  ~DynamicArray() = default;
};

template <class data_type, int array_size>
class StaticArray : public std::array<data_type, array_size>
{
  private:
  /** Check if an index is in-range. */
  inline void Index_Range_Check(const int _index) const
  {
    DEBUG_ASSERT(isBounded(_index, 0, this->size()), "The array index ", _index, " must be in the range [0, ", this->size(), ").")
  }

  public:
  StaticArray() : std::array<data_type, array_size>() {}

  StaticArray(const data_type& _init_value) : std::array<data_type, array_size>(_init_value) {}

  StaticArray(const StaticArray<data_type, array_size>& _array_to_copy) : std::array<data_type, array_size>(_array_to_copy) {}

  StaticArray(typename std::array<data_type, array_size>::iterator _first, typename std::array<data_type, array_size>::iterator _last)
    : std::array<data_type, array_size>(_first, _last) {}

  StaticArray(const std::initializer_list<data_type>& _initialiser_list) : std::array<data_type, array_size>(_initialiser_list) {};

  /** Default destructor. */
  ~StaticArray() = default;
};

}
