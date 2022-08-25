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

#include "../include/FrameTexture.h"
#include "../include/ModelFactory.h"
#include "../include/Window.h"

namespace aprn::vis {

FrameTexture::FrameTexture()
   : _Texture(TextureType::Diffuse, true) {}

void
FrameTexture::Init(const UInt width, const UInt height, bool is_hdr)
{
   _isHDR  = is_hdr;
   _Width  = width;
   _Height = height;

   _FBO.Init();
   _FBO.Bind();

   // Attach a texture as the frame buffer's colour buffer.
   const bool high_prec = false;
   const GLint internal_format = _isHDR ? (high_prec ? GL_RGBA32F : GL_RGBA16F) : GL_RGBA;
   _Texture.Init(_Width, _Height, internal_format, GL_RGBA, GL_FLOAT, GL_CLAMP_TO_EDGE);
   _FBO.AttachTexture2D(GL_COLOR_ATTACHMENT0, _Texture.ID());
   _FBO.Draw(GL_COLOR_ATTACHMENT0);

   // Attach a render buffer as the frame buffer's depth/stencil buffer.
   _RBO.Init();
   _RBO.Allocate(GL_DEPTH24_STENCIL8, _Width, _Height);
   _FBO.AttachRenderBuffer(GL_DEPTH_STENCIL_ATTACHMENT, _RBO.ID());

   _FBO.Unbind();

   // Create rectangular screen-filling quad to render texture to.
   _TextureQuad = ModelFactory::ScreenQuad();
   _TextureQuad.Init();
}

void
FrameTexture::Render(Shader& shader)
{
   GLCall(glClearColor(1.0f, 1.0f, 1.0f, 1.0f));
   GLCall(glClear(GL_COLOR_BUFFER_BIT));
   GLCall(glDisable(GL_DEPTH_TEST));

   shader.Bind();

   shader.UseTexture(_Texture, "u_screen_texture", 0);
   shader.SetUniform1i("u_is_hdr"  , _isHDR);
   shader.SetUniform1f("u_exposure", 1.0);

   _TextureQuad.Render();
   _Texture.Unbind();

   shader.Unbind();
}

void
FrameTexture::StartWrite() const
{
   _FBO.Bind();
   ClearFrameBuffer();
}

}
