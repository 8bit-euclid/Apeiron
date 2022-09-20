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

#include "../include/TeXGlyph.h"
#include "../include/ModelFactory.h"
#include "../include/ParseTeX.h"

namespace aprn::vis {

/***************************************************************************************************************************************************************
* Glyph Public Interface
***************************************************************************************************************************************************************/
TeXGlyph::TeXGlyph(const char tex_char)
   : TeXGlyph(std::string(1, tex_char)) {}

TeXGlyph::TeXGlyph(const std::string& tex_str) { Set(tex_str); }

TeXGlyph&
TeXGlyph::Set(const char tex_char) { return Set(std::string(1, tex_char)); }

TeXGlyph&
TeXGlyph::Set(const std::string& tex_str)
{
   DEBUG_ASSERT(isGlyphString(tex_str), "The following string does not yet qualify as a glyph: ", tex_str)
   _Text = tex_str;
   return *this;
}

TeXGlyph&
TeXGlyph::Add(const char tex_char) { return Add(std::string{tex_char}); }

TeXGlyph&
TeXGlyph::Add(const std::string& str)
{
   _Text.append(str);
   return *this;
}

TeXGlyph&
TeXGlyph::Add(std::string&& str)
{
   _Text.append(std::move(str));
   return *this;
}

TeXGlyph&
TeXGlyph::SetColour(const SVectorR4& rgba_colour) { return SetColour(Colour{rgba_colour}); }

TeXGlyph&
TeXGlyph::SetColour(const Colour& colour)
{
   if(!_Colour.has_value()) _Colour = colour;
   return *this;
}

TeXGlyph&
TeXGlyph::SetItalic(const bool is_italic)
{
   if(!_isItalic.has_value()) _isItalic = is_italic;
   return *this;
}

TeXGlyph&
TeXGlyph::SetBold(const bool is_bold)
{
   if(!_isBold.has_value()) _isBold = is_bold;
   return *this;
}

/***************************************************************************************************************************************************************
* Glyph Private Interface
***************************************************************************************************************************************************************/
void
TeXGlyph::Init(GlyphSheet::IndexT& index_offset)
{
   ASSERT(!_isInit, "This glyph has already been initialised.")

   // Initialise subglyphs and assign glyph index.
   if(isCompound()) FOR_EACH(glyph, _SubGlyphs) glyph->Init(index_offset);
   else if(isRendered())
   {
      _isInit = true;
      _Index  = index_offset++;
   }
}

void
TeXGlyph::ComputeDimensions(const GlyphSheet& glyph_sheet, const UChar font_size, const SVectorR3& texbox_anchor, const SVectorR2& texbox_dimensions)
{
   if(!isRendered()) return;

   ASSERT(_isInit, "The glyph must be initialise before its dimensions are computed.")

   const auto& glyph_info = glyph_sheet.GlyphInfo(_Index);
   const auto  scale      = GlyphSheet::FontSizeScale(font_size);
   SVectorR2   dimensions; // Dimensions of the glyph in the xy-plane in world-space coordinates.
   SVectorR2   anchor;     // Bottom-left corner of the glyph in world-space coordinates.

   dimensions.x() = scale * static_cast<Real>(glyph_info.Width);
   dimensions.y() = scale * static_cast<Real>(glyph_info.Height + glyph_info.Depth);
   anchor         = scale * SVectorR2{ glyph_info.Position.x(), glyph_info.Position.y() - glyph_info.Depth }; // Anchor in TeXBox local coordinate system.

   // Set texture coordinates based on the glyph's dimensions w.r.t. the tex-box's dimensions.
   _Mesh = ModelFactory::Rectangle(dimensions.x(), dimensions.y()).Geometry();
   auto set_tex_coor = [&](const size_t i, const SVectorR2& point) { _Mesh.Vertices[i].TextureCoordinates =
                                                                     glm::vec2(point.x() / texbox_dimensions.x(), point.y() / texbox_dimensions.y()); };
   set_tex_coor(0, anchor);
   set_tex_coor(1, anchor + SVectorR2{ dimensions.x(), Zero });
   set_tex_coor(2, anchor + dimensions);
   set_tex_coor(3, anchor + SVectorR2{ Zero, dimensions.y() });

   // Offset the glyph w.r.t. the parent tex-box's anchor. Also need to offset by half the glyph's dimensions as the glyph's rectangle is centred on the origin.
   anchor += Half * SVectorR2{ dimensions.x(), dimensions.y() };
   anchor += SVectorR2{ texbox_anchor.x(), texbox_anchor.y() };
   OffsetPosition(ToVector<3>(anchor));

   // Initialise underlying model.
   Model::Init();
}

}