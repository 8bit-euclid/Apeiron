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
#include "GlyphSheet.h"
#include "ModelGroup.h"

namespace aprn::vis {

class String : public TeXObject,
               public ModelGroup
{
 public:
   String() = default;

   explicit String(const char* str);

   explicit String(const std::string& str);

   explicit String(const TeXGlyph& glyph);

   explicit String(const DArray<TeXGlyph>& glyphs);

   String& Add(const std::string& str);

   String& Add(const TeXGlyph& glyph);

   String& Add(const DArray<TeXGlyph>& glyphs);

   String& SetColour(const SVectorR4& rgba_colour) override;

   String& SetColour(const Colour& colour) override;

   String& SetItalic(bool is_italic) override;

   String& SetBold(bool is_bold) override;

   /** Sub-TeXObject Addition
   ************************************************************************************************************************************************************/
   String& Add(Model& model, const std::string& name);

   String& Add(ModelGroup& model_group, const std::string& name);

   String& Add(Model&& model, const std::string& name);

   String& Add(ModelGroup&& model_group, const std::string& name);

 private:
   friend class TeXBox;

   void Init(GlyphSheet::IndexT& index_offset);

   DArray<TeXGlyph> Parse(const std::string& str);

   void ComputeDimensions(const GlyphSheet& glyph_sheet, const UChar font_size, const SVectorR3& texbox_anchor, const SVectorR2& texbox_dimensions);

   void LoadSubGlyphTextures(const Pair<std::string, Real>& texture_info);

   DArray<SPtr<TeXObject>> SubTeXObjects_;
};

}