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
#include "FileManager/include/FileSystem.h"

#include "Glyph.h"
#include "Model.h"
#include "String.h"
#include "Texture.h"

namespace aprn::vis {

namespace fm = flmgr;

/***************************************************************************************************************************************************************
* TeXBox Class
***************************************************************************************************************************************************************/
class TeXBox final : public Model
{
 public:
   TeXBox() = default;

   TeXBox(const char* str, const std::string& label);

   TeXBox(const std::string& str, const std::string& label);

   TeXBox(const Glyph& glyph, const std::string& label);

   TeXBox(const DArray<Glyph>& glyphs, const std::string& label);

   TeXBox(const String& str, const std::string& label);

   TeXBox(const DArray<String>& strings, const std::string& label);

   TeXBox& Add(const std::string& str);

   TeXBox& Add(const Glyph& glyph);

   TeXBox& Add(const String& str);

   TeXBox& Add(const DArray<Glyph>& glyphs);

   TeXBox& Add(const DArray<String>& strings);

   TeXBox& SetLabel(const std::string& label);

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

   void AddStringText();

   void CompileLaTeXSource();

   void CreateTeXBoxImage();

   void CreateGlyphSheet();

   void ReadGlyphBoxPositions();

   void ReadGlyphBoxAttributes();

   void ComputeGlyphSheetDimensions();

   void LinkGlyphSheet();

   void ComputeTeXBoxDimensions();

   void SetCompileDirectory(size_t id);

   fm::Path ImagePath() const;

   static void InitTeXDirectory() { fm::CreateDirectory(LaTeXDirectory(), true); }

   static fm::Path LaTeXDirectory() { return "./libs/Visualiser/data/latex"; }

   static fm::Path LaTeXTemplate() { return "./libs/Visualiser/resources/latex/texbox.tex"; }

   static fm::Path LuaTeXTemplate() { return "./libs/Visualiser/resources/latex/write_boxes.lua"; }

   std::string              _Label;
   std::string              _Text;
   DArray<SPtr<String>>     _Strings;
   SVectorF3                _Anchor;             // Bottom-left corner.
   char                     _FontSize{10};       // Defaults to a 10pt font.
   std::optional<SVectorF2> _Dimensions;         // [width, height] in world-space coordinates
   std::optional<SVectorF2> _Scale;
   GlyphSheet               _GlyphSheet;
   fm::Path                 _CompileDirectory;
   fm::Path                 _TeXFile;
   UInt                     _PixelDensity{2000}; // Dots per inch
   constexpr static UInt32  _FontSize10{655360}; // Height of a 10pt font size expressed in LaTeX scaled points (1pt = 65536sp).
   constexpr static Float   _UnitLength{1.0};    // Unit of length in world space equivalent to a 10pt font size.
};


}
