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

#include "../include/ModelFactory.h"
#include "../include/ParseTeX.h"
#include "../include/TeXGlyphGroup.h"

namespace aprn::vis {

/***************************************************************************************************************************************************************
* String Public Interface
***************************************************************************************************************************************************************/
TeXGlyphGroup::TeXGlyphGroup(const char* str)
   : TeXGlyphGroup(std::string(str)) {}

TeXGlyphGroup::TeXGlyphGroup(const std::string& str) { Add(str); }

TeXGlyphGroup::TeXGlyphGroup(const TeXGlyph& glyph) { Add(glyph); }

TeXGlyphGroup::TeXGlyphGroup(const DArray<TeXGlyph>& glyphs) { Add(glyphs); }

TeXGlyphGroup&
TeXGlyphGroup::Add(const std::string& str)
{
   Add(Parse(str));
   return *this;
}

TeXGlyphGroup&
TeXGlyphGroup::Add(const TeXGlyph& glyph)
{
   // Allocate new memory for the glyph, initialise it, and add it as a sub-model of this string.
   const std::string& glyph_id = "Glyph_" + ToString(_SubTeXObjects.size());
   _SubTeXObjects.push_back(std::make_shared<TeXGlyph>(glyph));
   _SubModels.emplace(glyph_id, _SubTeXObjects.back());
   return *this;
}

TeXGlyphGroup&
TeXGlyphGroup::Add(const DArray<TeXGlyph>& glyphs)
{
   FOR_EACH_CONST(glyph, glyphs) Add(glyph);
   return *this;
}

TeXGlyphGroup&
TeXGlyphGroup::SetColour(const SVectorR4& rgba_colour) { return SetColour(Colour{rgba_colour}); }

TeXGlyphGroup&
TeXGlyphGroup::SetColour(const Colour& colour)
{
   FOR_EACH(glyph, _SubTeXObjects) glyph->SetColour(colour);
   return *this;
}

TeXGlyphGroup&
TeXGlyphGroup::SetItalic(bool is_italic)
{
   FOR_EACH(glyph, _SubTeXObjects) glyph->SetItalic(is_italic);
   return *this;
}

TeXGlyphGroup&
TeXGlyphGroup::SetBold(bool is_bold)
{
   FOR_EACH(glyph, _SubTeXObjects) glyph->SetBold(is_bold);
   return *this;
}

/***************************************************************************************************************************************************************
* String Private Interface
***************************************************************************************************************************************************************/
void
TeXGlyphGroup::Init(GlyphSheet::IndexT& index_offset)
{
   // Compute the position, height, and width of this string.
   _Text.clear();
   FOR_EACH(glyph, _SubTeXObjects)
   {
      // Add contribution from the glyph to the TeX string.
      glyph->Init(index_offset);
      _Text += glyph->_Text;
   }
}

DArray<TeXGlyph>
TeXGlyphGroup::Parse(const std::string& str)
{
   auto current = str.begin();
   return ParseTeXString(current, str.end());
}

void
TeXGlyphGroup::ComputeDimensions(const GlyphSheet& glyph_sheet, const UChar font_size, const SVectorR3& texbox_anchor, const SVectorR2& texbox_dimensions)
{
   FOR_EACH(glyph, _SubTeXObjects)
      glyph->ComputeDimensions(glyph_sheet, font_size, texbox_anchor, texbox_dimensions);
}

void
TeXGlyphGroup::LoadSubGlyphTextures(const Pair<std::string, Real>& texture_info) { FOR_EACH(glyph, _SubTeXObjects) glyph->_TextureInfo = texture_info; }

}