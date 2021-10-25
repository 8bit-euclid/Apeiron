#pragma once

#include "Debug.h"
#include "Types.h"
#include "StandardLibrary.h"

namespace Apeiron{

/***************************************************************************************************************************************************************
* Random Number Base Class
***************************************************************************************************************************************************************/
template <typename t_data_type, TypeCategory type_category = GetTypeCategory(t_data_type())>
class RandomBase
{
protected:
  std::random_device Device;
  std::mt19937 Generator;

  RandomBase() : Generator(Device()) {};

public:
  virtual ~RandomBase() {};

  /** Pure virtual subscript operator overload to generate a random number. */
  virtual t_data_type operator()() = 0;
};

/** Forward declaration of specialised Random classes. */
template <typename t_data_type, TypeCategory type_category = GetTypeCategory(t_data_type())>
class Random;

/***************************************************************************************************************************************************************
* Random Integer Class
***************************************************************************************************************************************************************/
template <typename t_data_type>
class Random<t_data_type, TypeCategory::Integer> : public RandomBase<t_data_type, TypeCategory::Integer>
{
  private:
  std::uniform_int_distribution<t_data_type> Distribution;

  public:
  Random(const t_data_type& _min, const t_data_type& _max) : RandomBase<t_data_type, TypeCategory::Integer>(), Distribution(_min, _max) {}
  Random() = delete;
  ~Random() = default;

  /** Overloaded subscript operator to generate a random integer. */
  inline t_data_type operator()() { return Distribution(this->Generator); }

  /** Reset min/max bounds for the distribution. */
  void Reset(const t_data_type& _min, const t_data_type& _max) { *this = Random(_min, _max); }
};

/***************************************************************************************************************************************************************
* Random Floating-point Class
***************************************************************************************************************************************************************/
template <typename t_data_type>
class Random<t_data_type, TypeCategory::FloatingPoint> : public RandomBase<t_data_type, TypeCategory::FloatingPoint>
{
  private:
  std::uniform_real_distribution<t_data_type> Distribution;

  public:
  Random(const t_data_type& _min, const t_data_type& _max) : RandomBase<t_data_type, TypeCategory::FloatingPoint>(), Distribution(_min, _max) {}
  Random() = delete;
  ~Random() = default;

  /** Overloaded subscript operator to generate a random floating-point value. */
  inline t_data_type operator()() { return Distribution(this->Generator); }

  /** Reset min/max bounds for the distribution. */
  void Reset(const t_data_type& _min, const t_data_type& _max) { *this = Random(_min, _max); }
};

/** Template deduction guide. */
template <typename t_data_type> Random(t_data_type, t_data_type) -> Random<t_data_type>;

}
