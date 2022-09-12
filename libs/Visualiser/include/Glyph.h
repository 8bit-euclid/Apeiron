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
#include "LinearAlgebra/include/Vector.h"
#include "Colour.h"
#include "Model.h"
#include "GlyphSheet.h"

#include <optional>

namespace aprn::vis {

class Glyph final : public Model
{
 public:
   Glyph() = default;

   explicit Glyph(const char tex_char);

   explicit Glyph(const std::string& tex_str);

   Glyph& Set(const char tex_char);

   Glyph& Set(const std::string& tex_str);

   Glyph& Set(Glyph&& glyph);

   Glyph& SetFontSize(char font_size);

   Glyph& SetColour(const Colour& colour);

   Glyph& SetScale(const Float width_scale, const std::optional<Float> height_scale = std::nullopt);

   Glyph& SetItalic(const bool is_italic);

   Glyph& SetBold(const bool is_bold);

   void Add(const Glyph& glyph);

   void Add(const DArray<Glyph>& glyphs);

   void Add(const std::string& str);

   void Add(Glyph&& glyph);

   void Add(DArray<Glyph>&& glyphs);

   void Add(std::string&& str);

   inline void DoNotRender() { _Render = false; }

   inline bool isRendered() const { return _Render; }

   inline const std::string& GetText() const { return _Text; }

 private:
   friend class String;

   void Init(UInt16& index_offset);

   void ComputeDimensions();

   inline bool isCompound() const { return !_SubGlyphs.empty(); }

   inline void SetAnchor(const SVectorF3* anchor) { _Anchor = anchor; }

   std::string              _Text;
   GlyphSheet::IndexT       _Index;
   std::optional<char>      _FontSize;
   std::optional<Colour>    _Colour;
   std::optional<bool>      _isItalic;
   std::optional<bool>      _isBold;
   std::optional<SVectorF2> _Dimensions; // [width, height] in world-space coordinates.
   std::optional<SVectorF2> _Scale;      // [width-scale, height-scale]
   SVectorF2                _Position;   // Position (of the LaTeX glyph) in world-space coordinates.
   const SVectorF3*         _Anchor{};   // Bottom-left corner of the parent TeX-box
   const GlyphSheet*        _GlyphSheet{};
   DArray<SPtr<Glyph>>      _SubGlyphs;
   bool                     _Render{true};
   bool                     _isInit{false};

   /** Friend unit tests */
   friend class ParseTeXTest_ParseTeXChar_Test;
   friend class ParseTeXTest_ParseTeXMath_Test;
   friend class ParseTeXTest_ParseTeXGlyph_Test;
   friend class ParseTeXTest_ParseTeXString_Test;
   friend class ParseTeXTest_ParseTeXCommand_Test;
   friend class ParseTeXTest_ParseTeXScriptText_Test;
   friend class ParseTeXTest_ParseAllTeXScriptText_Test;
};

}