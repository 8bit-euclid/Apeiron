#pragma once

#include "Manifold.h"

namespace aprn {
namespace mnfld {

/***************************************************************************************************************************************************************
* Curve Alias Template
***************************************************************************************************************************************************************/
template<class derived>
using Surface = Manifold<derived, 2, 3>;

/***************************************************************************************************************************************************************
* Linear/Piecewise Linear Surfaces
***************************************************************************************************************************************************************/
class Plane : public Surface<Plane>
{
public:
  Plane(const SVectorF3& _unit_normal, const SVectorF3& _point = {Zero, Zero, Zero});
};

}
}