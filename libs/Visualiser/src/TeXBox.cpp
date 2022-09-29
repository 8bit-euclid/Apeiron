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
#include "../include/TeXBox.h"
#include "../include/Texture.h"
#include "FileManager/include/File.h"

namespace aprn::vis {

/***************************************************************************************************************************************************************
* TeXBox Public Interface
***************************************************************************************************************************************************************/
TeXBox::TeXBox(const char* str) { Add(str); }

TeXBox::TeXBox(const std::string& str) { Add(str); }

TeXBox::TeXBox(const TeXGlyph& tex_glyph) { Add(tex_glyph); }

TeXBox::TeXBox(const TeXBox& tex_box) { Add(tex_box); }

TeXBox::TeXBox(const DArray<TeXGlyph>& tex_glyphs) { Add(tex_glyphs); }

TeXBox::TeXBox(const DArray<TeXBox>& tex_boxes) { Add(tex_boxes); }

TeXBox&
TeXBox::Add(const char* str) { return Add(std::string(str)); }

TeXBox&
TeXBox::Add(const std::string& str) { return Add(ParseTeXString(str)); }

TeXBox&
TeXBox::Add(const TeXGlyph& tex_glyph)
{
   const std::string& glyph_id = "SubBox_" + ToString(SubBoxes_.size());
   auto glyph = std::make_shared<TeXGlyph>(tex_glyph);
   SubBoxes_.emplace_back(glyph);
   SubModels_.emplace(glyph_id, glyph);
   return *this;
}

TeXBox&
TeXBox::Add(const TeXBox& tex_box)
{
   const std::string& box_id = "SubBox_" + ToString(SubBoxes_.size());
   auto box = std::make_shared<TeXBox>(tex_box);
   SubBoxes_.emplace_back(box);
   SubModels_.emplace(box_id, box);
   return *this;
}

TeXBox&
TeXBox::Add(const DArray<TeXGlyph>& tex_glyphs)
{
   FOR_EACH(tex_glyph, tex_glyphs) Add(tex_glyph);
   return *this;
}

TeXBox&
TeXBox::Add(const DArray<TeXBox>& tex_boxes)
{
   FOR_EACH(tex_box, tex_boxes) Add(tex_box);
   return *this;
}

TeXBox&
TeXBox::SetPixelDensity(const UInt density)
{
   GlyphSheet_.SetPixelDensity(density);
   return *this;
}

TeXBox&
TeXBox::SetAnchor(const SVectorR3& anchor)
{
   Anchor_ = anchor;
   return *this;
}

TeXBox&
TeXBox::SetFontSize(const UChar font_size)
{
   FontSize_ = font_size;
   return *this;
}

TeXBox&
TeXBox::SetColour(const SVectorR4& rgba_colour) { return SetColour(Colour{rgba_colour}); }

TeXBox&
TeXBox::SetColour(const Colour& colour)
{
   FOR_EACH(str, SubBoxes_) str->SetColour(colour);
   return *this;
}

TeXBox&
TeXBox::SetItalic(const bool is_italic)
{
   FOR_EACH(str, SubBoxes_) str->SetItalic(is_italic);
   return *this;
}

TeXBox&
TeXBox::SetBold(const bool is_bold)
{
   FOR_EACH(str, SubBoxes_) str->SetBold(is_bold);
   return *this;
}

/***************************************************************************************************************************************************************
* TeXBox Private Interface
***************************************************************************************************************************************************************/
void
TeXBox::InitTeXBox(const size_t id)
{
   // Initialise sub-boxes, assign glyph indices, and accumulate text.
   GlyphSheet::IndexT glyph_index = 0;
   InitTeXObject(glyph_index);

   // Initialise glyph sheet.
   GlyphSheet_.Init(id, Text_);
   ASSERT(GlyphSheet_.Width() && GlyphSheet_.Height(), "The dimensions of the glyph sheet must be computed before those of the TeXBox.")

   // Compute the world-space dimensions by converting glyph sheet dimensions from scaled point dimensions.
   Dimensions_ = { GlyphSheet_.Width(), GlyphSheet_.Height() };
   Dimensions_ *= GlyphSheet::FontSizeScale(FontSize_);

   // Compute the sub-glyph dimensions and their texture coordinates.
   ComputeDimensions(GlyphSheet_, FontSize_, Anchor_, Dimensions_);
}

void
TeXBox::InitTeXObject(GlyphSheet::IndexT& index_offset)
{
   // Initialise sub-glyphs and add contributions from each sub-glyph to the TeX-box string.
   Text_.clear();
   FOR_EACH(sub_box, SubBoxes_)
   {
      sub_box->InitTeXObject(index_offset);
      Text_ += sub_box->Text();
   }
}

void
TeXBox::ComputeDimensions(const GlyphSheet& glyph_sheet, const UChar font_size, const SVectorR3& texbox_anchor, const SVectorR2& texbox_dimensions)
{
   FOR_EACH(sub_box, SubBoxes_) sub_box->ComputeDimensions(glyph_sheet, font_size, texbox_anchor, texbox_dimensions);
}

void
TeXBox::LoadTeXBoxTexture(const Pair<std::string, Real>& texture_info) { FOR_EACH(sub_box, SubBoxes_) sub_box->LoadTeXBoxTexture(texture_info); }

fm::Path
TeXBox::ImagePath() const
{
   const auto compile_dir = GlyphSheet_.CompileDirectory();
   ASSERT(!compile_dir.empty(), "The compile directory has not yet been set for this TeXBox.")
   return compile_dir / LaTeXTemplate().filename().replace_extension(".png");
}

}