#pragma once

#include "../../../include/Global.h"
#include "DataContainer/include/Array.h"
#include "FileManager/include/FileSystem.h"

#include "Glyph.h"
#include "Model.h"
#include "String.h"

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

   TeXBox& SetColour(const Colour& colour);

   TeXBox& SetDimensions(const Float width, const std::optional<Float> height = std::nullopt);

   TeXBox& SetScale(const Float width_scale, const std::optional<Float> height_scale = std::nullopt);

   TeXBox& SetItalic(const bool is_italic);

   TeXBox& SetBold(const bool is_bold);

 private:
   friend class Scene;
   friend class Visualiser;

   void Init(const size_t id);

   void CreateTeXBoxImage();

   void CreateGlyphSheet();

   void ComputeDimensions();

   void ComputeScale();

   void SetCompileDirectory(const size_t id);

   static void InitTeXDirectory();

   std::string              _Label;
   std::string              _Text;
   DArray<SPtr<String>>     _Strings;
   SVectorF3                _Anchor; // Bottom-left corner
   GlyphSheet               _GlyphSheet;
   std::optional<SVectorF2> _Scale;
   std::optional<SVectorF2> _Dimensions;
   fm::Path                 _CompileDirectory;

   typedef const flmgr::Path Path;
   inline static Path _LaTeXDirectory = "./libs/Visualiser/data/latex";
   inline static Path _LaTeXTemplate  = "./libs/Visualiser/resources/latex/texbox.tex";
   inline static Path _LuaTeXTemplate = "./libs/Visualiser/resources/latex/write_boxes.lua";
};


}
