#pragma once

#include "../../../include/Global.h"
#include "LinearAlgebra/include/Vector.h"
#include "Colour.h"
#include "Model.h"

namespace aprn::vis {

class GlyphSheet;

class GlyphBox
{
 public:

 private:
   friend class Glyph;

   SVector2<int> Position;
   int           Width;
   int           Height;
   int           Depth;
};

class Glyph : public Model
{
 public:
   Glyph() = default;

 private:
   std::string       Font;
   unsigned int      Index;
   ColourSpec        Colour;
   GlyphBox          Box;
};

}