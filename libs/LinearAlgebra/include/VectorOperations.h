#pragma once

#include "../../../include/Global.h"
#include "../../LinearAlgebra/include/Vector.h"

namespace aprn {

/***************************************************************************************************************************************************************
* Cross/Dot/Outer Products
***************************************************************************************************************************************************************/
template<typename T, class D>
constexpr T
InnerProduct(const Vector<T, D>& _v0, const Vector<T, D>& _v1)
{
   const auto& v0 = _v0.Derived();
   const auto& v1 = _v1.Derived();
   return std::inner_product(v0.begin(), v0.end(), v1.begin(), static_cast<T>(Zero));
}

template<typename T, class D>
constexpr SVectorF3
CrossProduct(const Vector<T, D>& _v0, const Vector<T, D>& _v1)
{
   const auto& v0 = _v0.Derived();
   const auto& v1 = _v1.Derived();
   ASSERT(v0.size() == v1.size() && (v0.size() == 2 || v0.size() == 3), "Cross products can only be computed for 2D or 3D vectors.")

   return v0.size() == 2 ? SVectorF3{Zero, Zero, v0[0] * v1[1] - v0[1] * v1[0]} :
                           SVectorF3{v0[1]*v1[2] - v0[2]*v1[1], v0[2]*v1[0] - v0[0]*v1[2], v0[0]*v1[1] - v0[1]*v1[0]};
}

/***************************************************************************************************************************************************************
* Lp-Norms
***************************************************************************************************************************************************************/
template<size_t p, typename T, class D>
requires Arithmetic<T>
constexpr Float
LpNorm(const Vector<T, D>& _v)
{
   STATIC_ASSERT(0 < p && p < 4, "Lp-norms can currently only be computed for p = 1, 2, 3.")

   const auto& vector = _v.Derived();
   switch(p)
   {
     case 1: return Sum(vector.begin(), vector.end());
     case 2: return Sqrt(InnerProduct(vector, vector));
     case 3: throw("TODO");
   }
}

template<typename T, class D>
constexpr T
L1Norm(const Vector<T, D>& _v) { return static_cast<T>(LpNorm<1>(_v)); }

template<typename T, class D>
constexpr Float
L2Norm(const Vector<T, D>& _v) { return LpNorm<2>(_v); }

template<typename T, class D>
constexpr T
LInfNorm(const Vector<T, D>& _v) { return MaxEntry(_v.Derived().begin(), _v.Derived().end()); }

template<typename T, class D>
constexpr Float
Magnitude(const Vector<T, D>& _v) { return L2Norm(_v); }

template<typename T, class D>
constexpr bool
isNormalised(const Vector<T, D>& _v) { return isEqual(Magnitude(_v), One); }

template<typename T, class D>
constexpr D
Normalise(const Vector<T, D>& _v)
{
  const auto magn = Magnitude(_v);
  return !isEqual(magn, Zero) ? _v / magn : throw std::invalid_argument("Cannot normalise a vector of zero magnitude.");
}

/***************************************************************************************************************************************************************
* Vector Angle/Alignment/Rotation
***************************************************************************************************************************************************************/
template<bool orientangle = false, typename T, class D>
constexpr Float
ComputeAngle(const Vector<T, D>& _v0, const Vector<T, D>& _v1, const SVectorF3& _orient = zAxis3)
{
  const auto smallangle = Arccos(InnerProduct(Normalise(_v0), Normalise(_v1)));
  if constexpr(!orientangle) return smallangle;
  else return Sgn(InnerProduct(CrossProduct(_v0, _v1), _orient)) * smallangle;
}

template<typename T, class D>
constexpr bool
isAligned(const Vector<T, D>& _v0, const Vector<T, D>& _v1, const Float angle_thresh = TwelfthPi)
{
  const auto angle = ComputeAngle(_v0, _v1);
  return isBounded(angle_thresh, Zero, HalfPi) ? angle < angle_thresh || angle > (Pi - angle_thresh) :
                                                  throw std::domain_error("Angle threshold is out of bounds.");
}

template<typename T, class D>
constexpr D
RotateAbout(const Vector<T, D>& _vector, const Float& angle, const SVectorF3& axis = zAxis3)
{
  return D{};
}

template<typename T, class D>
constexpr D
RotateTowards(const Vector<T, D>& _vector, const Float& angle, const Vector<T, D>& _reference)
{
  return D{};
}

}
