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

#include "../include/TeXBox.h"
#include "../include/Texture.h"
#include "FileManager/include/File.h"

namespace aprn::vis {

/***************************************************************************************************************************************************************
* TeXBox Public Interface
***************************************************************************************************************************************************************/
TeXBox::TeXBox(const char* str)
   : TeXBox(std::string(str)) {}

TeXBox::TeXBox(const std::string& str)
   : TeXBox(String(str)) {}

TeXBox::TeXBox(const Glyph& glyph)
   : TeXBox(String(glyph)) {}

TeXBox::TeXBox(const DArray<Glyph>& glyphs)
   : TeXBox(String(glyphs)) {}

TeXBox::TeXBox(const String& str)
   : TeXBox(DArray<String>{str}) {}

TeXBox::TeXBox(const DArray<String>& strings) { Add(strings); }

TeXBox&
TeXBox::Add(const std::string& str) { return Add(String(str)); }

TeXBox&
TeXBox::Add(const Glyph& glyph) { return Add(String(glyph)); }

TeXBox&
TeXBox::Add(const String& str)
{
   // Allocate new memory for the string, initialise it, and add it as a sub-model of this TeX-box.
   const std::string& str_id = "String_" + ToString(_Strings.size());
   _Strings.emplace_back(std::make_shared<String>(str));
   _SubModels.emplace(str_id, _Strings.back());
   return *this;
}

TeXBox&
TeXBox::Add(const DArray<Glyph>& glyphs)
{
   Add(String(glyphs));
   return *this;
}

TeXBox&
TeXBox::Add(const DArray<String>& strings)
{
   FOR_EACH(str, strings) Add(str);
   return *this;
}

TeXBox&
TeXBox::SetPixelDensity(const UInt density)
{
   _GlyphSheet.SetPixelDensity(density);
   return *this;
}

TeXBox&
TeXBox::SetAnchor(const SVectorR3& anchor)
{
   _Anchor = anchor;
   return *this;
}

TeXBox&
TeXBox::SetFontSize(const UChar font_size)
{
   FOR_EACH(str, _Strings) str->SetFontSize(font_size);
   return *this;
}

TeXBox&
TeXBox::SetColour(const Colour& colour)
{
   FOR_EACH(str, _Strings) str->SetColour(colour);
   return *this;
}

TeXBox&
TeXBox::SetItalic(const bool is_italic)
{
   FOR_EACH(str, _Strings) str->SetItalic(is_italic);
   return *this;
}

TeXBox&
TeXBox::SetBold(const bool is_bold)
{
   FOR_EACH(str, _Strings) str->SetBold(is_bold);
   return *this;
}

/***************************************************************************************************************************************************************
* TeXBox Private Interface
***************************************************************************************************************************************************************/
void
TeXBox::Init(const size_t id)
{
   // Initialise sub-strings and accumulate text.
   InitSubGlyphs();

   // Initialise glyph sheet and link to all sub-glyphs.
   _GlyphSheet.Init(id, _Text);
   FOR_EACH(str, _Strings) str->ComputeDimensions(_GlyphSheet, _Anchor);

   ComputeDimensions();
}

void
TeXBox::InitSubGlyphs()
{
   // Initialise sub-glyphs and add contributions from each sub-glyph to the TeX-box string.
   GlyphSheet::IndexT glyph_index{};
   _Text.clear();
   FOR_EACH(str, _Strings)
   {
      str->Init(glyph_index);
      _Text += str->_Text;
   }
}

void
TeXBox::ComputeDimensions()
{
   ASSERT(_GlyphSheet.Width() && _GlyphSheet.Height(), "The dimensions of the glyph sheet must be computed before those of the TeXBox.")

   const auto width  = static_cast<Real>(_GlyphSheet.Width());
   const auto height = static_cast<Real>(_GlyphSheet.Height());

   if(!_Dimensions.has_value())
   {
      // Compute the world-space dimensions by converting glyph sheet dimensions from scaled point dimensions.
      _Dimensions = { width, height };
      _Dimensions.value() *= GlyphSheet::FontSizeScale(_FontSize);

      // If a custom scaling has been prescribed, scale the dimensions.
      if(_Scale.has_value()) _Dimensions.value() *= _Scale.value();
   }
   else if(_Dimensions.value().y() < Zero) // If height dimension has not been prescribed, i.e. must preserve aspect-ratio.
   {
      const auto scale = _Dimensions.value().x() / width;
      _Dimensions.value().y() = scale * height;
   }
}

fm::Path
TeXBox::ImagePath() const
{
   const auto comp_dir = _GlyphSheet.CompileDirectory();
   ASSERT(!comp_dir.empty(), "The compile directory has not yet been set for this TeXBox.")
   return comp_dir / LaTeXTemplate().filename().replace_extension(".png");
}

}