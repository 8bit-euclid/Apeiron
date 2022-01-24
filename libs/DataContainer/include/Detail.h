#pragma once

#include "../../../include/Global.h"

namespace Apeiron {
namespace Detail {

/***************************************************************************************************************************************************************
* Static Array Support Functions
***************************************************************************************************************************************************************/

/** Initialise an std::array with a single value. */
template <typename T, std::size_t t_array_size>
constexpr auto InitStaticArray(const T& _value)
{
  std::array<T, t_array_size> initialised_array;
  std::fill(initialised_array.begin(), initialised_array.end(), _value);
  return initialised_array;
}

/** Initialise an std::array with an initializer list. */
template <typename T, std::size_t t_array_size>
constexpr auto InitStaticArray(const std::initializer_list<T>& _list)
{
  areSizesEqual(t_array_size, _list.size()) ? true : throw "The size of the initializer list must equal the array size.";
  std::array<T, t_array_size> initialised_array;
  std::copy(_list.begin(), _list.end(), initialised_array.begin());
  return initialised_array;
}

/** Initialise an std::array with a first and last iterator. */
template <typename T, std::size_t t_array_size, class iter>
constexpr auto InitStaticArray(const iter _first, const iter _last)
{
  isTypeEqual<T, typename std::iterator_traits<iter>::value_type>() ? true : throw "The number of iterators must equal the array size.";
  areSizesEqual(t_array_size, std::distance(_first, _last)) ? true : throw "The number of iterators must equal the array size.";
  std::array<T, t_array_size> initialised_array;
  std::copy(_first, _last, initialised_array.begin());
  return initialised_array;
}

/***************************************************************************************************************************************************************
* Numeric Data Container Support
***************************************************************************************************************************************************************/
template <typename T, class derived>
class NumericContainer
{
protected:
  constexpr NumericContainer() { STATIC_ASSERT(isNumber<T>(), "Tensors can only be populated with numerical values.") }

public:
  /** Operator overloads. */

private:
  /** Derived class access. */
  constexpr derived&
  Derived() noexcept { return static_cast<derived&>(*this); }

  constexpr const derived&
  Derived() const noexcept { return static_cast<const derived&>(*this); }
};

}
}