#include "../../../include/Global.h"

namespace aprn::mnfld {

template<class derived, size_t manifold_dim, size_t ambient_dim>
constexpr
Manifold<derived, manifold_dim, ambient_dim>::Manifold()
{
   STATIC_ASSERT(ambient_dim > 1, "Only manifolds that can be embedded in an ambient space of dimension 2 or more are supported.")
}

template<class derived, size_t manifold_dim, size_t ambient_dim>
constexpr SVectorF<ambient_dim>
Manifold<derived, manifold_dim, ambient_dim>::Point(const std::convertible_to<Float> auto... _params)
{
   ParamCountCheck(_params...);
   return ComputePoint(SVectorF<manifold_dim>{_params...});
}

template<class derived, size_t manifold_dim, size_t ambient_dim>
constexpr SVectorF<ambient_dim>
Manifold<derived, manifold_dim, ambient_dim>::Tangent(const std::convertible_to<Float> auto... _params)
{
   ParamCountCheck(_params...);
   return ComputeTangent(SVectorF<manifold_dim>{_params...});
}

template<class derived, size_t manifold_dim, size_t ambient_dim>
constexpr SVectorF<ambient_dim>
Manifold<derived, manifold_dim, ambient_dim>::Bitangent(const std::convertible_to<Float> auto... _params)
{
   ParamCountCheck(_params...);
   return ComputeBitangent(SVectorF<manifold_dim>{_params...});
}

template<class derived, size_t manifold_dim, size_t ambient_dim>
constexpr SVectorF<ambient_dim>
Manifold<derived, manifold_dim, ambient_dim>::Normal(const std::convertible_to<Float> auto... _params)
{
   ParamCountCheck(_params...);
   return ComputeNormal(SVectorF<manifold_dim>{_params...});
}

template<class derived, size_t manifold_dim, size_t ambient_dim>
constexpr void
Manifold<derived, manifold_dim, ambient_dim>::ParamCountCheck(const std::convertible_to<Float> auto... _params) const
{
   STATIC_ASSERT(sizeof...(_params) == manifold_dim,
                "Expected ", manifold_dim, " parameter/s from a ", manifold_dim, "-manifold, but got ", sizeof...(_params), ".")
}

}