#pragma once

#include "../../../include/Global.h"
#include "DataContainer/include/Array.h"
#include "Glyph.h"
#include "../include/Model.h"

namespace aprn::vis {

class String : public Model
{
 public:
   String() = default;

   String(const std::string& str, const std::string& label = "");

   String(const Glyph& glyph, const std::string& label = "");

   String(const DArray<Glyph>& glyphs, const std::string& label = "");

   String& Add(const std::string& str);

   String& Add(const Glyph& glyph);

   String& Add(const DArray<Glyph>& glyphs);

   String& SetLabel(const std::string& label);

   String& SetColour(const Colour& colour);

   String& SetScale(const Float width_scale, const std::optional<Float> height_scale = std::nullopt);

   String& SetItalic(bool is_italic);

   String& SetBold(bool is_bold);

 private:
   friend class TeXBox;

   void Init();

   void SetAnchor(const SVectorF3* anchor);

   DArray<Glyph> Parse(const std::string& str);

   inline bool areIndicesOffset() const { return _Glyphs.front()->isIndexOffset(); }

   inline void OffsetIndices(GlyphSheet::IndexType offset) { FOR_EACH(glyph, _Glyphs) glyph->OffsetIndex(offset); }

   std::string         _Label;
   std::string         _Text;
   unsigned            _N_Char{};
   DArray<SPtr<Glyph>> _Glyphs;
   const SVectorF3*    _Anchor; // Bottom-left corner of the parent TeX-box
   Float               _Width;
   Float               _Height;
};

}