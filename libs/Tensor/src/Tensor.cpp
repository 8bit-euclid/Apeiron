#ifndef TENSOR_TEMPLATE_DEF
#define TENSOR_TEMPLATE_DEF

#include "../include/Tensor.h"

namespace aprn{

/***************************************************************************************************************************************************************
* Tensor Abstract Base Class
***************************************************************************************************************************************************************/
template<typename T, class derived>
constexpr Tensor<T, derived>::Tensor()
{

}

template<typename T, class derived>
constexpr T&
Tensor<T, derived>::operator()(std::convertible_to<size_t> auto... _multi_index)
{
  return Derived().Entries(_multi_index...);
}

template<typename T, class derived>
constexpr const T&
Tensor<T, derived>::operator()(const std::convertible_to<size_t> auto... _multi_index) const
{
  return Derived().Entries(_multi_index...);
}

template<typename T, class derived>
constexpr derived&
Tensor<T, derived>::operator=(const std::initializer_list<T>& _value_array) noexcept
{
  Derived().Entries = _value_array;
}

template<typename T, class derived>
constexpr derived&
Tensor<T, derived>::operator=(const std::initializer_list<std::initializer_list<T>>& _value_matrix) noexcept
{
  Derived().Entries = _value_matrix;
}

/***************************************************************************************************************************************************************
* Static Tensor Class
***************************************************************************************************************************************************************/
template<typename T, size_t... dims>
StaticTensor<T, dims...>::StaticTensor()
{
  STATIC_ASSERT(0 < sizeof...(dims), "A tensor must have at least 1 dimension.")
}

/***************************************************************************************************************************************************************
* Dynamic Tensor Class
***************************************************************************************************************************************************************/
template<typename T>
DynamicTensor<T>::DynamicTensor()
  : Entries() {}

template<typename T>
DynamicTensor<T>::DynamicTensor(const std::convertible_to<size_t> auto... _dimensions)
  : Entries(_dimensions...) {}

}

#endif