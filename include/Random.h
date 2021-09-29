#pragma once

#include "Debug.h"
#include "Types.h"
#include "StandardLibrary.h"

namespace Apeiron{

/** Random base class. */
template <typename data_type, TypeCategory type_category = GetTypeCategory(data_type())>
class RandomBase
{
  protected:
  std::random_device Device; /** Random device. */
  std::mt19937 Generator;    /** Random number generator. */

  /** Protected constructor. */
  RandomBase() : Generator(Device()) {};

  public:
  /** Virtual destructor. */
  virtual ~RandomBase() {};

  /** Pure virtual random number generating method. */
  virtual inline data_type Get() = 0;
};

template <typename data_type, TypeCategory type_category = GetTypeCategory(data_type())>
class Random;

/** Random integer class. */
template <typename data_type>
class Random<data_type, TypeCategory::Integer> : public RandomBase<data_type, TypeCategory::Integer>
{
  private:
  std::uniform_int_distribution<data_type> Distribution;

  public:
  Random() = delete;
  Random(const data_type& _min, const data_type& _max) : RandomBase<data_type, TypeCategory::Integer>(), Distribution(_min, _max) {}
  ~Random() = default;

  inline data_type Get() { return Distribution(this->Generator); }
};

/** Random floating-point class. */
template <typename data_type>
class Random<data_type, TypeCategory::FloatingPoint> : public RandomBase<data_type, TypeCategory::FloatingPoint>
{
  private:
  std::uniform_real_distribution<data_type> Distribution;

  public:
  Random() = delete;
  Random(const data_type& _min, const data_type& _max) : RandomBase<data_type, TypeCategory::FloatingPoint>(), Distribution(_min, _max) {}
  ~Random() = default;

  inline data_type Get() { return Distribution(this->Generator); }
};

/** Template deduction guide. */
template <typename data_type> Random(data_type, data_type) -> Random<data_type>;

}
