#pragma once

#include "../../../include/Global.h"
#include <DataContainers/include/Array.h>

namespace Apeiron{

//template <std::size_t, typename t_data_type>
//using GetDataType = t_data_type;
//
//template <std::size_t ... t_dimensions>
//std::size_t ComputeIndex(std::index_sequence<t_dimensions...>, GetDataType<t_dimensions, std::size_t> ..._multi_index)
//{
//  constexpr std::size_t dimensions[] = {t_dimensions...};
//  std::size_t multi_index[] = {_multi_index...};
//
//  std::size_t index(0);
//  std::size_t factor(1);
//  FOR(i, sizeof...(t_dimensions))
//  {
//    DEBUG_ASSERT(multi_index[i] < dimensions[i], "Multi index component ", multi_index[i]," must be lesser than ", dimensions[i],".")
//    index += factor * multi_index[i];
//    factor *= dimensions[i];
//  }
//
//  DEBUG_ASSERT(isBounded(index, 0, Multiply(t_dimensions...)), "The computed index ", index, " should be in the range [0, ", Multiply(t_dimensions...), "].")
//  return index;
//}
//
///** Tensor base class. */
//template <class t_derived_class>
//class TensorBase
//{
//  private:
//  const t_derived_class& Derived;
//
//  public:
//  /** Default constructor. */
//  TensorBase() : Derived(static_cast<t_derived_class&>(*this)) {}
//
//  /** Pure virtual desctructor. */
//  virtual ~TensorBase() = 0;
//
//  template <std::size_t... t_multi_index>
//  auto& operator()(std::index_sequence<t_multi_index...>)
//  {
//    STATIC_ASSERT(sizeof...(t_multi_index) == Derived.Dimensions.size(), "The number of indices must equal the rank of the tensor.")
//    return Derived.Entries[ComputeIndex(t_multi_index...)];
//  }
//
//  const auto& operator()(GetDataType<Derived.Dimensions, std::size_t>... _multi_index) const
//  {
//    STATIC_ASSERT(sizeof...(_multi_index) == Derived.Dimensions.size(), "The number of indices must equal the rank of the tensor.")
//    return Derived.Entries[ComputeIndex(_multi_index...)];
//  }
//};
//
//template <class t_data_type, std::size_t ...t_dimensions>
//class StaticTensor : public TensorBase<StaticTensor<t_data_type, t_dimensions...>>
//{
//  using DataType = t_data_type;
//  using Dimensions = std::index_sequence<t_dimensions...>;
//  using Base = TensorBase<StaticTensor<t_data_type, t_dimensions...>>;
//  friend Base;
//
//  private:
//  std::pair<std::size_t, std::size_t> Type;
//  StaticArray<t_data_type, Multiply(t_dimensions...)> Entries;
//
//  public:
//  StaticTensor()
//  {
//    STATIC_ASSERT(isNumber<t_data_type>, "Tensors can only be constructed with real/integral numbers.")
//    STATIC_ASSERT(0 < sizeof...(t_dimensions), "Only tensors of rank 1 or higher are supported.")
//  }
//};
//
//template <class t_data_type>
//class DynamicTensor
//{
//  private:
//  std::pair<unsigned, unsigned> Type;
//  DynamicArray<t_data_type> Entries;
//
//  public:
//  DynamicTensor()
//  {
//    STATIC_ASSERT(isNumber<t_data_type>, "Tensors can only be constructed with real/integral numbers.")
////    STATIC_ASSERT(0 < sizeof...(t_dimensions), "Only tensors of rank 1 or higher are supported.")
//  }
//
//  template <typename ...t_dimensions>
//  DynamicTensor(t_dimensions ...dims)
//  {
//    Entries.resize(Multiply(dims...));
//  }
//
//  template <typename ...t_dimensions>
//  void Resize(t_dimensions ...dims)
//  {
//    Entries.resize(Multiply(dims...));
//  }
//
//  void DynamicMethod()
//  {
//    // Some code that operates on Entries.
//  };
//};
//
//
//StaticTensor<Float, 3> test;
//
//template <class t_data_type, unsigned t_dimension>
//class StaticVector : public StaticTensor<t_data_type, 1, t_dimension> {};
//
//template <class t_data_type, unsigned t_dimension0, unsigned t_dimension1>
//class StaticMatrix : public StaticTensor<t_data_type, 2, t_dimension0, t_dimension1> {};
//
//typedef StaticTensor<Float, 1, 1> Vector_StFl;

}
