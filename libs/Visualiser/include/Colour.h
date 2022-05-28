#pragma once

#include "../../../include/Global.h"
#include "../../LinearAlgebra/include/Vector.h"

namespace aprn::vis {

enum class ColourType
{
   RGB,
   HSV,
   HSL,
   CMYK
};

struct Colour
{
   SVectorF4 Values;
   ColourType Type{ColourType::RGB};
};

}
