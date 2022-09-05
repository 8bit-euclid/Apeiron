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
PostProcessor::Init(const UInt width, const UInt height)
{
   // Set member variables.
   _Width  = width;
   _Height = height;

   // Load required post-processing shaders.
   for(std::string shader : {"HDR", "Blur", "Blend"}) _Shaders.emplace(shader, Shader::Directory + shader + ".glsl");

   // Initialise required post-processing buffers.
   InitMultiSampledBuffers();
   InitResolvedBuffers();
   InitBlurBuffers();

   // Create rectangular screen-filling quad (a square in normalised device coordinates) to draw each texture to.
   _ScreenQuad = ModelFactory::ScreenQuad();
   _ScreenQuad.Init();
}

void
PostProcessor::InitMultiSampledBuffers()
{
   _MultiSampledImage.FBO.Init(true);
   _MultiSampledImage.FBO.Bind();

   // Add colour buffer and bloom buffer textures.
   _MultiSampledImage.Textures.emplace("HDR"   , Texture(TextureType::Diffuse, true));
   _MultiSampledImage.Textures.emplace("Bright", Texture(TextureType::Diffuse, true));

   // Attach textures as the main frame buffer's colour buffers.
   size_t i = 0;
   DArray<GLenum> attachments;
   for(std::string texture_name : {"HDR", "Bright"}) // Note: must be attached to the frame buffer in this order.
   {
      auto& texture = _MultiSampledImage.Textures.at(texture_name);
      attachments.push_back(GL_COLOR_ATTACHMENT0 + i++);

      texture.Init(_Width, _Height, GL_RGBA16F, GL_RGBA, GL_FLOAT, GL_CLAMP_TO_EDGE, 24);
      _MultiSampledImage.FBO.AttachTexture2D(attachments.back(), texture.ID());
   }
   _MultiSampledImage.FBO.Draw(attachments);

   // Attach a render buffer as the frame buffer's depth/stencil buffer.
   _MultiSampledImage.RBO.Init(true);
   _MultiSampledImage.RBO.Allocate(GL_DEPTH24_STENCIL8, _Width, _Height);
   _MultiSampledImage.FBO.AttachRenderBuffer(GL_DEPTH_STENCIL_ATTACHMENT, _MultiSampledImage.RBO.ID());

   _MultiSampledImage.FBO.Unbind();
}

void
PostProcessor::InitResolvedBuffers()
{
   _ResolvedImage.FBO.Init(false);
   _ResolvedImage.FBO.Bind();

   // Add colour buffer and bloom buffer textures.
   _ResolvedImage.Textures.emplace(_Default, Texture(TextureType::Diffuse, true));

//   // Attach textures as the main frame buffer's colour buffers.
//   size_t i = 0;
//   DArray<GLenum> attachments;
//   for(std::string texture_name : {"HDR", "Bright"}) // Note: must be attached to the frame buffer in this order.
//   {
//      auto& texture = _ResolvedImage.Textures.at(texture_name);
//      attachments.push_back(GL_COLOR_ATTACHMENT0 + i++);
//
//      texture.Init(_Width, _Height, GL_RGBA16F, GL_RGBA, GL_FLOAT, GL_CLAMP_TO_EDGE, 24);
//      _ResolvedImage.FBO.AttachTexture2D(attachments.back(), texture.ID());
//   }
//   _ResolvedImage.FBO.Draw(attachments);
//
//   // Attach a render buffer as the frame buffer's depth/stencil buffer.
//   _ResolvedImage.RBO.Init(true);
//   _ResolvedImage.RBO.Allocate(GL_DEPTH24_STENCIL8, _Width, _Height);
//   _ResolvedImage.FBO.AttachRenderBuffer(GL_DEPTH_STENCIL_ATTACHMENT, _ResolvedImage.RBO.ID());
//
//   _ResolvedImage.FBO.Unbind();
}

void
PostProcessor::InitBlurBuffers()
{
   // Configure ping-pong buffers
   for(auto name : {"Ping", "Pong"})
   {
      auto& buffer = _BlurBuffers[name]; // Note: emplacement is intended here.

      buffer.FBO.Init();
      buffer.FBO.Bind();

      // Add a single colour buffer.
      buffer.Textures.emplace(_Default, Texture(TextureType::Diffuse, true));
      buffer.Textures.at(_Default).Init(_Width, _Height, GL_RGBA16F, GL_RGBA, GL_FLOAT, GL_CLAMP_TO_EDGE);
      buffer.FBO.AttachTexture2D(GL_COLOR_ATTACHMENT0, buffer.Textures.at(_Default).ID());
      buffer.FBO.Draw(GL_COLOR_ATTACHMENT0);

      buffer.FBO.Unbind();
   }
}

void
PostProcessor::StartWrite() const
{
   _MultiSampledImage.FBO.Bind();
   ClearFrameBuffer();
}

void
PostProcessor::StopWrite() const  { _MultiSampledImage.FBO.Unbind(); }

void
PostProcessor::Render()
{
   GLCall(glClearColor(1.0f, 1.0f, 1.0f, 1.0f));
   GLCall(glClear(GL_COLOR_BUFFER_BIT));
   GLCall(glDisable(GL_DEPTH_TEST));

   // Apply Gaussian blur.
   DArray<FrameImage*> buffers = {&_BlurBuffers.at("Ping"),
                                  &_BlurBuffers.at("Pong")};
   const size_t n_passes = 6;
   bool horizontal = true;
   auto& blur_shader = _Shaders.at("Blur");
   blur_shader.Bind();
   for(size_t i = 0; i < n_passes; i++)
   {
      auto ping = buffers[ horizontal];
      auto pong = buffers[!horizontal];

      // Bind the FBO of the Ping buffer and the texture of the Pong buffer.
      auto& texture = i == 0 ? _MultiSampledImage.Textures.at("Bright") : pong->Textures.at(_Default);
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

   auto& hdr_texture  = _MultiSampledImage.Textures.at("HDR");
   auto& blur_texture = buffers[!horizontal]->Textures.at(_Default);

//   auto& shader = _Shaders.at("HDR");
   auto& shader = _Shaders.at("Blend");
   shader.Bind();

//   shader.UseTexture(texture, "u_texture", 0);
   shader.UseTexture(hdr_texture , "u_hdr_texture", 0);
   shader.UseTexture(blur_texture, "u_blur_texture", 1);
   shader.SetUniform1f("u_exposure", 1.0);

   _ScreenQuad.Render();

   hdr_texture.Unbind();
   blur_texture.Unbind();
   shader.Unbind();
}

}
