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

  /** Pure virtual () operator overload to generate a random number. */
  virtual data_type operator()() = 0;
};

/** Forward declaration of specialised Random classes. */
template <typename data_type, TypeCategory type_category = GetTypeCategory(data_type())>
class Random;

/** Random integer class. */
template <typename data_type>
class Random<data_type, TypeCategory::Integer> : public RandomBase<data_type, TypeCategory::Integer>
{
  private:
  std::uniform_int_distribution<data_type> Distribution;

  public:
  Random(const data_type& _min, const data_type& _max) : RandomBase<data_type, TypeCategory::Integer>(), Distribution(_min, _max) {}
  Random() = delete;
  ~Random() = default;

  /** Overloaded () operator to generate a random integer. */
  inline data_type operator()() { return Distribution(this->Generator); }

  /** Reset min/max bounds for the distribution. */
  void Reset(const data_type& _min, const data_type& _max) { *this = Random(_min, _max); }
};

/** Random floating-point class. */
template <typename data_type>
class Random<data_type, TypeCategory::FloatingPoint> : public RandomBase<data_type, TypeCategory::FloatingPoint>
{
  private:
  std::uniform_real_distribution<data_type> Distribution;

  public:
  Random(const data_type& _min, const data_type& _max) : RandomBase<data_type, TypeCategory::FloatingPoint>(), Distribution(_min, _max) {}
  Random() = delete;
  ~Random() = default;

  /** Overloaded () operator to generate a random floating-point value. */
  inline data_type operator()() { return Distribution(this->Generator); }

  /** Reset min/max bounds for the distribution. */
  void Reset(const data_type& _min, const data_type& _max) { *this = Random(_min, _max); }
};

/** Template deduction guide. */
template <typename data_type> Random(data_type, data_type) -> Random<data_type>;

}
