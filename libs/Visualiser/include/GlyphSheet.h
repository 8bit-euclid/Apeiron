#pragma once

#include "../../../include/Global.h"
#include "DataContainer/include/Array.h"
#include "Visualiser/include/Texture.h"

#include <string_view>
#include <unordered_map>

namespace aprn::vis {

template<typename T = Int64>
struct GlyphBox
{
   wchar_t     Char;
   T           Width;
   T           Height;
   T           Depth;
   SVector2<T> Position;
};

struct GlyphSheet
{
   typedef UInt16 IndexT;
   template<class type> using Map = std::unordered_map<IndexT, type>;

   Map<GlyphBox<Int64>> Boxes;
   SPtr<Texture>        Image;
   Int64                Width;
   Int64                Height;
};

}