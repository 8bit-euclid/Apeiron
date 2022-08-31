/***************************************************************************************************************************************************************
* GPL-3.0 License
* Copyright (C) 2022 Niran A. Ilangakoon
*
* This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as
* published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
* of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program.
* If not, see <https://www.gnu.org/licenses/>.
***************************************************************************************************************************************************************/

#pragma once

#include "../../../include/Global.h"
#include "DataContainer/include/Array.h"
#include "Visualiser/include/Texture.h"

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
   template<class type> using UMap = std::unordered_map<IndexT, type>;

   UMap<GlyphBox<Int64>> Boxes;
   SPtr<Texture>         Image;
   Int64                 Width;
   Int64                 Height;
};

}