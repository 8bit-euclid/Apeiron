#pragma once

#include "Debug.h"
#include "Types.h"
#include "StandardLibrary.h"

namespace Apeiron{

template <typename data_type, TypeCategory type_category = GetTypeCategory(data_type())>
struct Random;

template <typename data_type>
struct Random<data_type, TypeCategory::Integer>
{
  std::random_device Device;
  std::mt19937 Generator;
  std::uniform_int_distribution<data_type> Distribution;

  Random() = delete;
  Random(const data_type& _min, const data_type& _max) : Generator(Device()), Distribution(_min, _max) {}
  ~Random() = default;

  inline data_type Get() { return Distribution(Generator); }
};

template <typename data_type>
struct Random<data_type, TypeCategory::FloatingPoint>
{
  std::random_device Device;
  std::mt19937 Generator;
  std::uniform_real_distribution<data_type> Distribution;

  Random() = delete;
  Random(const data_type& _min, const data_type& _max) : Generator(Device()), Distribution(_min, _max) {}
  ~Random() = default;

  inline data_type Get() { return Distribution(Generator); }
};

/** Template deduction guide. */
template <typename data_type> Random(data_type, data_type) -> Random<data_type>;

}
