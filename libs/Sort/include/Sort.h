#pragma once

#include "../../../include/Global.h"
#include <DataContainer/include/Array.h>

namespace aprn{

template<typename T, int n_values>
struct SortObject
{
  size_t Index;
  StaticArray<T, n_values> Values;

  SortObject() {}

  SortObject(const size_t _index, const StaticArray<T, n_values>& _values) : Index(_index), Values(_values) {}
};

template<typename T, unsigned int n_values = 1>
class Sort
{
  private:
  DynamicArray<SortObject<T, n_values>> SortObjects;

  public:
  Sort() {}

  Sort(const size_t _n_sort_objects)
  {
    Init(_n_sort_objects);
  }

  ~Sort() = default;

  inline void Init(const size_t _n_sort_objects)
  {
    STATIC_ASSERT(isArithmetic<T>(), "Can only sort numerical data types currently.")
    SortObjects.reserve(_n_sort_objects);
  }

  inline void AddSortObject(const size_t _index, const StaticArray<T, n_values>& _values)
  {
    SortObjects.emplace_back(_index, _values);
  }

  inline void SortAll(const bool _is_sort_in_ascending = true)
  {
    std::sort(SortObjects.begin(), SortObjects.end(), [](const SortObject<T, n_values>& _a, const SortObject<T, n_values>& _b)
                                                      {
                                                        FOR(i, n_values)
                                                        {
                                                          if(_a[i] < _b[i]) return true;
                                                          else if(_a[i] > _b[i]) return false;
                                                          else continue;
                                                        }
                                                      });

    if(!_is_sort_in_ascending) std::reverse(SortObjects.begin(), SortObjects.end());
  }

  inline size_t GetIndex(const size_t _i_object)
  {
    return SortObjects[_i_object].Index;
  }

  inline const StaticArray<T, n_values>& GetValues(const size_t _i_object)
  {
    return SortObjects[_i_object].Values;
  }
};

}
