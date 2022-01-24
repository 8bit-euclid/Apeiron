#pragma once

#include "../../../include/Global.h"
#include "../../DataContainer/include/MultiArray.h"
#include "../../DataContainer/include/Detail.h"

namespace Apeiron {

/***************************************************************************************************************************************************************
* Matrix Abstract Base Class
***************************************************************************************************************************************************************/
template <typename T, class derived>
class Matrix : Detail::NumericContainer<T, Matrix<T, derived>>
{
protected:
  constexpr Matrix() = default;

private:
  /** Derived Class Access */
  constexpr derived& Derived() noexcept { return static_cast<derived&>(*this); }

  constexpr const derived& Derived() const noexcept { return static_cast<const derived&>(*this); }
};

/***************************************************************************************************************************************************************
* Static Matrix Class
***************************************************************************************************************************************************************/
template <typename T, std::size_t M, std::size_t N>
class StaticMatrix : public StaticMultiArray<T, M, N>,
                     public Matrix<T, StaticMatrix<T, M, N>>
{
  using BaseMultiArray = StaticMultiArray<T, M, N>;

public:
  /** Constructors */
  constexpr StaticMatrix()
    : BaseMultiArray() {}

  explicit constexpr StaticMatrix(const T& _value)
    : BaseMultiArray(_value) {}

  explicit constexpr StaticMatrix(const std::initializer_list<T>& _list)
    : BaseMultiArray(_list) {}

  template <class iter>
  constexpr StaticMatrix(const iter _first, const iter _last)
    : BaseMultiArray(_first, _last) {}

private:
  friend Matrix<T, BaseMultiArray>;
};

/***************************************************************************************************************************************************************
* Dynamic Matrix Class
***************************************************************************************************************************************************************/
template <typename T>
class DynamicMatrix : public DynamicMultiArray<T>,
                      public Matrix<T, DynamicMatrix<T>>
{
  using BaseMultiArray = DynamicMultiArray<T>;

public:
  /** Constructors */
  DynamicMatrix()
    : BaseMultiArray() {}

  explicit DynamicMatrix(const std::size_t _size)
    : BaseMultiArray(_size) {}

  DynamicMatrix(const std::size_t _size, const T& _value)
    : BaseMultiArray(_size, _value) {}

  explicit DynamicMatrix(const std::initializer_list<T>& _list)
    : BaseMultiArray(_list) {}

  template <class iter>
  DynamicMatrix(const iter _first, const iter _last)
    : BaseMultiArray(_first, _last) {}

private:
  friend Matrix<T, BaseMultiArray>;
};

}
