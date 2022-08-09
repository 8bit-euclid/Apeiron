#include "../include/TeXBox.h"
#include "../include/ModelFactory.h"

namespace aprn::vis {

/***************************************************************************************************************************************************************
* TeXBox Public Interface
***************************************************************************************************************************************************************/
TeXBox::TeXBox(const char* str, const std::string& label)
   : TeXBox(std::string(str), label) {}

TeXBox::TeXBox(const std::string& str, const std::string& label)
   : TeXBox(String(str, label + "_String_0"), label) {}

TeXBox::TeXBox(const Glyph& glyph, const std::string& label)
   : TeXBox(String(glyph, label + "_String_0"), label) {}

TeXBox::TeXBox(const DArray<Glyph>& glyphs, const std::string& label)
   : TeXBox(String(glyphs, label + "_String_0"), label) {}

TeXBox::TeXBox(const String& str, const std::string& label)
   : _Label(label) { Add(str); }

TeXBox::TeXBox(const DArray<String>& strings, const std::string& label)
   : _Label(label) { Add(strings); }

TeXBox&
TeXBox::Add(const std::string& str) { return Add(String(str, _Label + "_String_" + ToStr(_Strings.size()))); }

TeXBox&
TeXBox::Add(const Glyph& glyph) { return Add(String(glyph, _Label + "_String_" + ToStr(_Strings.size()))); }

TeXBox&
TeXBox::Add(const String& str)
{
   // Allocate new memory for the string, initialise it, and add it as a sub-model of this TeX-box.
   const std::string& str_id = !str._Label.empty() ? str._Label : "String_" + ToStr(_Strings.size());
   _Strings.push_back(std::make_shared<String>(str));
   _Strings.back()->Init();
   _SubModels.emplace(str_id, _Strings.back());
   return *this;
}

TeXBox&
TeXBox::Add(const DArray<Glyph>& glyphs)
{
   Add(String(glyphs, _Label + "_String_" + ToStr(_Strings.size())));
   return *this;
}

TeXBox&
TeXBox::Add(const DArray<String>& strings)
{
   FOR_EACH(str, strings) Add(str);
   return *this;
}

TeXBox&
TeXBox::SetLabel(const std::string& label)
{
   ASSERT(_Label.empty(), "The label for this TeX box has already been set.")
   _Label = label;
   return *this;
}

TeXBox&
TeXBox::SetAnchor(const SVectorF3& anchor)
{
   _Anchor = anchor;
   FOR_EACH(str, _Strings) str->SetAnchor(&_Anchor);
   return *this;
}

TeXBox&
TeXBox::SetColour(const Colour& colour)
{
   FOR_EACH(str, _Strings) str->SetColour(colour);
   return *this;
}

TeXBox&
TeXBox::SetDimensions(const Float width, const std::optional<Float> height)
{
   ASSERT(!_Scale.has_value(), "Cannot set both the scale and dimensions of the TeX-box.")
   _Dimensions = {width, height.has_value() ? height.value() : -One};
   return *this;
}

TeXBox&
TeXBox::SetScale(const Float width_scale, const std::optional<Float> height_scale)
{
   ASSERT(!_Dimensions.has_value(), "Cannot set both the scale and dimensions of the TeX-box.")
   _Scale = {width_scale, height_scale.has_value() ? height_scale.value() : width_scale};
   FOR_EACH(str, _Strings) str->SetScale(width_scale, height_scale);
   return *this;
}

TeXBox&
TeXBox::SetItalic(const bool is_italic)
{
   FOR_EACH(str, _Strings) str->SetItalic(is_italic);
   return *this;
}

TeXBox&
TeXBox::SetBold(const bool is_bold)
{
   FOR_EACH(str, _Strings) str->SetBold(is_bold);
   return *this;
}

/***************************************************************************************************************************************************************
* TeXBox Private Interface
***************************************************************************************************************************************************************/
void
TeXBox::Init()
{
   // Compute the position, height, and width of this TeX-box.
   GlyphSheet::IndexType offset{};
   _Text.clear();
   FOR_EACH(str, _Strings)
   {
      // Add contribution from the string to the TeX-box string.
      str->Init();
      _Text += str->_Text;

      // Add and update index offset.
      str->OffsetIndices(offset);
      offset += str->_N_Char;
   }

   // Embed TeX-box into a rectangular model with the same position, height, and width, and initialise model.
   Model::Init();
}

}