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

TeXBox::TeXBox(const TeXGlyph& tex_glyph)
   : TeXBox(String(tex_glyph)) {}

TeXBox::TeXBox(const DArray<TeXGlyph>& tex_glyphs)
   : TeXBox(String(tex_glyphs)) {}

TeXBox::TeXBox(const String& tex_boxes)
   : TeXBox(DArray<String>{tex_boxes}) {}

TeXBox::TeXBox(const DArray<String>& strings) { Add(strings); }

TeXBox&
TeXBox::Add(const char* str)
{
   return <#initializer#>;
}

TeXBox&
TeXBox::Add(const std::string& str) { return Add(TeXGlyph(str)); }

TeXBox&
TeXBox::Add(const TeXGlyph& tex_glyph) { return Add(String(tex_glyph)); }

TeXBox&
TeXBox::Add(const TeXBox& tex_box)
{
   // Allocate new memory for the string, initialise it, and add it as a sub-model of this TeX-box.
   const std::string& str_id = "String_" + ToString(_SubBoxes.size());
   _SubBoxes.emplace_back(std::make_shared<String>(str));
   _SubModels.emplace(str_id, _SubBoxes.back());
   return *this;
}

TeXBox&
TeXBox::Add(const DArray<TeXGlyph>& tex_glyphs)
{
   Add(String(tex_glyphs));
   return *this;
}

TeXBox&
TeXBox::Add(const DArray<TeXBox>& tex_boxes)
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
   _FontSize = font_size;
   return *this;
}

TeXBox&
TeXBox::SetColour(const SVectorR4& rgba_colour) { return SetColour(Colour{rgba_colour}); }

TeXBox&
TeXBox::SetColour(const Colour& colour)
{
   FOR_EACH(str, _SubBoxes) str->SetColour(colour);
   return *this;
}

TeXBox&
TeXBox::SetItalic(const bool is_italic)
{
   FOR_EACH(str, _SubBoxes) str->SetItalic(is_italic);
   return *this;
}

TeXBox&
TeXBox::SetBold(const bool is_bold)
{
   FOR_EACH(str, _SubBoxes) str->SetBold(is_bold);
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

   // Initialise glyph sheet and compute tex-box dimensions.
   _GlyphSheet.Init(id, _Text);
   ComputeDimensions();

   // Compute the sub-glyph dimensions and their texture coordinates.
   FOR_EACH(str, _SubBoxes) str->ComputeDimensions(_GlyphSheet, _FontSize, _Anchor, _Dimensions);
}

void
TeXBox::InitSubGlyphs()
{
   // Initialise sub-glyphs and add contributions from each sub-glyph to the TeX-box string.
   GlyphSheet::IndexT glyph_index{};
   _Text.clear();
   FOR_EACH(str, _SubBoxes)
   {
      str->Init(glyph_index);
      _Text += str->_Text;
   }
}

void
TeXBox::ComputeDimensions()
{
   ASSERT(_GlyphSheet.Width() && _GlyphSheet.Height(), "The dimensions of the glyph sheet must be computed before those of the TeXBox.")

   // Compute the world-space dimensions by converting glyph sheet dimensions from scaled point dimensions.
   _Dimensions = { _GlyphSheet.Width(), _GlyphSheet.Height() };
   _Dimensions *= GlyphSheet::FontSizeScale(_FontSize);
}

void
TeXBox::LoadSubGlyphTextures(const Pair<std::string, Real>& texture_info) { FOR_EACH(str, _SubBoxes) str->LoadSubGlyphTextures(texture_info); }

fm::Path
TeXBox::ImagePath() const
{
   const auto comp_dir = _GlyphSheet.CompileDirectory();
   ASSERT(!comp_dir.empty(), "The compile directory has not yet been set for this TeXBox.")
   return comp_dir / LaTeXTemplate().filename().replace_extension(".png");
}

}