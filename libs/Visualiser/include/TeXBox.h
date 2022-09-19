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
#include "TeXObject.h"
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

   TeXBox(const char* str);

   TeXBox(const std::string& str);

   TeXBox(const TeXGlyph& glyph);

   TeXBox(const DArray<TeXGlyph>& glyphs);

   TeXBox(const TeXBox& tex_box);

   TeXBox(const DArray<TeXBox>& tex_boxes);

   TeXBox& Add(const char* str);

   TeXBox& Add(const std::string& str);

   TeXBox& Add(const TeXGlyph& glyph);

   TeXBox& Add(const String& str);

   TeXBox& Add(const DArray<TeXGlyph>& glyphs);

   TeXBox& Add(const DArray<String>& strings);

   TeXBox& SetPixelDensity(UInt density);

   TeXBox& SetAnchor(const SVectorR3& anchor);

   TeXBox& SetFontSize(const UChar font_size);

   TeXBox& SetColour(const Colour& colour);

   TeXBox& SetItalic(bool is_italic);

   TeXBox& SetBold(bool is_bold);

 private:
   friend class Scene;
   friend class Visualiser;

   void Init(size_t id);

   void InitSubGlyphs();

   void ComputeDimensions();

   void LoadSubGlyphTextures(const Pair<std::string, Real>& texture_info);

   fm::Path ImagePath() const;

   std::string             _Text;
   DArray<SPtr<TeXObject>> _SubGlyphs;
   GlyphSheet              _GlyphSheet;
   SVectorR2               _Dimensions{}; // [width, height] in world-space.
   SVectorR3               _Anchor{};     // Bottom-left corner.
   UChar                   _FontSize{10}; // Defaults to a 10pt font.
};


}
