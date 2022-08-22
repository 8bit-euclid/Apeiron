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
#include "../include/ParseTeX.h"

namespace aprn::vis {

/***************************************************************************************************************************************************************
* Glyph Public Interface
***************************************************************************************************************************************************************/
Glyph::Glyph(const char tex_char, const std::string& label)
   : Glyph(std::string(1, tex_char), label) {}

Glyph::Glyph(const std::string& tex_str, const std::string& label)
   : _Label(label) { Set(tex_str); }

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
Glyph::SetLabel(const std::string& label)
{
   _Label = label;
   return *this;
}

Glyph&
Glyph::SetColour(const Colour& colour)
{
   if(!_Colour.isSet()) _Colour.Set(colour);
   return *this;
}

Glyph&
Glyph::SetScale(const Float width_scale, const std::optional<Float> height_scale)
{
   if(!_Scale.isSet()) _Scale.Set({width_scale, height_scale.has_value() ? height_scale.value() : width_scale});
   return *this;
}

Glyph&
Glyph::SetItalic(const bool is_italic)
{
   if(!_isItalic.isSet()) _isItalic.Set(is_italic);
   return *this;
}

Glyph&
Glyph::SetBold(const bool is_bold)
{
   if(!_isBold.isSet()) _isBold.Set(is_bold);
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

/***************************************************************************************************************************************************************
* Glyph Private Interface
***************************************************************************************************************************************************************/
void
Glyph::Init(UInt16& index_offset)
{
   // Initialise subglyphs and compute character count.
   if(isCompound()) FOR_EACH(glyph, _SubGlyphs) glyph->Init(index_offset);
   else _Index = index_offset++;

   // Compute the position, height, and width of this glyph.

   // Embed glyph into a rectangular model with the same position, height, and width, and initialise model.
   Model::Init();

   _isInit = true;
}

}