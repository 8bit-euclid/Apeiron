#include "../include/Explicit.h"

namespace Apeiron {

constexpr StaticArray<Float, 2> Circle(const Float _radius, const Float _theta)
{
  return Ellipse({_radius, _radius}, _theta);
}

constexpr StaticArray<Float, 2> Ellipse(const StaticArray<Float, 2>& _radii, const Float _theta)
{
  return { _radii[0]*Cos(_theta), _radii[1]*Sin(_theta) };
}

constexpr StaticArray<Float, 3> Sphere(const Float _radius, const Float _theta, const Float _phi)
{
  return Ellipsoid({_radius, _radius, _radius}, _theta, _phi);
}

constexpr StaticArray<Float, 3> Ellipsoid(const StaticArray<Float, 3>& _radii, const Float _theta, const Float _phi)
{
  return { _radii[0]*Cos(_theta)*Sin(_phi), _radii[1]*Sin(_theta)*Sin(_phi), _radii[2]*Cos(_phi) };
}

}