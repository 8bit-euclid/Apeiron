#pragma once

#include "StandardLibrary.h"

/** Run-time macros. */
//#define PARALLEL

/** Debugging macros. */
#define DEBUG_MODE
#define STACK_TRACE_SIZE 10

/** Dimension. */
constexpr int Dim = 2;

#include "Macros.h"
#include "Types.h"
#include "Debug.h"
#include "Loops.h"
#include "Print.h"
#include "Constants.h"
#include "BasicMath.h"
#include "Comparators.h"
#include "Math.h"
#include "Random.h"

///** Initialise an std::array with a single value. */
//template <class t_data_type, std::size_t t_array_size>
//constexpr auto InitStaticArray(const t_data_type& _init_value)
//{
//  std::array<t_data_type, t_array_size> initialised_array;
//  std::fill(initialised_array.begin(), initialised_array.end(), _init_value);
//  return initialised_array;
//}
//
//template <class t_derived_class, class t_data_type>
//class MultiDimArray
//{
//private:
//  constexpr t_derived_class& Derived() noexcept
//  {
//    return static_cast<t_derived_class&>(*this);
//  }
//
//  constexpr const t_derived_class& Derived() const noexcept
//  {
//    return static_cast<const t_derived_class&>(*this);
//  }
//
//protected:
//  constexpr MultiDimArray() {}
//
//public:
//  constexpr ~MultiDimArray() = default;
//
//  /*************************************************************************************************************************************************************
//  * Multi-dimensional Subscript Indexing
//  *************************************************************************************************************************************************************/
////  constexpr std::size_t GetSingleIndex(const std::convertible_to<std::size_t> auto ..._multi_index) const
////  {
////    const auto& dimensions = this->Derived().Dimensions;
////    const auto& n_entries = this->Derived().nEntries;
////    const auto n_dimensions = dimensions.size();
////    const int multi_index[] = {_multi_index...};
////
////    std::size_t index(0), factor(1);
////    FOR(i, n_dimensions)
////    {
////      index += factor * multi_index[i];
////      factor *= dimensions[i];
////    }
////    return index;
////  }
//
//  template<typename ...Is>
//  constexpr std::size_t GetSingleIndex(const Is& ..._multi_index) const
//  {
////    const auto& dimensions = Derived().Dimensions;
////    const auto& n_entries = Derived().nEntries;
////    const auto n_dimensions = dimensions.size();
//    const std::array<int, sizeof...(Is)> multi_index{_multi_index...};
////    const int multi_index[] = {_multi_index...};
//
//    std::size_t index(0), factor(1);
//    FOR(i, Derived().Dimensions.size())
//    {
//      index += factor * static_cast<std::size_t>(multi_index[i]);
//      factor *= Derived().Dimensions[i];
//    }
//    return index;
//  }
//
//  /*************************************************************************************************************************************************************
//  * Multi-dimensional Subscript Operator Overloads
//  *************************************************************************************************************************************************************/
////  constexpr const t_data_type& operator()(const std::convertible_to<std::size_t> auto ..._multi_index) const
////  {
////    return *(Derived().begin() + GetSingleIndex(_multi_index...));
//////    return this->Derived()[GetSingleIndex(_multi_index...)];
////  }
//
//  template<typename ...Is>
//  constexpr const t_data_type& operator()(const Is& ..._multi_index) const
//  {
//    return *(Derived().begin() + GetSingleIndex(_multi_index...));
////    return this->Derived()[GetSingleIndex(_multi_index...)];
//  }
//
////  constexpr t_data_type& operator()(const std::convertible_to<std::size_t> auto ..._multi_index)
////  {
////    return *(Derived().begin() + GetSingleIndex(_multi_index...));
//////    return this->Derived()[GetSingleIndex(_multi_index...)];
////  }
//};
//
///***************************************************************************************************************************************************************
//* Static Multi-dimensional Array Class
//***************************************************************************************************************************************************************/
//template <class t_data_type, std::size_t ...t_dimensions>
//class StaticMultiDimArray : public std::array<t_data_type, (t_dimensions * ...)>,
//                            public MultiDimArray<StaticMultiDimArray<t_data_type, t_dimensions...>, t_data_type>
//{
//  using BaseMultiArray = MultiDimArray<StaticMultiDimArray<t_data_type, t_dimensions...>, t_data_type>;
//  friend BaseMultiArray;
//
//private:
//  constexpr static std::array<std::size_t, sizeof...(t_dimensions)> Dimensions{t_dimensions...};
//  constexpr static std::size_t nEntries = Apeiron::Multiply(Dimensions.begin(), Dimensions.end());
//
//public:
//  /** Default constructor. */
//  constexpr StaticMultiDimArray() : StaticMultiDimArray(Apeiron::GetTypeInitValue<t_data_type>()) {}
//
//  /** Constructor from single initial value. */
//  constexpr StaticMultiDimArray(const t_data_type& _init_value) : std::array<t_data_type, nEntries>(InitStaticArray<t_data_type, nEntries>(_init_value)) {}
//
//  /** Default destructor. */
//  constexpr ~StaticMultiDimArray() = default;
//};
