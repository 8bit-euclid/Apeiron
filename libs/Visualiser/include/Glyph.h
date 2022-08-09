#pragma once

#include "../../../include/Global.h"
#include "LinearAlgebra/include/Vector.h"
#include "Visualiser/include/Colour.h"
#include "Visualiser/include/Model.h"
#include "GlyphSheet.h"

namespace aprn::vis {

/***************************************************************************************************************************************************************
* Glyph Attribute Class
***************************************************************************************************************************************************************/
template<typename T>
class GlyphAttribute
{
 public:
   GlyphAttribute() {}

   GlyphAttribute(const T& attribute) : _Data(attribute) {}

   inline void Set(const T& attribute) { _isSet = true; _Data = attribute; }

   inline bool isSet() const { return _isSet; }

   inline const T& Get() const { return _Data; }

 private:
   T    _Data{};
   bool _isSet{false};
};

/***************************************************************************************************************************************************************
* Glyph Class
***************************************************************************************************************************************************************/
class Glyph final : public Model
{
 public:
   Glyph() = default;

   Glyph(const char tex_char, const std::string& label = "");

   Glyph(const std::string& tex_str, const std::string& label = "");

   Glyph& Set(const char tex_char);

   Glyph& Set(const std::string& tex_str);

   Glyph& Set(Glyph&& glyph);

   Glyph& SetLabel(const std::string& label);

   Glyph& SetColour(const Colour& colour);

   Glyph& SetScale(const Float width_scale, const std::optional<Float> height_scale = std::nullopt);

   Glyph& SetItalic(const bool is_italic);

   Glyph& SetBold(const bool is_bold);

   void Add(const Glyph& glyph);

   void Add(const DArray<Glyph>& glyphs);

   void Add(Glyph&& glyph);

   void Add(DArray<Glyph>&& glyphs);

   void Add(const std::string& str);

   void Add(std::string&& str);

   inline void DoNotRender() { _Render = false; }

   inline bool isRendered() const { return _Render; }

   inline const std::string& GetText() const { return _Text; }

 private:
   friend class String;

   void Init();

   bool isCompound() const;

   bool isIndexOffset() const;

   void OffsetIndex(GlyphSheet::IndexType offset);

   inline void SetAnchor(const SVectorF3* anchor) { _Anchor = anchor; }

   std::string               _Label;
   std::string               _Text;
   GlyphSheet::IndexType     _Index{};
   size_t                    _N_Char{};
   GlyphAttribute<Colour>    _Colour{};
   GlyphAttribute<SVectorF2> _Scale{{One, One}};
   GlyphAttribute<bool>      _isItalic{false};
   GlyphAttribute<bool>      _isBold{false};
   const SVectorF3*          _Anchor{}; // Bottom-left corner of the parent TeX-box
   GlyphBox<Float>           _Box;
   GlyphBox<int>*            _GlyphSheetBox{};
   GlyphSheet*               _GlyphSheet{};
   DArray<SPtr<Glyph>>       _SubGlyphs;
   bool                      _Render{true};

   /** Friend unit tests */
   friend class ParseTeXTest_ParseTeXChar_Test;
   friend class ParseTeXTest_ParseTeXMath_Test;
   friend class ParseTeXTest_ParseTeXGlyph_Test;
   friend class ParseTeXTest_ParseTeXString_Test;
   friend class ParseTeXTest_ParseTeXCommand_Test;
   friend class ParseTeXTest_ParseTeXScriptText_Test;
   friend class ParseTeXTest_ParseAllTeXScriptText_Test;
};

}