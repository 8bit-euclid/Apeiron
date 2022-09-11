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
#include "FileManager/include/FileSystem.h"
#include "Visualiser/include/Texture.h"

#include <unordered_map>

namespace aprn::vis {

namespace fm = flmgr;

/***************************************************************************************************************************************************************
* Glyph Box Struct
***************************************************************************************************************************************************************/
struct GlyphBox
{
   wchar_t         Char{};
   Int64           Width{};
   Int64           Height{};
   Int64           Depth{};
   SVector2<Int64> Position{};
};

/***************************************************************************************************************************************************************
* Glyph Sheet Class
***************************************************************************************************************************************************************/
class GlyphSheet
{
 public:
   void Init(size_t id, const std::string& text);

   void CompileLaTeXSource(const std::string& text);

   void CreateGlyphSheetImage();

   void ReadGlyphBoxPositions();

   void ReadGlyphBoxAttributes();

   void ComputeDimensions();

   inline auto CompileDirectory() const { return _CompileDirectory; }

   inline auto Width() const { return _Width; }

   inline auto Height() const { return _Height; }

   typedef UInt16 IndexT;
   constexpr static Float FontSize10Scale{0.5 / 655360.0}; // Height of a 10pt font size converted from LaTeX scaled points (1pt = 65536sp) to world-space.

 private:
   friend class TeXBox;

   template<class type> using UMap = std::unordered_map<IndexT, type>;
   UMap<GlyphBox> _Boxes;
   fm::Path       _CompileDirectory;
   fm::Path       _TeXFile;
   Int64          _Width{};
   Int64          _Height{};
   UInt16         _PixelDensity{2000}; // Measured in DPI.
};

/***************************************************************************************************************************************************************
* Stand-alone Functions
***************************************************************************************************************************************************************/
inline fm::Path LaTeXDirectory() { return "./libs/Visualiser/data/latex"; }

inline fm::Path LaTeXTemplate() { return "./libs/Visualiser/resources/latex/texbox.tex"; }

inline fm::Path LuaTeXTemplate() { return "./libs/Visualiser/resources/latex/write_boxes.lua"; }

inline void InitTeXDirectory() { fm::CreateDirectory(LaTeXDirectory(), true); }

}