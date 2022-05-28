#ifndef MULTIARRAY_TEMPLATE_DEF
#define MULTIARRAY_TEMPLATE_DEF

#include "../../../include/Global.h"
#include "../include/MultiArray.h"

namespace aprn {

/***************************************************************************************************************************************************************
* Multi-dimensional Array Abstract Base Class
***************************************************************************************************************************************************************/

/** Size and index range-checking */
template<typename T, class derived>
constexpr void
MultiArray<T, derived>::MultiIndexBoundCheck(const std::convertible_to<size_t> auto... _multi_index) const
{
#ifdef DEBUG_MODE
  ASSERT(areSizesEqual(sizeof...(_multi_index), Derived().Dimensions.size()), "Multi-index size mismatch.")

  auto& dims = Derived().Dimensions;
  const size_t multi_index[] = {static_cast<size_t>(_multi_index)...};
  FOR(i, dims.size()) ASSERT(multi_index[i] < dims[i], "Multi index component ", multi_index[i], " must be lesser than ", dims[i], ".")
#endif
}

/** Multi-dimensional subscript index toggling */
template<typename T, class derived>
constexpr size_t
MultiArray<T, derived>::ComputeLinearIndex(const std::convertible_to<size_t> auto... _multi_index) const
{
  MultiIndexBoundCheck(_multi_index...);

  auto& dims = Derived().Dimensions;
  const size_t multi_index[] = {static_cast<size_t>(_multi_index)...};

  size_t index(0), factor(1);
  FOR(i, dims.size())
  {
    index += factor * multi_index[i];
    factor *= dims[i];
  }
//  this->IndexBoundCheck(index);

  return index;
}

template<typename T, class derived>
constexpr auto
MultiArray<T, derived>::ComputeMultiIndex(size_t _index) const
{
  EXIT("Yet to complete.")

//  constexpr auto& dims = Derived().Dimensions;
//  constexpr auto& n_entries = Derived().nEntries;
//
//  DEBUG_ASSERT(isBounded(_index, 0, n_entries), "The passed index ", _index, " should be in the range [0, ", n_entries, "].")
//
//  StaticArray<size_t, dims.size()> res;

//  size_t factor = Multiply(dims...);
//  for (size_t i = sizeof...(dims); i != 0; --i) {
//    factor /= dims[i - 1];
//    res[i - 1] = _index / factor;
//    assert(res[i - 1] < dims[i - 1]);
//    _index -= res[i - 1] * factor;
//  }
//  return res;
  return 0;
}

/** Operator overloads. */
template<typename T, class derived>
constexpr T&
MultiArray<T, derived>::operator()(std::convertible_to<size_t> auto... _multi_index)
{
  return Derived().Entries[ComputeLinearIndex(_multi_index...)];
}

template<typename T, class derived>
constexpr const T&
MultiArray<T, derived>::operator()(const std::convertible_to<size_t> auto... _multi_index) const
{
  return Derived().Entries[ComputeLinearIndex(_multi_index...)];
}

template<typename T, class derived>
constexpr derived&
MultiArray<T, derived>::operator=(const std::initializer_list<T>& _value_array) noexcept
{
  derived& derived_class = Derived();

  SizeCheck(derived_class.Dimensions.size(), 1);
  SizeCheck(_value_array.size(), derived_class.Dimensions[0].size());
  FOR(i, _value_array.size()) derived_class.Entries[i] = _value_array[i];

  return derived_class;
}

template<typename T, class derived>
constexpr derived&
MultiArray<T, derived>::operator=(const std::initializer_list<std::initializer_list<T>>& _value_matrix) noexcept
{
  derived& derived_class = Derived();

  SizeCheck(derived_class.Dimensions.size(), 2);
  SizeCheck(_value_matrix.size(), derived_class.Dimensions[0].size());

  FOR(i, _value_matrix.size())
  {
    SizeCheck(_value_matrix[i].size(), derived_class.Dimensions[1].size());
    FOR(j, _value_matrix[i].size()) derived_class(i, j) = _value_matrix[i][j];
  }

  return derived_class;
}

/***************************************************************************************************************************************************************
* Static Multi-dimensional Array Class
***************************************************************************************************************************************************************/
template<typename T, size_t... dims>
constexpr StaticMultiArray<T, dims...>::StaticMultiArray()
  : StaticMultiArray(GetStaticInitValue<T>()) {}

template<typename T, size_t ...dims>
constexpr StaticMultiArray<T, dims...>::StaticMultiArray(const T _value)
  : Entries(_value)
{
  STATIC_ASSERT(0 < sizeof...(dims), "A multi-dimensional array must have at least 1 dimension.")
}

/***************************************************************************************************************************************************************
* Dynamic Multi-dimensional Array Class
***************************************************************************************************************************************************************/

/** Constructors/Destructors */
template<typename T>
DynamicMultiArray<T>::DynamicMultiArray()
  : DynamicMultiArray(0) {}

template<typename T>
DynamicMultiArray<T>::DynamicMultiArray(const std::convertible_to<size_t> auto... _dimensions)
  : Dimensions{static_cast<size_t>(_dimensions)...}, nEntries(Product(_dimensions...)), Entries(nEntries, GetDynamicInitValue<T>()) {}

/** Multi-array Resize Functions */
template<typename T>
void DynamicMultiArray<T>::Resize(const std::convertible_to<size_t> auto... _dimensions)
{
  Dimensions = {_dimensions...};
  nEntries = Product(_dimensions...);
  Entries.resize(nEntries, GetDynamicInitValue<T>());
}

}

#endif
