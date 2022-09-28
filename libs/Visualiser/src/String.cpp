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

#include "../include/ParseTeX.h"
#include "../include/String.h"

namespace aprn::vis {

/***************************************************************************************************************************************************************
* String Public Interface
***************************************************************************************************************************************************************/
String::String(const char* str)
   : String(std::string(str)) {}

String::String(const std::string& str) { Add(str); }

String::String(const TeXGlyph& glyph) { Add(glyph); }

String::String(const DArray<TeXGlyph>& glyphs) { Add(glyphs); }

String&
String::Add(const std::string& str)
{
   Add(Parse(str));
   return *this;
}

String&
String::Add(const TeXGlyph& glyph)
{
   // Allocate new memory for the glyph, initialise it, and add it as a sub-model of this string.
   const std::string& glyph_id = "Glyph_" + ToString(SubTeXObjects_.size());
   SubTeXObjects_.push_back(std::make_shared<TeXGlyph>(glyph));
   SubModels_.emplace(glyph_id, SubTeXObjects_.back());
   return *this;
}

String&
String::Add(const DArray<TeXGlyph>& glyphs)
{
   FOR_EACH_CONST(glyph, glyphs) Add(glyph);
   return *this;
}

String&
String::SetColour(const SVectorR4& rgba_colour) { return SetColour(Colour{rgba_colour}); }

String&
String::SetColour(const Colour& colour)
{
   FOR_EACH(glyph, SubTeXObjects_) glyph->SetColour(colour);
   return *this;
}

String&
String::SetItalic(bool is_italic)
{
   FOR_EACH(glyph, SubTeXObjects_) glyph->SetItalic(is_italic);
   return *this;
}

String&
String::SetBold(bool is_bold)
{
   FOR_EACH(glyph, SubTeXObjects_) glyph->SetBold(is_bold);
   return *this;
}

/***************************************************************************************************************************************************************
* String Private Interface
***************************************************************************************************************************************************************/
void
String::Init(GlyphSheet::IndexT& index_offset)
{
   // Compute the position, height, and width of this string.
   Text_.clear();
   FOR_EACH(glyph, SubTeXObjects_)
   {
      // Add contribution from the glyph to the TeX string.
      glyph->Init(index_offset);
      Text_ += glyph->Text_;
   }
}

DArray<TeXGlyph>
String::Parse(const std::string& str)
{
   auto current = str.begin();
   return ParseTeXString(current, str.end());
}

void
String::ComputeDimensions(const GlyphSheet& glyph_sheet, const UChar font_size, const SVectorR3& texbox_anchor, const SVectorR2& texbox_dimensions)
{
   FOR_EACH(glyph, SubTeXObjects_)
      glyph->ComputeDimensions(glyph_sheet, font_size, texbox_anchor, texbox_dimensions);
}

void
String::LoadSubGlyphTextures(const Pair<std::string, Real>& texture_info) { FOR_EACH(glyph, SubTeXObjects_) glyph->_TextureInfo = texture_info; }

}