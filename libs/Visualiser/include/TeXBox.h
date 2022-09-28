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
#include "DataContainer/include/Array.h"

#include "TeXGlyph.h"
#include "String.h"
#include "ModelGroup.h"
#include "Texture.h"

namespace aprn::vis {

/***************************************************************************************************************************************************************
* TeXBox Class
***************************************************************************************************************************************************************/
class TeXBox final : public TeXObject,
                     public ModelGroup
{
 public:
   TeXBox() = default;

   explicit TeXBox(const char* str);

   explicit TeXBox(const std::string& str);

   explicit TeXBox(const TeXGlyph& tex_glyph);

   explicit TeXBox(const TeXBox& tex_box);

   explicit TeXBox(const DArray<TeXGlyph>& tex_glyphs);

   explicit TeXBox(const DArray<TeXBox>& tex_boxes);

   TeXBox& Add(const char* str);

   TeXBox& Add(const std::string& str);

   TeXBox& Add(const TeXGlyph& tex_glyph);

   TeXBox& Add(const TeXBox& tex_box);

   TeXBox& Add(const DArray<TeXGlyph>& tex_glyphs);

   TeXBox& Add(const DArray<TeXBox>& tex_boxes);

   TeXBox& SetPixelDensity(UInt density);

   TeXBox& SetAnchor(const SVectorR3& anchor);

   TeXBox& SetFontSize(const UChar font_size);

   TeXBox& SetColour(const SVectorR4& rgba_colour) override;

   TeXBox& SetColour(const Colour& colour) override;

   TeXBox& SetItalic(bool is_italic) override;

   TeXBox& SetBold(bool is_bold) override;

 private:
   friend class Scene;
   friend class Visualiser;

   void Init(size_t id);

   void InitSubGlyphs();

   void ComputeDimensions();

   void LoadSubGlyphTextures(const Pair<std::string, Real>& texture_info);

   fm::Path ImagePath() const;

   DArray<SPtr<TeXObject>> SubBoxes_;
   GlyphSheet              GlyphSheet_;
   SVectorR2               Dimensions_{}; // [width, height] in world-space.
   SVectorR3               Anchor_{};     // Bottom-left corner.
   UChar                   FontSize_{10}; // Defaults to a 10pt font.
};


}
