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

#include "../include/Glyph.h"
#include "../include/ModelFactory.h"
#include "../include/ParseTeX.h"

namespace aprn::vis {

/***************************************************************************************************************************************************************
* Glyph Public Interface
***************************************************************************************************************************************************************/
Glyph::Glyph(const char tex_char)
   : Glyph(std::string(1, tex_char)) {}

Glyph::Glyph(const std::string& tex_str) { Set(tex_str); }

Glyph&
Glyph::Set(const char tex_char) { return Set(std::string(1, tex_char)); }

Glyph&
Glyph::Set(const std::string& tex_str)
{
   DEBUG_ASSERT(isGlyphString(tex_str), "The following string does not yet qualify as a glyph: ", tex_str)
   _Text = tex_str;
   return *this;
}

Glyph&
Glyph::Set(Glyph&& glyph) { return *this = std::move(glyph); }

Glyph&
Glyph::SetColour(const Colour& colour)
{
   if(!_Colour.has_value()) _Colour = colour;
   return *this;
}

Glyph&
Glyph::SetItalic(const bool is_italic)
{
   if(!_isItalic.has_value()) _isItalic = is_italic;
   return *this;
}

Glyph&
Glyph::SetBold(const bool is_bold)
{
   if(!_isBold.has_value()) _isBold = is_bold;
   return *this;
}

void
Glyph::Add(const Glyph& glyph) { _SubGlyphs.emplace_back(std::make_shared<Glyph>(glyph)); }

void
Glyph::Add(const DArray<Glyph>& glyphs) { FOR_EACH_CONST(glyph, glyphs) Add(glyph); }

void
Glyph::Add(const std::string& str) { _Text.append(str); }

void
Glyph::Add(Glyph&& glyph) { _SubGlyphs.emplace_back(std::make_shared<Glyph>(std::move(glyph))); }

void
Glyph::Add(DArray<Glyph>&& glyphs) { FOR_EACH(glyph, glyphs) Add(std::move(glyph)); }

void
Glyph::Add(std::string&& str) { _Text.append(std::move(str)); }

void
Glyph::DoNotRender() { !isCompound() ? _Render = false : throw std::logic_error("Attempting to set a compound glyph to no-render."); }

/***************************************************************************************************************************************************************
* Glyph Private Interface
***************************************************************************************************************************************************************/
void
Glyph::Init(GlyphSheet::IndexT& index_offset)
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
Glyph::ComputeDimensions(const GlyphSheet& glyph_sheet, const UChar font_size, const SVectorR3& anchor)
{
   if(!isRendered()) return;

   ASSERT(_isInit, "The glyph must be initialise before its dimensions are computed.")

   const auto& glyph_info = glyph_sheet.GlyphInfo(_Index);
   const auto  scale      = GlyphSheet::FontSizeScale(font_size);

   _Dimensions.x() = scale * static_cast<Real>(glyph_info.Width);
   _Dimensions.y() = scale * static_cast<Real>(glyph_info.Height + glyph_info.Depth);
   _Position       = scale * SVectorR2{ glyph_info.Position.x(), glyph_info.Position.y() };
   _Position      += SVectorR2{ anchor.x(), anchor.y() }; // Offset glyph position with respect to the parent TeXBox anchor.

//   // Set model mesh and initialise model.
//   _Mesh = ModelFactory::Rectangle(_Dimensions->x(), _Dimensions->y()).Geometry();
//   Model::Init();
}

}