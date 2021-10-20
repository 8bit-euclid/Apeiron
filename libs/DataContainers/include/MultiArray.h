#pragma once

#include "../../../include/Global.h"

namespace Apeiron{

template <class t_data_type, int t_array_size>
class StaticMultiArray : public std::array<t_data_type, t_array_size>
{
  private:
  /** Check if an index is in-range. */
  inline void Index_Range_Check(const int _index) const
  {
    DEBUG_ASSERT(isBounded(_index, 0, this->size()), "The array index ", _index, " must be in the range [0, ", this->size(), ").")
  }

  public:
  /** Default constructor. */
  StaticArray() = default;

  /** Default destructor. */
  ~StaticArray() = default;
};

template <class t_data_type>
class DynamicMultiArray : public std::vector<t_data_type>
{
  private:
  /** Check if an index is in-range. */
  inline void Index_Range_Check(const int _index) const
  {
    DEBUG_ASSERT(isBounded(_index, 0, this->size()), "The array index ", _index, " must be in the range [0, ", this->size(), ").")
  }

  public:
  /** Default constructor. */
  DynamicArray() = default;

  /** Default destructor. */
  ~DynamicArray() = default;
};

}