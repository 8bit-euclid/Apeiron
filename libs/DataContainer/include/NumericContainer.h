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
#include "../../../include/Concepts.h"
#include "../../../include/Random.h"

namespace aprn {
namespace detail {

/***************************************************************************************************************************************************************
* Numeric Data Container Class and Arithmetic Operations
***************************************************************************************************************************************************************/
template<Arithmetic T, class derived>
class NumericContainer
{
protected:
  constexpr NumericContainer() {}

public:
  /** Scalar arithmetic operator overloads. */
  constexpr derived operator+(const std::convertible_to<T> auto scalar) const;

  constexpr derived operator-(const std::convertible_to<T> auto scalar) const;

  constexpr derived operator*(const std::convertible_to<T> auto scalar) const;

  constexpr derived operator/(const std::convertible_to<T> auto scalar) const;

  constexpr derived& operator+=(const std::convertible_to<T> auto scalar);

  constexpr derived& operator-=(const std::convertible_to<T> auto scalar);

  constexpr derived& operator*=(const std::convertible_to<T> auto scalar);

  constexpr derived& operator/=(const std::convertible_to<T> auto scalar);

  /** Entry-wise binary arithmetic operator overloads. */
  template<class D>
  constexpr derived operator+(const NumericContainer<T, D>& container) const;

  template<class D>
  constexpr derived operator-(const NumericContainer<T, D>& container) const;

  template<class D>
  constexpr derived operator*(const NumericContainer<T, D>& container) const;

  template<class D>
  constexpr derived operator/(const NumericContainer<T, D>& container) const;

  template<class D>
  constexpr derived& operator+=(const NumericContainer<T, D>& container);

  template<class D>
  constexpr derived& operator-=(const NumericContainer<T, D>& container);

  template<class D>
  constexpr derived& operator*=(const NumericContainer<T, D>& container);

  template<class D>
  constexpr derived& operator/=(const NumericContainer<T, D>& container);

  /** Entry-wise unary operator overloads. */
  constexpr derived operator-() const;

  /** Entry randomisation. */
  void Randomise();

  static void ResetRandomiser(const T min, const T max);

  /** Derived class access. */
  constexpr derived&
  Derived() noexcept { return static_cast<derived&>(*this); }

  constexpr const derived&
  Derived() const noexcept { return static_cast<const derived&>(*this); }

private:
  inline static Random<T> Randomiser = Random<T>();
};

}//detail

/** Stand-alone Operator overloads. */
template<Arithmetic T, class derived>
constexpr derived operator*(const std::convertible_to<T> auto scalar, const detail::NumericContainer<T, derived>& container);

}//aprn

#include "NumericContainer.tpp"
