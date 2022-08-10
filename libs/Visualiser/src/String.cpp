#include "../include/ModelFactory.h"
#include "../include/ParseTeX.h"
#include "../include/String.h"

namespace aprn::vis {

/***************************************************************************************************************************************************************
* String Public Interface
***************************************************************************************************************************************************************/
String::String(const char* str, const std::string& label)
   : String(std::string(str), label) {}

String::String(const std::string& str, const std::string& label)
   : _Label(label) { Add(str); }

String::String(const Glyph& glyph, const std::string& label)
   : _Label(label) { Add(glyph); }

String::String(const DArray<Glyph>& glyphs, const std::string& label)
   : _Label(label) { Add(glyphs); }

String&
String::Add(const std::string& str)
{
   Add(Parse(str));
   return *this;
}

String&
String::Add(const Glyph& glyph)
{
   // Allocate new memory for the glyph, initialise it, and add it as a sub-model of this string.
   const std::string& glyph_id = !glyph._Label.empty() ? glyph._Label : "Glyph_" + ToStr(_Glyphs.size());
   _Glyphs.push_back(std::make_shared<Glyph>(glyph));
   _Glyphs.back()->Init();
   _SubModels.emplace(glyph_id, _Glyphs.back());
   return *this;
}

String&
String::Add(const DArray<Glyph>& glyphs)
{
   FOR_EACH_CONST(glyph, glyphs) Add(glyph);
   return *this;
}

String&
String::SetLabel(const std::string& label)
{
   _Label = label;
   return *this;
}

String&
String::SetColour(const Colour& colour)
{
   FOR_EACH(glyph, _Glyphs) glyph->SetColour(colour);
   return *this;
}

String&
String::SetScale(const Float width_scale, const std::optional<Float> height_scale)
{
   FOR_EACH(glyph, _Glyphs) glyph->SetScale(width_scale, height_scale);
   return *this;
}

String&
String::SetItalic(bool is_italic)
{
   FOR_EACH(glyph, _Glyphs) glyph->SetItalic(is_italic);
   return *this;
}

String&
String::SetBold(bool is_bold)
{
   FOR_EACH(glyph, _Glyphs) glyph->SetBold(is_bold);
   return *this;
}

/***************************************************************************************************************************************************************
* String Private Interface
***************************************************************************************************************************************************************/
void
String::Init()
{
   // Compute the position, height, and width of this string.
   GlyphSheet::IndexType offset{};
   _Text.clear();
   FOR_EACH(glyph, _Glyphs)
   {
      // Add contribution from the glyph to the TeX string.
      glyph->Init();
      _Text += glyph->_Text;

      // Add and update index offset.
      glyph->OffsetIndex(offset);
      offset += glyph->_N_Char;
   }

   // Set total number of characters associated to this string.
   _N_Char = offset;

   // Embed string into a rectangular model with the same position, height, and width, and initialise model.
   Model::Init();
}

void
String::SetAnchor(const SVectorF3* anchor)
{
   _Anchor = anchor;
   FOR_EACH(glyph, _Glyphs) glyph->SetAnchor(_Anchor);
}

DArray<Glyph>
String::Parse(const std::string& str)
{
   auto current = str.begin();
   return ParseTeXString(current, str.end());
}

}