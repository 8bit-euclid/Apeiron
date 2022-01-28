#pragma once

#include "../../../include/Global.h"
#include "../../DataContainer/include/Array.h"
#include "../../LinearAlgebra/include/Vector.h"
#include "Categories.h"

namespace Apeiron {
namespace Manifold {

/***************************************************************************************************************************************************************
* Manifold Abstract Base Class
***************************************************************************************************************************************************************/
template<class derived, std::size_t manifold_dim, std::size_t ambient_dim = 3>
class Manifold
{
public:
  constexpr Manifold();

  constexpr SVectorF<ambient_dim>
  Point(const std::convertible_to<Float> auto... _params);

  constexpr SVectorF<ambient_dim>
  Tangent(const std::convertible_to<Float> auto... _params);

  constexpr SVectorF<ambient_dim>
  Bitangent(const std::convertible_to<Float> auto... _params);

  constexpr SVectorF<ambient_dim>
  Normal(const std::convertible_to<Float> auto... _params);

protected:
  virtual constexpr SVectorF<ambient_dim>
  ComputePoint(const SVectorF<manifold_dim>& _params) = 0;

  virtual constexpr SVectorF<ambient_dim>
  ComputeTangent(const SVectorF<manifold_dim>& _params) = 0;

  virtual constexpr SVectorF<ambient_dim>
  ComputeBitangent(const SVectorF<manifold_dim>& _params) = 0;

  virtual constexpr SVectorF<ambient_dim>
  ComputeNormal(const SVectorF<manifold_dim>& _params) = 0;

private:
  constexpr void ParamSizeCheck(const std::convertible_to<Float> auto... _params) const;

  /** Derived Class Access */
  constexpr derived&
  Derived() noexcept { return static_cast<derived&>(*this); }

  constexpr const derived&
  Derived() const noexcept { return static_cast<const derived&>(*this); }
};

}
}
