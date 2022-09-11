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

#include "Glyph.h"
#include "Model.h"
#include "String.h"
#include "Texture.h"

namespace aprn::vis {

/***************************************************************************************************************************************************************
* TeXBox Class
***************************************************************************************************************************************************************/
class TeXBox final : public Model
{
 public:
   TeXBox() = default;

   TeXBox(const char* str);

   TeXBox(const std::string& str);

   TeXBox(const Glyph& glyph);

   TeXBox(const DArray<Glyph>& glyphs);

   TeXBox(const String& str);

   TeXBox(const DArray<String>& strings);

   TeXBox& Add(const std::string& str);

   TeXBox& Add(const Glyph& glyph);

   TeXBox& Add(const String& str);

   TeXBox& Add(const DArray<Glyph>& glyphs);

   TeXBox& Add(const DArray<String>& strings);

   TeXBox& SetAnchor(const SVectorF3& anchor);

   TeXBox& SetFontSize(char font_size);

   TeXBox& SetColour(const Colour& colour);

   TeXBox& SetDimensions(Float width, std::optional<Float> height = std::nullopt);

   TeXBox& SetScale(Float width_scale, std::optional<Float> height_scale = std::nullopt);

   TeXBox& SetItalic(bool is_italic);

   TeXBox& SetBold(bool is_bold);

   TeXBox& SetPixelDensity(UInt density);

 private:
   friend class Scene;
   friend class Visualiser;

   void Init(size_t id);

   void InitStrings();

   void ReadGlyphBoxPositions();

   void ReadGlyphBoxAttributes();

   void ComputeGlyphSheetDimensions();

   void LinkGlyphSheet();

   void ComputeDimensions();

   fm::Path ImagePath() const;

   std::string              _Text;
   DArray<SPtr<String>>     _Strings;
   GlyphSheet               _GlyphSheet;
   std::optional<SVectorF3> _Anchor;       // Bottom-left corner.
   std::optional<SVectorF2> _Dimensions;   // [width, height] in world-space coordinates.
   std::optional<SVectorF2> _Scale;        // [width-scale, height-scale]
   char                     _FontSize{10}; // Defaults to a 10pt font.
};


}
