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
#include "../include/String.h"

namespace aprn::vis {

/***************************************************************************************************************************************************************
* String Public Interface
***************************************************************************************************************************************************************/
String::String(const char* str)
   : String(std::string(str)) {}

String::String(const std::string& str) { Add(str); }

String::String(const Glyph& glyph) { Add(glyph); }

String::String(const DArray<Glyph>& glyphs) { Add(glyphs); }

String&
String::Add(const std::string& str)
{
   Add(Parse(str));
   return *this;
}

String&
String::Add(const Glyph& glyph)
{
   // Allocate new memory for the glyph, initialise it, and add it as a sub-model of this string.
   const std::string& glyph_id = "Glyph_" + ToString(_Glyphs.size());
   _Glyphs.push_back(std::make_shared<Glyph>(glyph));
   _SubModels.emplace(glyph_id, _Glyphs.back());
   return *this;
}

String&
String::Add(const DArray<Glyph>& glyphs)
{
   FOR_EACH_CONST(glyph, glyphs) Add(glyph);
   return *this;
}

String&
String::SetColour(const Colour& colour)
{
   FOR_EACH(glyph, _Glyphs) glyph->SetColour(colour);
   return *this;
}

String&
String::SetItalic(bool is_italic)
{
   FOR_EACH(glyph, _Glyphs) glyph->SetItalic(is_italic);
   return *this;
}

String&
String::SetBold(bool is_bold)
{
   FOR_EACH(glyph, _Glyphs) glyph->SetBold(is_bold);
   return *this;
}

/***************************************************************************************************************************************************************
* String Private Interface
***************************************************************************************************************************************************************/
void
String::Init(GlyphSheet::IndexT& index_offset)
{
   // Compute the position, height, and width of this string.
   _Text.clear();
   FOR_EACH(glyph, _Glyphs)
   {
      // Add contribution from the glyph to the TeX string.
      glyph->Init(index_offset);
      _Text += glyph->_Text;
   }
}

DArray<Glyph>
String::Parse(const std::string& str)
{
   auto current = str.begin();
   return ParseTeXString(current, str.end());
}

void
String::ComputeDimensions(const GlyphSheet& glyph_sheet, const UChar font_size, const SVectorR3& anchor)
{
   FOR_EACH(glyph, _Glyphs)
      glyph->ComputeDimensions(glyph_sheet, font_size, anchor);
}

}