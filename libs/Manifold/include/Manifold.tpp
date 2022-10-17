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

template<class D, size_t manifold_dim, size_t ambient_dim>
constexpr
Manifold<D, manifold_dim, ambient_dim>::Manifold()
{
   STATIC_ASSERT(ambient_dim > 1, "Only manifolds that can be embedded in an ambient space of dimension 2 or more are supported.")
}

template<class D, size_t manifold_dim, size_t ambient_dim>
constexpr SVectorR<ambient_dim>
Manifold<D, manifold_dim, ambient_dim>::Point(const std::convertible_to<Real> auto... _params)
{
   ParamCountCheck(_params...);
   return ComputePoint(SVectorR<manifold_dim>{_params...});
}

template<class D, size_t manifold_dim, size_t ambient_dim>
constexpr SVectorR<ambient_dim>
Manifold<D, manifold_dim, ambient_dim>::Tangent(const std::convertible_to<Real> auto... _params)
{
   ParamCountCheck(_params...);
   return ComputeTangent(SVectorR<manifold_dim>{_params...});
}

template<class D, size_t manifold_dim, size_t ambient_dim>
constexpr SVectorR<ambient_dim>
Manifold<D, manifold_dim, ambient_dim>::Bitangent(const std::convertible_to<Real> auto... _params)
{
   ParamCountCheck(_params...);
   return ComputeBitangent(SVectorR<manifold_dim>{_params...});
}

template<class D, size_t manifold_dim, size_t ambient_dim>
constexpr SVectorR<ambient_dim>
Manifold<D, manifold_dim, ambient_dim>::Normal(const std::convertible_to<Real> auto... _params)
{
   ParamCountCheck(_params...);
   return ComputeNormal(SVectorR<manifold_dim>{_params...});
}

template<class D, size_t manifold_dim, size_t ambient_dim>
constexpr void
Manifold<D, manifold_dim, ambient_dim>::ParamCountCheck(const std::convertible_to<Real> auto... _params) const
{
   STATIC_ASSERT(sizeof...(_params) == manifold_dim,
                "Expected ", manifold_dim, " parameter/s from a ", manifold_dim, "-manifold, but got ", sizeof...(_params), ".")
}

}