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

#include "../include/PostProcessor.h"
#include "../include/ModelFactory.h"
#include "../include/Window.h"

namespace aprn::vis {

void
PostProcessor::Init(const UInt width, const UInt height, bool is_hdr)
{
   _isHDR  = is_hdr;
   _Width  = width;
   _Height = height;
   const bool  high_prec       = false;
   const GLint internal_format = _isHDR ? (high_prec ? GL_RGBA32F : GL_RGBA16F) : GL_RGBA;

   _Output.FBO.Init();
   _Output.FBO.Bind();

   // Add colour buffer and bloom buffer textures.
   _Output.Textures.emplace("HDR"   , Texture(TextureType::Diffuse, true));
   _Output.Textures.emplace("Bright", Texture(TextureType::Diffuse, true));

   // Attach textures as the main frame buffer's colour buffers.
   size_t i = 0;
   DArray<GLenum> attachments;
   for(std::string texture_name : {"HDR", "Bright"}) // Note: must be attached to the frame buffer in this order.
   {
      auto& texture = _Output.Textures.at(texture_name);
      attachments.push_back(GL_COLOR_ATTACHMENT0 + i++);

      texture.Init(_Width, _Height, internal_format, GL_RGBA, GL_FLOAT, GL_CLAMP_TO_EDGE);
      _Output.FBO.AttachTexture2D(attachments.back(), texture.ID());
   }
   _Output.FBO.Draw(attachments);

   // Attach a render buffer as the frame buffer's depth/stencil buffer.
   _Output.RBO.Init();
   _Output.RBO.Allocate(GL_DEPTH24_STENCIL8, _Width, _Height);
   _Output.FBO.AttachRenderBuffer(GL_DEPTH_STENCIL_ATTACHMENT, _Output.RBO.ID());

   _Output.FBO.Unbind();

   // Configure ping-pong buffers
   for(const auto name : {"Ping", "Pong"})
   {
      auto& buffer = _FrameImages[name]; // Note: emplacement is intended here.

      buffer.FBO.Init();
      buffer.FBO.Bind();

      // Add a single colour buffer.
      buffer.Textures.emplace("Image", Texture(TextureType::Diffuse, true));
      buffer.Textures.at("Image").Init(_Width, _Height, internal_format, GL_RGBA, GL_FLOAT, GL_CLAMP_TO_EDGE);
      buffer.FBO.AttachTexture2D(GL_COLOR_ATTACHMENT0, buffer.Textures.at("Image").ID());
      buffer.FBO.Draw(GL_COLOR_ATTACHMENT0);

      buffer.FBO.Unbind();
   }

   // Create rectangular screen-filling quad to render texture to.
   _ScreenQuad = ModelFactory::ScreenQuad();
   _ScreenQuad.Init();

   // Load post-processing shaders.
   for(std::string shader : {"HDR", "Blur"})
      _Shaders.emplace(shader, Shader::_ShaderDirectory + shader + ".glsl");
}

void
PostProcessor::Render()
{
   GLCall(glClearColor(1.0f, 1.0f, 1.0f, 1.0f));
   GLCall(glClear(GL_COLOR_BUFFER_BIT));
   GLCall(glDisable(GL_DEPTH_TEST));

   // Apply Gaussian blur.
   DArray<FrameImage*> buffers = {&_FrameImages.at("Ping"),
                                  &_FrameImages.at("Pong")};
   const size_t n_passes = 6;
   bool horizontal = true;
   auto& blur_shader = _Shaders.at("Blur");
   blur_shader.Bind();
   for(size_t i = 0; i < n_passes; i++)
   {
      auto ping = buffers[ horizontal];
      auto pong = buffers[!horizontal];

      // Bind the FBO of the Ping buffer and the texture of the Pong buffer.
      auto& texture = i == 0 ? _Output.Textures.at("Bright") : pong->Textures.at("Image");
      ping->FBO.Bind();
      texture.Bind();

      blur_shader.UseTexture(texture, "u_texture", 0);
      blur_shader.SetUniform1i("u_horizontal", horizontal);
      _ScreenQuad.Render();

      texture.Unbind();
      ping->FBO.Unbind();

      horizontal = !horizontal;
   }
   blur_shader.Unbind();

//   auto& texture = _Textures.at("HDR");
//   auto& texture = _Output.Textures.at("Bright");
   auto& texture = buffers[!horizontal]->Textures.at("Image");

   auto& hdr_shader = _Shaders.at("HDR");
   hdr_shader.Bind();

   hdr_shader.UseTexture(texture, "u_texture", 0);
   hdr_shader.SetUniform1i("u_is_hdr"  , _isHDR);
   hdr_shader.SetUniform1f("u_exposure", 1.0);

   _ScreenQuad.Render();
   texture.Unbind();

   hdr_shader.Unbind();
}

void
PostProcessor::StartWrite() const
{
   _Output.FBO.Bind();
   ClearFrameBuffer();
}

}
