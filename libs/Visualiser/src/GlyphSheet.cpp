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

#include "../include/GlyphSheet.h"
#include "FileManager/include/File.h"

namespace aprn::vis {

void
GlyphSheet::Init(size_t id, const std::string& text)
{
   ASSERT(!text.empty(), "The TeXBox being compiled has no text.")

   // Set the compile directory for this glyph sheet.
   _CompileDirectory = LaTeXDirectory() / ("texbox" + ToString(id));

   // Compile LaTeX and generate image.
   CompileLaTeXSource(text);
   CreateGlyphSheetImage();

   // Read glyph data and size the glyph sheet.
   ReadGlyphBoxPositions();
   ReadGlyphBoxAttributes();
   ComputeDimensions();
}

void
GlyphSheet::CompileLaTeXSource(const std::string& text)
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
   file.Write("\n", text, "\n\\end{document}");
   file.Close();

   // Compile LaTeX source code.
   fm::CompileTeXFile("lualatex", _TeXFile);
}

void
GlyphSheet::CreateGlyphSheetImage() { fm::ConvertPDFtoPNG(_TeXFile.replace_extension(".pdf"), _PixelDensity); }

void
GlyphSheet::ReadGlyphBoxPositions()
{
   fm::File file(_CompileDirectory / "positions.txt", fm::Mode::Read);
   UInt16 glyph_index{};

   while(!file.isEnd())
   {
      auto& x = _Boxes[glyph_index].Position.x(); // Note: emplacement occurs here.
      auto& y = _Boxes[glyph_index].Position.y();

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
GlyphSheet::ReadGlyphBoxAttributes()
{
   // Read glyph box attributes. Note: need to read with a wide file, as the glyph characters must be read in as wchar_t.
   fm::WFile wfile;
   wfile.Open(_CompileDirectory / "attributes.txt", fm::Mode::Read);
   UInt16 glyph_index{};

   while(wfile.isValid())
   {
      DEBUG_ASSERT(_Boxes.find(glyph_index) != _Boxes.end(), "The glyph index ", glyph_index, " was not previously populated.")

      auto& c = _Boxes[glyph_index].Char;
      auto& w = _Boxes[glyph_index].Width;
      auto& h = _Boxes[glyph_index].Height;
      auto& d = _Boxes[glyph_index].Depth;

      wfile.Read(c, w, h, d);
      ++glyph_index;
   }

   wfile.Close();
   ASSERT(glyph_index == _Boxes.size(), "The number of glyph attributes does not match the number of positions read in.")
}

void
GlyphSheet::ComputeDimensions()
{
   using int_T  = decltype(_Width);
   using coor_T = SVector2<int_T>;

   // Calculate glyph sheet dimensions (min/max bounds of all glyph boxes).
   coor_T min_pos(MaxInt<int_T>);
   coor_T max_pos(MinInt<int_T>);

   FOR_EACH_CONST(_, glyph, _Boxes)
   {
      const coor_T bott_left = { glyph.Position.x()              , glyph.Position.y() - glyph.Depth  };
      const coor_T top_right = { glyph.Position.x() + glyph.Width, glyph.Position.y() + glyph.Height };

      FOR(i, 2)
      {
         min_pos[i] = Min(min_pos[i], bott_left[i]);
         max_pos[i] = Max(max_pos[i], top_right[i]);
      }
   }
   _Width  = max_pos.x() - min_pos.x();
   _Height = max_pos.y() - min_pos.y();
}

}
