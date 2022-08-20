#include "../include/TeXBox.h"
#include "../include/ModelFactory.h"
#include "../include/Texture.h"
#include "FileManager/include/File.h"

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
   _Strings.emplace_back(std::make_shared<String>(str));
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
TeXBox::Init(const size_t id)
{

   // Compute the position, height, and width of this TeX-box.
   uint16_t glyph_index{};
   _Text.clear();
   FOR_EACH(str, _Strings)
   {
      // Add contribution from the string to the TeX-box string.
      str->Init(glyph_index);
      _Text += str->_Text;
   }

   SetCompileDirectory(id);
   CreateTeXBoxImage();
   CreateGlyphSheet();

   // Embed TeX-box into a rectangular model with the same position, height, and width, and initialise model.
   Model::Init();
}

void
TeXBox::CreateTeXBoxImage()
{
   const auto& comp_dir = _CompileDirectory;

   // Initialise the LaTeX compile directory for this TeX-box and copy over the LaTeX template.
   fm::CreateDirectory(comp_dir);
   fm::ClearDirectory(comp_dir);
   fm::CopyFile(_LaTeXTemplate , comp_dir);
   fm::CopyFile(_LuaTeXTemplate, comp_dir);

   // Transfer TeX-box text to the .tex file.
   fm::Path file_path = comp_dir / _LaTeXTemplate.filename();
   fm::File file(file_path, fm::Mode::Append);
   file.Write("\n", _Text, "\n\\end{document}");
   file.Close();

   // Compile LaTeX source code.
   fm::CompileTeXFile("lualatex", file_path);
   fm::ConvertPDFtoPNG(file_path.replace_extension(".pdf"), 500); // NOTE: pixel density is currently hard-coded.
}

void
TeXBox::CreateGlyphSheet()
{
   const auto& comp_dir = _CompileDirectory;
   fm::File file;

   // Read glyph box positions.
   file.Open(comp_dir / "positions.txt", fm::Mode::Read);
   UInt16 glyph_index{};
   while(!file.isEnd())
   {
      auto& x = _GlyphSheet.Boxes[glyph_index].Position.x();
      auto& y = _GlyphSheet.Boxes[glyph_index].Position.y();

      file.Read(x, y);
      if(x < 0) x = 0;
      ++glyph_index;
   }
   const auto glyph_count = glyph_index;
   file.Close();

   // Read glyph box attributes. Note: need to read with a wide file, as the glyph characters must be read in as wchar_t.
   fm::WFile wfile;
   wfile.Open(comp_dir / "attributes.txt", fm::Mode::Read);
   glyph_index = 0;
   while(wfile.isValid())
   {
      auto& c = _GlyphSheet.Boxes[glyph_index].Char;
      auto& w = _GlyphSheet.Boxes[glyph_index].Width;
      auto& h = _GlyphSheet.Boxes[glyph_index].Height;
      auto& d = _GlyphSheet.Boxes[glyph_index].Depth;

      wfile.Read(c, w, h, d);
      ++glyph_index;
   }
   wfile.Close();
   ASSERT(glyph_index == glyph_count, "The number of glyph attributes does not match the number of positions read in.")

   // Calculate TeX box dimensions (min/max bounds of all glyph boxes).
   using int_T = decltype(_GlyphSheet.Width);
   SVector2<int_T> min_pos(MaxInt<int_T>), max_pos(MinInt<int_T>);
   FOR_EACH_CONST(_, glyph, _GlyphSheet.Boxes)
   {
      SArray2<int_T> bott_left = { glyph.Position.x(), glyph.Position.y() - glyph.Depth };
      SArray2<int_T> top_right = { glyph.Position.x() + glyph.Width, glyph.Position.y() + glyph.Height };

      FOR(i, 2)
      {
         min_pos[i] = Min(min_pos[i], bott_left[i]);
         max_pos[i] = Max(max_pos[i], top_right[i]);
      }
   }
   _GlyphSheet.Width  = max_pos.x() - min_pos.x();
   _GlyphSheet.Height = max_pos.y() - min_pos.y();

   // Load texture
   fm::Path image_path = comp_dir / _LaTeXTemplate.filename().replace_extension(".png");
   _GlyphSheet.Image = std::make_shared<Texture>(TextureType::Diffuse, image_path);
}

void
TeXBox::ComputeDimensions()
{

}

void
TeXBox::ComputeScale()
{

}

void
TeXBox::SetCompileDirectory(const size_t id) { _CompileDirectory = _LaTeXDirectory / ("texbox" + ToStr(id)); }

fm::Path
TeXBox::ImagePath() const
{
   ASSERT(!_CompileDirectory.empty(), "The compile directory has not yet been set for this TeXBox.")
   return _CompileDirectory / _LaTeXTemplate.filename().replace_extension(".png");
}

void
TeXBox::InitTeXDirectory()
{
   fm::CreateDirectory(_LaTeXDirectory);
   fm::ClearDirectory(_LaTeXDirectory);
}

}