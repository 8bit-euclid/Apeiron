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
#include "../../DataContainer/include/Array.h"
#include "../../LinearAlgebra/include/Vector.h"
#include "Categories.h"

namespace aprn::mnfld {

template<size_t manifold_dim, size_t ambient_dim = 3>
class Manifold
{
   static_assert(manifold_dim <= ambient_dim, "The dimension of a manifold cannot be larger than that of the ambient space in which it is embedded.");
   using Vector    = SVectorR<ambient_dim>;
   using Parameter = SVectorR<manifold_dim>;

 public:
   constexpr Manifold();

   constexpr Vector Point(const std::convertible_to<Real> auto... params) const;

   constexpr Vector Tangent(const std::convertible_to<Real> auto... params) const;

   constexpr Vector Bitangent(const std::convertible_to<Real> auto... params) const;

   constexpr Vector Normal(const std::convertible_to<Real> auto... params) const;

 protected:
   virtual constexpr Vector ComputePoint(const Parameter& params) = 0;

   virtual constexpr Vector ComputeTangent(const Parameter& params) = 0;

   virtual constexpr Vector ComputeBitangent(const Parameter& params) = 0;

   virtual constexpr Vector ComputeNormal(const Parameter& params) = 0;

 private:
   constexpr void CheckParamCount(const std::convertible_to<Real> auto... params) const;
};

}

#include "Manifold.tpp"
