/***************************************************************************************************************************************************************
* GPL-3.0 License
* Copyright (C) 2022 Niran A. Ilangakoon
*
* This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as
* published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
* of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program.
* If not, see <https://www.gnu.org/licenses/>.
***************************************************************************************************************************************************************/

#pragma once

#include "../../../include/Global.h"
#include "DataContainer/include/Array.h"
#include "Glyph.h"
#include "GlyphSheet.h"
#include "Model.h"

namespace aprn::vis {

class String final : public Model
{
 public:
   String() = default;

   explicit String(const char* str, const std::string& label = "");

   explicit String(const std::string& str, const std::string& label = "");

   explicit String(const Glyph& glyph, const std::string& label = "");

   explicit String(const DArray<Glyph>& glyphs, const std::string& label = "");

   String& Add(const std::string& str);

   String& Add(const Glyph& glyph);

   String& Add(const DArray<Glyph>& glyphs);

   String& SetLabel(const std::string& label);

   String& SetColour(const Colour& colour);

   String& SetScale(const Float width_scale, const std::optional<Float> height_scale = std::nullopt);

   String& SetItalic(bool is_italic);

   String& SetBold(bool is_bold);

 private:
   friend class TeXBox;

   void Init(UInt16& index_offset);

   void SetAnchor(const SVectorF3* anchor);

   DArray<Glyph> Parse(const std::string& str);

   void LinkGlyphSheet(const GlyphSheet* glyph_sheet);

   std::string         _Label;
   std::string         _Text;
   DArray<SPtr<Glyph>> _Glyphs;
   const SVectorF3*    _Anchor; // Bottom-left corner of the parent TeX-box
   Float               _Width;
   Float               _Height;
};

}