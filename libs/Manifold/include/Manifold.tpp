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

namespace aprn::mnfld {

template<size_t manifold_dim, size_t ambient_dim>
constexpr
Manifold<manifold_dim, ambient_dim>::Manifold()
{
   STATIC_ASSERT(ambient_dim > 1, "Only manifolds that can be embedded in an ambient space of dimension 2 or more are supported.")
}

template<size_t manifold_dim, size_t ambient_dim>
constexpr SVectorR<ambient_dim>
Manifold<manifold_dim, ambient_dim>::Point(const std::convertible_to<Real> auto... params) const
{
   CheckParamCount(params...);
   return ComputePoint(SVectorR<manifold_dim>{params...});
}

template<size_t manifold_dim, size_t ambient_dim>
constexpr SVectorR<ambient_dim>
Manifold<manifold_dim, ambient_dim>::Tangent(const std::convertible_to<Real> auto... params) const
{
   CheckParamCount(params...);
   return ComputeTangent(SVectorR<manifold_dim>{params...});
}

template<size_t manifold_dim, size_t ambient_dim>
constexpr SVectorR<ambient_dim>
Manifold<manifold_dim, ambient_dim>::Bitangent(const std::convertible_to<Real> auto... params) const
{
   CheckParamCount(params...);
   return ComputeBitangent(SVectorR<manifold_dim>{params...});
}

template<size_t manifold_dim, size_t ambient_dim>
constexpr SVectorR<ambient_dim>
Manifold<manifold_dim, ambient_dim>::Normal(const std::convertible_to<Real> auto... params) const
{
   CheckParamCount(params...);
   return ComputeNormal(SVectorR<manifold_dim>{params...});
}

template<size_t manifold_dim, size_t ambient_dim>
constexpr void
Manifold<manifold_dim, ambient_dim>::CheckParamCount(const std::convertible_to<Real> auto... params) const
{
   STATIC_ASSERT(sizeof...(params) == manifold_dim,
                "Expected ", manifold_dim, " parameter/s for a ", manifold_dim, "-manifold, but got ", sizeof...(params), ".")
}

}