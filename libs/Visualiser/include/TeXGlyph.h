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
#include "TeXObject.h"

#include <optional>

namespace aprn::vis {

class TeXGlyph final : public TeXObject,
                       public Model
{
 public:
   TeXGlyph() = default;

   explicit TeXGlyph(const char tex_char);

   explicit TeXGlyph(const std::string& tex_str);

   TeXGlyph& Set(const char tex_char);

   TeXGlyph& Set(const std::string& tex_str);

   TeXGlyph& Set(TeXGlyph&& glyph);

   TeXGlyph& SetColour(const Colour& colour);

   TeXGlyph& SetItalic(const bool is_italic);

   TeXGlyph& SetBold(const bool is_bold);

   void Add(const TeXGlyph& glyph);

   void Add(const DArray<TeXGlyph>& glyphs);

   void Add(const std::string& str);

   void Add(TeXGlyph&& glyph);

   void Add(DArray<TeXGlyph>&& glyphs);

   void Add(std::string&& str);

   void DoNotRender();

   inline bool isCompound() const { return !_SubGlyphs.empty(); }

   inline bool isRendered() const { return _Render && !isCompound(); }

   inline const std::string& Text() const { return _Text; }

 private:
   friend class String;

   void Init(GlyphSheet::IndexT& index_offset);

   void ComputeDimensions(const GlyphSheet& glyph_sheet, const UChar font_size, const SVectorR3& texbox_anchor, const SVectorR2& texbox_dimensions);

   std::string            _Text;
   GlyphSheet::IndexT     _Index{MaxInt<GlyphSheet::IndexT>};
   std::optional<Colour>  _Colour;
   std::optional<bool>    _isItalic;
   std::optional<bool>    _isBold;
   DArray<SPtr<TeXGlyph>> _SubGlyphs;
   bool                   _Render{true};
   bool                   _isInit{false};

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