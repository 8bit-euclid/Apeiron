#pragma once

#include "../../../include/Global.h"
#include "LinearAlgebra/include/Vector.h"

namespace aprn::vis {

enum class ColourModel
{
   RGB,
   HSV,
   HSL,
   CMYK
};

struct ColourSpec
{
   SVectorF4   Values{};
   ColourModel Type{ColourModel::RGB};
};

}
