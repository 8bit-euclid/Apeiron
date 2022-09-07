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
   : TeXBox(DArray<String>{str}, label) {}

TeXBox::TeXBox(const DArray<String>& strings, const std::string& label)
   : _Label(label) { Add(strings); }

TeXBox&
TeXBox::Add(const std::string& str) { return Add(String(str, _Label + "_String_" + ToString(_Strings.size()))); }

TeXBox&
TeXBox::Add(const Glyph& glyph) { return Add(String(glyph, _Label + "_String_" + ToString(_Strings.size()))); }

TeXBox&
TeXBox::Add(const String& str)
{
   // Allocate new memory for the string, initialise it, and add it as a sub-model of this TeX-box.
   const std::string& str_id = !str._Label.empty() ? str._Label : "String_" + ToString(_Strings.size());
   _Strings.emplace_back(std::make_shared<String>(str));
   _SubModels.emplace(str_id, _Strings.back());
   return *this;
}

TeXBox&
TeXBox::Add(const DArray<Glyph>& glyphs)
{
   Add(String(glyphs, _Label + "_String_" + ToString(_Strings.size())));
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
TeXBox::SetFontSize(const char font_size)
{
   FOR_EACH(str, _Strings) str->SetFontSize(font_size);
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

   _Dimensions = { width, height.has_value() ? height.value() : -One };
   return *this;
}

TeXBox&
TeXBox::SetScale(const Float width_scale, const std::optional<Float> height_scale)
{
   ASSERT(!_Dimensions.has_value(), "Cannot set both the scale and dimensions of the TeX-box.")

   _Scale = { width_scale, height_scale.has_value() ? height_scale.value() : width_scale };
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

TeXBox&
TeXBox::SetPixelDensity(const UInt density)
{
   _PixelDensity = density;
   return *this;
}

/***************************************************************************************************************************************************************
* TeXBox Private Interface
***************************************************************************************************************************************************************/
void
TeXBox::Init(const size_t id)
{
   AddStringText();
   SetCompileDirectory(id);
   CompileLaTeXSource();
   CreateTeXBoxImage();
   CreateGlyphSheet();
   ComputeTeXBoxDimensions();
   Model::Init();
}

void
TeXBox::AddStringText()
{
   // Add contributions from each sub-string to the TeX-box string.
   UInt16 glyph_index{};
   _Text.clear();
   FOR_EACH(str, _Strings)
   {
      str->Init(glyph_index);
      _Text += str->_Text;
   }
}

void
TeXBox::CompileLaTeXSource()
{
   const auto& comp_dir = _CompileDirectory;

   // Initialise the LaTeX compile directory for this TeX-box and copy over the LaTeX template.
   fm::CreateDirectory(comp_dir);
   fm::ClearDirectory(comp_dir);
   fm::CopyFile(LaTeXTemplate() , comp_dir);
   fm::CopyFile(LuaTeXTemplate(), comp_dir);

   // Transfer TeX-box text to the .tex file.
   _TeXFile = comp_dir / LaTeXTemplate().filename();
   fm::File file(_TeXFile, fm::Mode::Append);
   file.Write("\n", _Text, "\n\\end{document}");
   file.Close();

   // Compile LaTeX source code.
   fm::CompileTeXFile("lualatex", _TeXFile);
}

void
TeXBox::CreateTeXBoxImage() { fm::ConvertPDFtoPNG(_TeXFile.replace_extension(".pdf"), _PixelDensity); }

void
TeXBox::CreateGlyphSheet()
{
   ReadGlyphBoxPositions();
   ReadGlyphBoxAttributes();
   ComputeGlyphSheetDimensions();
   LinkGlyphSheet();
}

void
TeXBox::ReadGlyphBoxPositions()
{
   fm::File file(_CompileDirectory / "positions.txt", fm::Mode::Read);
   UInt16 glyph_index{};

   while(!file.isEnd())
   {
      auto& x = _GlyphSheet.Boxes[glyph_index].Position.x(); // Note: emplacement occurs here.
      auto& y = _GlyphSheet.Boxes[glyph_index].Position.y();

      file.Read(x, y);
      if(x < 0)
      {
         DEBUG_ASSERT(x == -1, "Read in an x-coordinate which is not -1: ", x)
         x = 0;
      }
      ++glyph_index;
   }
   file.Close();
}

void
TeXBox::ReadGlyphBoxAttributes()
{
   // Read glyph box attributes. Note: need to read with a wide file, as the glyph characters must be read in as wchar_t.
   fm::WFile wfile;
   wfile.Open(_CompileDirectory / "attributes.txt", fm::Mode::Read);
   UInt16 glyph_index{};

   while(wfile.isValid())
   {
      DEBUG_ASSERT(_GlyphSheet.Boxes.find(glyph_index) != _GlyphSheet.Boxes.end(), "The glyph index ", glyph_index, " was not previously populated.")

      auto& c = _GlyphSheet.Boxes[glyph_index].Char;
      auto& w = _GlyphSheet.Boxes[glyph_index].Width;
      auto& h = _GlyphSheet.Boxes[glyph_index].Height;
      auto& d = _GlyphSheet.Boxes[glyph_index].Depth;

      wfile.Read(c, w, h, d);
      ++glyph_index;
   }

   wfile.Close();
   ASSERT(glyph_index == _GlyphSheet.Boxes.size(), "The number of glyph attributes does not match the number of positions read in.")
}

void
TeXBox::ComputeGlyphSheetDimensions()
{
   using int_T  = decltype(_GlyphSheet.Width);
   using coor_T = SVector2<int_T>;

   // Calculate glyph sheet dimensions (min/max bounds of all glyph boxes).
   coor_T min_pos(MaxInt<int_T>);
   coor_T max_pos(MinInt<int_T>);

   FOR_EACH_CONST(_, glyph, _GlyphSheet.Boxes)
   {
      const coor_T bott_left = { glyph.Position.x()              , glyph.Position.y() - glyph.Depth  };
      const coor_T top_right = { glyph.Position.x() + glyph.Width, glyph.Position.y() + glyph.Height };

      FOR(i, 2)
      {
         min_pos[i] = Min(min_pos[i], bott_left[i]);
         max_pos[i] = Max(max_pos[i], top_right[i]);
      }
   }
   _GlyphSheet.Width  = max_pos.x() - min_pos.x();
   _GlyphSheet.Height = max_pos.y() - min_pos.y();
}

void
TeXBox::LinkGlyphSheet() { FOR_EACH(str, _Strings) str->LinkGlyphSheet(&_GlyphSheet); }

void
TeXBox::ComputeTeXBoxDimensions()
{
   ASSERT(_GlyphSheet.Width && _GlyphSheet.Height, "The dimensions of the glyph sheet must be computed before those of the TeXBox.")
   if(_Dimensions.has_value()) return;

   // Compute the world-space dimensions by converting glyph sheet dimensions from scaled point dimensions.
   const Float scale_factor = _UnitLength / static_cast<Float>(_FontSize10);
   _Dimensions = { static_cast<Float>(_GlyphSheet.Width), static_cast<Float>(_GlyphSheet.Height) };
   _Dimensions.value() *= scale_factor;

   // Set model mesh.
   _Mesh = ModelFactory::Rectangle(_Dimensions->x(), _Dimensions->y())._Mesh;
}

void
TeXBox::SetCompileDirectory(const size_t id) { _CompileDirectory = LaTeXDirectory() / ("texbox" + ToString(id)); }

fm::Path
TeXBox::ImagePath() const
{
   ASSERT(!_CompileDirectory.empty(), "The compile directory has not yet been set for this TeXBox.")
   return _CompileDirectory / LaTeXTemplate().filename().replace_extension(".png");
}

}