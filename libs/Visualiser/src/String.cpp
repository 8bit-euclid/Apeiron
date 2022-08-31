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
String::String(const char* str, const std::string& label)
   : String(std::string(str), label) {}

String::String(const std::string& str, const std::string& label)
   : _Label(label) { Add(str); }

String::String(const Glyph& glyph, const std::string& label)
   : _Label(label) { Add(glyph); }

String::String(const DArray<Glyph>& glyphs, const std::string& label)
   : _Label(label) { Add(glyphs); }

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
   const std::string& glyph_id = !glyph._Label.empty() ? glyph._Label : "Glyph_" + ToStr(_Glyphs.size());
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
String::SetLabel(const std::string& label)
{
   _Label = label;
   return *this;
}

String&
String::SetColour(const Colour& colour)
{
   FOR_EACH(glyph, _Glyphs) glyph->SetColour(colour);
   return *this;
}

String&
String::SetScale(const Float width_scale, const std::optional<Float> height_scale)
{
   FOR_EACH(glyph, _Glyphs) glyph->SetScale(width_scale, height_scale);
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
String::Init(UInt16& index_offset)
{
   // Compute the position, height, and width of this string.
   _Text.clear();
   FOR_EACH(glyph, _Glyphs)
   {
      // Add contribution from the glyph to the TeX string.
      glyph->Init(index_offset);
      _Text += glyph->_Text;
   }

   // Embed string into a rectangular model with the same position, height, and width, and initialise model.
   Model::Init();
}

void
String::SetAnchor(const SVectorF3* anchor)
{
   _Anchor = anchor;
   FOR_EACH(glyph, _Glyphs) glyph->SetAnchor(_Anchor);
}

DArray<Glyph>
String::Parse(const std::string& str)
{
   auto current = str.begin();
   return ParseTeXString(current, str.end());
}

void
String::LinkGlyphSheet(const GlyphSheet* glyph_sheet) { FOR_EACH(glyph, _Glyphs) glyph->_GlyphSheet = glyph_sheet; }

}