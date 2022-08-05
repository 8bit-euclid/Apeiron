#pragma once

#include "../../../include/Global.h"
#include "DataContainer/include/Array.h"
#include "Visualiser/include/Texture.h"

namespace aprn::vis {

/***************************************************************************************************************************************************************
* Font Enumerators
***************************************************************************************************************************************************************/
enum class Font
{
   ComputerModern,
   LatinModern,
   TeXGyre
};

/***************************************************************************************************************************************************************
* Glyph Box Class
***************************************************************************************************************************************************************/
template<typename T = int>
struct GlyphBox
{
   T           Width;
   T           Height;
   T           Depth;
   SVector2<T> Position;
   Float       SkewAngle;
};

/***************************************************************************************************************************************************************
* Glyph Sheet Class
***************************************************************************************************************************************************************/
class GlyphSheet
{
 public:
   typedef unsigned IndexType;
   constexpr static Float ScaleFactor{One};

   GlyphSheet(Font font_name, IndexType glyph_index);

   void Generate();

 private:
   template<class type> using Map = std::map<IndexType, type>;
   Map<GlyphBox<int>> _Boxes;
   Texture*           _Texture;
   int                _Width;
   int                _Height;
};

}