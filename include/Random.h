#pragma once

#include "Debug.h"
#include "Types.h"
#include "StandardLibrary.h"

#include <chrono>
#include <random>

namespace Apeiron{

/***************************************************************************************************************************************************************
* Random Number Base Class
***************************************************************************************************************************************************************/
template <typename t_data_type, TypeCategory type_category = GetTypeCategory<t_data_type>()>
class RandomBase
{
protected:
  std::random_device Device;
  std::mt19937 Generator;

  RandomBase() : Generator(Device()) {};

public:
  virtual ~RandomBase() {};

  // Pure virtual subscript operator overload to generate a random number.
  virtual t_data_type operator()() = 0;
};

// Forward declaration of Random class partial template specialisations.
template <typename t_data_type, TypeCategory type_category = GetTypeCategory<t_data_type>()>
class Random;

/***************************************************************************************************************************************************************
* Random Boolean Class
***************************************************************************************************************************************************************/
template <typename t_data_type>
class Random<t_data_type, TypeCategory::Boolean> : public RandomBase<t_data_type, TypeCategory::Boolean>
{
public:
  Random() : Distribution(0, 1) {}

  ~Random() = default;

  // Overloaded subscript operator to generate a random boolean.
  inline t_data_type operator()() { return Distribution(this->Generator); }

private:
  std::uniform_int_distribution<UChar> Distribution;
};

/***************************************************************************************************************************************************************
* Random Integer Class
***************************************************************************************************************************************************************/
template <typename t_data_type>
class Random<t_data_type, TypeCategory::Integral> : public RandomBase<t_data_type, TypeCategory::Integral>
{
public:
  Random() = delete;

  Random(const t_data_type& _min, const t_data_type& _max) :  Distribution(_min, _max) {}

  ~Random() = default;

  // Overloaded subscript operator to generate a random integer.
  inline t_data_type operator()() { return Distribution(this->Generator); }

  // Reset min/max bounds for the distribution.
  void Reset(const t_data_type& _min, const t_data_type& _max) { *this = Random(_min, _max); }

private:
  std::uniform_int_distribution<t_data_type> Distribution;
};

/***************************************************************************************************************************************************************
* Random Floating-point Class
***************************************************************************************************************************************************************/
template <typename t_data_type>
class Random<t_data_type, TypeCategory::FloatingPoint> : public RandomBase<t_data_type, TypeCategory::FloatingPoint>
{
public:
  Random() = delete;

  Random(const t_data_type& _min, const t_data_type& _max) : Distribution(_min, _max) {}

  ~Random() = default;

  // Overloaded subscript operator to generate a random floating-point value.
  inline t_data_type operator()() { return Distribution(this->Generator); }

  // Reset min/max bounds for the distribution.
  void Reset(const t_data_type& _min, const t_data_type& _max) { *this = Random(_min, _max); }

private:
  std::uniform_real_distribution<t_data_type> Distribution;
};

// Template deduction guide.
template <typename t_data_type> Random(t_data_type, t_data_type) -> Random<t_data_type>;

}
