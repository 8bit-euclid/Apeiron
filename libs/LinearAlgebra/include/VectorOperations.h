#pragma once

#include "../../../include/Global.h"
#include "../../LinearAlgebra/include/Vector.h"

namespace Apeiron {

template<typename T, class D>
constexpr T
InnerProduct(const Vector<T, D>& _vector0, const Vector<T, D>& _vector1)
{
  auto vector0 = _vector0.Derived();
  auto vector1 = _vector1.Derived();
  return std::inner_product(vector0.begin(), vector0.end(), vector1.begin(), static_cast<T>(Zero));
}

template<typename T, class D>
constexpr Vector<T, D>
CrossProduct(const Vector<T, D>& _vector0, const Vector<T, D>& _vector1)
{

  auto vector0 = _vector0.Derived();
  auto vector1 = _vector1.Derived();
  ASSERT(vector0.size() == vector1.size() && (vector0.size() == 2 || vector0.size() == 3), "Cross products can only be computed for 2D or 3D vectors.")

  return std::inner_product(vector0.begin(), vector0.end(), vector1.begin(), static_cast<T>(Zero));
}

template<std::size_t p, typename T, class D>
constexpr T
LpNorm(const Vector<T, D>& _vector)
{
  STATIC_ASSERT(0 < p && p < 4, "Lp-norms can currently only be computed for p = 1, 2, 3.")
  STATIC_ASSERT(isFloatingPoint<T>(), "Lp-norms can only be computed for floating-point types.")

  auto vector = _vector.Derived();
  switch(p)
  {
    case 1: return Sum(vector.begin(), vector.end());
    case 2: return Sqrt(InnerProduct(vector, vector));
    case 3: throw("TODO");
//    case 3: return Cbrt(InnerProduct(vector, vector));
  }
}

template<typename T, class D>
constexpr T
L1Norm(const Vector<T, D>& _vector) { return LpNorm<1>(_vector); }

template<typename T, class D>
constexpr T
L2Norm(const Vector<T, D>& _vector) { return LpNorm<2>(_vector); }

template<typename T, class D>
constexpr T
LInfNorm(const Vector<T, D>& _vector) { return LpNorm<2>(_vector); }

template<typename T, class D>
constexpr Vector<T, D>
Normalise(const Vector<T, D>& _vector)
{
  DEBUG_ASSERT(!isEqual(L2Norm(_vector), static_cast<T>(Zero)), "Cannot normalise a vector of zero magnitude.")
  return _vector / L2Norm(_vector);
}

}
