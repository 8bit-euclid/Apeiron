#pragma once

#include "../../../include/Global.h"
#include "../../Array/include/Array.h"

namespace Apeiron{

template <typename data_type, int n_values>
struct SortObject
{
  std::size_t Index;
  StaticArray<data_type, n_values> Values;
};

template <typename data_type, int n_values>
class Sort
{
  private:
  DynamicArray<SortObject<data_type, n_values>> SortObjects;

  public:
  Sort() {}

  Sort(const std::size_t _n_sort_objects)
  {

  }

  ~Sort() = default;
};

}
