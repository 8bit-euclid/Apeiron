#pragma once

#include "../../../include/Global.h"
#include "../../DataContainer/include/Array.h"
#include "../../LinearAlgebra/include/Vector.h"
#include "Categories.h"

namespace aprn::mnfld {

/***************************************************************************************************************************************************************
* Manifold Abstract Base Class
***************************************************************************************************************************************************************/
template<class derived, size_t manifold_dim, size_t ambient_dim = 3>
class Manifold
{
   static_assert(manifold_dim <= ambient_dim, "The dimension of a manifold cannot be larger than that of the ambient space in which it is embedded.");

 protected:
   using Vect  = SVectorF<ambient_dim>;
   using Param = SVectorF<manifold_dim>;

 public:
   constexpr Manifold();

   constexpr Vect
   Point(const std::convertible_to<Float> auto... _params);

   constexpr Vect
   Tangent(const std::convertible_to<Float> auto... _params);

   constexpr Vect
   Bitangent(const std::convertible_to<Float> auto... _params);

   constexpr Vect
   Normal(const std::convertible_to<Float> auto... _params);

 protected:
   virtual constexpr Vect
   ComputePoint(const Param& _params) = 0;

   virtual constexpr Vect
   ComputeTangent(const Param& _params) = 0;

   virtual constexpr Vect
   ComputeBitangent(const Param& _params) = 0;

   virtual constexpr Vect
   ComputeNormal(const Param& _params) = 0;

 private:
   constexpr void
   ParamCountCheck(const std::convertible_to<Float> auto... _params) const;

   /** Derived Class Access */
   constexpr derived&
   Derived() noexcept { return static_cast<derived&>(*this); }

   constexpr const derived&
   Derived() const noexcept { return static_cast<const derived&>(*this); }
};

}

#include "Manifold.tpp"
