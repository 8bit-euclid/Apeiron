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

#pragma once

#include "../../../include/Global.h"
#include "LinearAlgebra/include/Vector.h"
#include "Colour.h"
#include "Model.h"
#include "GlyphSheet.h"
#include "TeXObject.h"

#include <optional>

namespace aprn::vis {

class TeXGlyph final : public TeXObject,
                       public Model
{
 public:
   TeXGlyph() = default;

   explicit TeXGlyph(const char tex_char);

   explicit TeXGlyph(const std::string& tex_str);

   TeXGlyph& Set(const char tex_char);

   TeXGlyph& Set(const std::string& tex_str);

   TeXGlyph& Add(const char tex_char);

   TeXGlyph& Add(const std::string& str);

   TeXGlyph& Add(std::string&& str);

   TeXGlyph& SetColour(const SVectorR4& rgba_colour) override;

   TeXGlyph& SetColour(const Colour& colour) override;

   TeXGlyph& SetItalic(bool is_italic) override;

   TeXGlyph& SetBold(bool is_bold) override;

   inline void DoNotRender() { Render_ = false; }

   inline bool Rendered() const { return Render_; }

 private:
   friend class TeXBox;

   void InitTeXObject(GlyphSheet::IndexT& index_offset) override;

   void ComputeDimensions(const GlyphSheet& glyph_sheet, UChar font_size, const SVectorR3& texbox_anchor, const SVectorR2& texbox_dimensions) override;

   GlyphSheet::IndexT    Index_{MaxInt<GlyphSheet::IndexT>};
   std::optional<Colour> Colour_;
   std::optional<bool>   isItalic_;
   std::optional<bool>   isBold_;
   bool                  Render_{true};
   bool                  Init_{false};
};

}