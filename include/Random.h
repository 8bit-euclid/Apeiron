#pragma once

#include "Debug.h"
#include "Types.h"
#include "StandardLibrary.h"

#include <chrono>
#include <random>

namespace aprn{

/***************************************************************************************************************************************************************
* Abstract Random Number Base Class
***************************************************************************************************************************************************************/
template<typename T, TypeCategory cat = GetTypeCategory<T>()>
class RandomBase
{
protected:
  RandomBase() : Generator(Device()) {}

public:
  /** Subscript operator overload to generate a random number. */
  virtual T operator()() = 0;

protected:
  std::random_device Device;
  std::mt19937 Generator;
};

/** Forward declaration of Random class and templatededuction guide. */
template<typename T, TypeCategory cat = GetTypeCategory<T>()> class Random;
template<typename T> Random(T, T) -> Random<T>;

/***************************************************************************************************************************************************************
* Random Boolean Class
***************************************************************************************************************************************************************/
template<typename T>
class Random<T, TypeCategory::Boolean> : public RandomBase<T, TypeCategory::Boolean>
{
public:
  Random() : Distribution(0, 1) {}

  /** Overloaded subscript operator to generate a random boolean. */
  inline T operator()() override { return Distribution(this->Generator); }

private:
  std::uniform_int_distribution<UChar> Distribution;
};

/***************************************************************************************************************************************************************
* Random Integer Class
***************************************************************************************************************************************************************/
template<typename T>
class Random<T, TypeCategory::Integral> : public RandomBase<T, TypeCategory::Integral>
{
public:
  Random() : Distribution(-1, 1) {}

  Random(const T _min, const T _max) :  Distribution(_min, _max) {}

  /** Overloaded subscript operator to generate a random integer. */
  inline T operator()() override { return Distribution(this->Generator); }

  /** Reset min/max bounds for the distribution. */
  void Reset(const T _min, const T _max) { Distribution = std::uniform_int_distribution<T>(_min, _max); }

private:
  std::uniform_int_distribution<T> Distribution;
};

/***************************************************************************************************************************************************************
* Random Floating-point Class
***************************************************************************************************************************************************************/
template<typename T>
class Random<T, TypeCategory::FloatingPoint> : public RandomBase<T, TypeCategory::FloatingPoint>
{
public:
  Random() : Distribution(-One, One) {}

  Random(const T _min, const T _max) : Distribution(_min, _max) {}

  /** Overloaded subscript operator to generate a random floating-point value. */
  inline T operator()() override { return Distribution(this->Generator); }

  /** Reset min/max bounds for the distribution. */
  void Reset(const T _min, const T _max) { Distribution = std::uniform_real_distribution<T>(_min, _max); }

private:
  std::uniform_real_distribution<T> Distribution;
};

}
