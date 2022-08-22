/***************************************************************************************************************************************************************
* GPL-3.0 License
* Copyright (C) 2022 Niran A. Ilangakoon
*
* This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as
* published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
* of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program.
* If not, see <https://www.gnu.org/licenses/>.
***************************************************************************************************************************************************************/

#pragma once

#include "../../../include/Global.h"
#include "../../DataContainer/include/MultiArray.h"
#include "../../DataContainer/include/NumericContainer.h"

namespace aprn {

/***************************************************************************************************************************************************************
* Matrix Abstract Base Class
***************************************************************************************************************************************************************/
template<typename T, class derived>
class Matrix : detail::NumericContainer<T, Matrix<T, derived>>
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
template<typename T, size_t M, size_t N>
class StaticMatrix final : public StaticMultiArray<T, M, N>,
                           public Matrix<T, StaticMatrix<T, M, N>>
{
  using BaseMultiArray = StaticMultiArray<T, M, N>;

public:
  /** Constructors */
  constexpr StaticMatrix()
    : BaseMultiArray() {}

  explicit constexpr StaticMatrix(const T& value)
    : BaseMultiArray(value) {}

  explicit constexpr StaticMatrix(const std::initializer_list<T>& list)
    : BaseMultiArray(list) {}

  template<class iter>
  constexpr StaticMatrix(const iter first, const iter last)
    : BaseMultiArray(first, last) {}

private:
  friend Matrix<T, BaseMultiArray>;
};

/***************************************************************************************************************************************************************
* Dynamic Matrix Class
***************************************************************************************************************************************************************/
template<typename T>
class DynamicMatrix final : public DynamicMultiArray<T>,
                            public Matrix<T, DynamicMatrix<T>>
{
  using BaseMultiArray = DynamicMultiArray<T>;

public:
  /** Constructors */
  DynamicMatrix()
    : BaseMultiArray() {}

  explicit DynamicMatrix(const size_t size)
    : BaseMultiArray(size) {}

  DynamicMatrix(const size_t size, const T& value)
    : BaseMultiArray(size, value) {}

  explicit DynamicMatrix(const std::initializer_list<T>& list)
    : BaseMultiArray(list) {}

  template<class iter>
  DynamicMatrix(const iter first, const iter last)
    : BaseMultiArray(first, last) {}

private:
  friend Matrix<T, BaseMultiArray>;
};

}
