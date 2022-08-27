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

#include "Buffers.h"
#include "Model.h"
#include "Shader.h"
#include "Texture.h"

namespace aprn::vis {

enum class PostProcessType
{
   HDR,
   Blur,
   MotionBlur,
   Fade,
   Vignette
};

struct FrameImage
{
   template<class type> using UMap = std::unordered_map<std::string, type>;
   FrameBuffer   FBO;
   RenderBuffer  RBO;
   UMap<Texture> Textures;
};

class PostProcessor
{
 public:
   void Init(UInt width, UInt height, bool is_hdr);

   void Render();

   void StartWrite() const;

   inline void StopWrite() const { _Output.FBO.Unbind(); }

 private:
   template<class type> using UMap = std::unordered_map<std::string, type>;
   Model            _ScreenQuad;
   FrameImage       _Output;
   UMap<FrameImage> _FrameImages;
   UMap<Shader>     _Shaders;
   UInt             _Width;
   UInt             _Height;
   bool             _isHDR;
};

}
