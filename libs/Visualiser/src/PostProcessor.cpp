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
   EXIT("Fix")
//   _ScreenQuad.Init();
}

void
PostProcessor::InitMultiSampledBuffers()
{
   auto& fbo      = _MultiSampledImage.FBO;
   auto& rbo      = _MultiSampledImage.RBO;
   auto& textures = _MultiSampledImage.Textures;

   // Initialise and bind an MSAA frame buffer.
   fbo.Init(true);
   fbo.Bind();

   // Add MSAA HDR buffer and bright buffer textures.
   textures.emplace("HDR"   , Texture(TextureType::Diffuse, true));
   textures.emplace("Bright", Texture(TextureType::Diffuse, true));

   // Attach MSAA textures as the frame buffer's colour buffers.
   size_t i = 0;
   DArray<GLenum> attachments;
   for (std::string name : { "HDR", "Bright" }) // Note: must be attached to the frame buffer in this order.
   {
      auto& texture = textures.at(name);
      attachments.push_back(GL_COLOR_ATTACHMENT0 + i++);

      texture.Init(_Width, _Height, GL_RGBA16F, GL_RGBA, GL_FLOAT, GL_CLAMP_TO_EDGE, _SampleCount);
      fbo.AttachTexture2D(attachments.back(), texture.ID());
   }
   fbo.Draw(attachments);

   // Attach a render buffer as the frame buffer's depth/stencil buffer.
   rbo.Init(_SampleCount);
   rbo.Allocate(GL_DEPTH24_STENCIL8, _Width, _Height);
   fbo.AttachRenderBuffer(GL_DEPTH_STENCIL_ATTACHMENT, rbo.ID());

   fbo.Unbind();
}

void
PostProcessor::InitResolvedBuffers()
{
   auto& fbo      = _ResolvedImage.FBO;
   auto& textures = _ResolvedImage.Textures;

   // Initialise and bind a non-MSAA frame buffer.
   fbo.Init(false);
   fbo.Bind();

   // Add non-MSAA HDR buffer and bright buffer textures.
   textures.emplace("HDR"   , Texture(TextureType::Diffuse, true));
   textures.emplace("Bright", Texture(TextureType::Diffuse, true));

   // Attach textures as frame buffer's colour buffers.
   size_t i = 0;
   DArray<GLenum> attachments;
   for (std::string name : { "HDR", "Bright" }) // Note: must be attached to the frame buffer in this order.
   {
      auto& texture = textures.at(name);
      attachments.push_back(GL_COLOR_ATTACHMENT0 + i++);

      texture.Init(_Width, _Height, GL_RGBA16F, GL_RGBA, GL_FLOAT, GL_CLAMP_TO_EDGE);
      fbo.AttachTexture2D(attachments.back(), texture.ID());
   }
   fbo.Draw(attachments);

   fbo.Unbind();
}

void
PostProcessor::InitBlurBuffers()
{
   // Configure ping-pong buffers
   for(auto name : { "Ping", "Pong" })
   {
      auto& buffer   = _BlurBuffers[name]; // Note: emplacement is intended here.
      auto& fbo      = buffer.FBO;
      auto& textures = buffer.Textures;

      // Initialise and bind a non-MSAA frame buffer.
      fbo.Init(false);
      fbo.Bind();

      // Attach a single colour buffer texture.
      textures.emplace(_Default, Texture(TextureType::Diffuse, true));
      auto& texture = textures.at(_Default);
      texture.Init(_Width, _Height, GL_RGBA16F, GL_RGBA, GL_FLOAT, GL_CLAMP_TO_EDGE);
      fbo.AttachTexture2D(GL_COLOR_ATTACHMENT0, texture.ID());
      fbo.Draw(GL_COLOR_ATTACHMENT0);

      fbo.Unbind();
   }
}

void
PostProcessor::StartWrite() const
{
   _MultiSampledImage.FBO.Bind();
   ClearFrameBuffer();
}

void
PostProcessor::StopWrite() const
{
   ResolveFrameBuffer();
   _MultiSampledImage.FBO.Unbind();
}

void
PostProcessor::ResolveFrameBuffer() const
{
   glBindFramebuffer(GL_READ_FRAMEBUFFER, _MultiSampledImage.FBO.ID());
   glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _ResolvedImage.FBO.ID());
   FOR(i, 2)
   {
      const GLenum attachment = GL_COLOR_ATTACHMENT0 + i;
      glReadBuffer(attachment);
      glDrawBuffer(attachment);
      glBlitFramebuffer(0, 0, _Width, _Height, 0, 0, _Width, _Height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
   }
}

void
PostProcessor::Render()
{
   GLCall(glClearColor(1.0f, 1.0f, 1.0f, 1.0f))
   GLCall(glClear(GL_COLOR_BUFFER_BIT))
   GLCall(glDisable(GL_DEPTH_TEST))

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
      auto& texture = i == 0 ? _ResolvedImage.Textures.at("Bright") : pong->Textures.at(_Default);
      ping->FBO.Bind();
      texture.Bind();

      blur_shader.UseTexture(texture, "u_texture", 0);
      blur_shader.SetUniform1i("u_horizontal", horizontal);
      EXIT("Fix")
//      _ScreenQuad.Render();

      texture.Unbind();
      ping->FBO.Unbind();

      horizontal = !horizontal;
   }
   blur_shader.Unbind();

   // Now blend the HDR and blurred bright textures.
   auto& hdr_texture  = _ResolvedImage.Textures.at("HDR");
   auto& blur_texture = buffers[!horizontal]->Textures.at(_Default);
   auto& blend_shader = _Shaders.at("Blend");
   blend_shader.Bind();

   // Load textures.
   blend_shader.UseTexture(hdr_texture , "u_hdr_texture" , 0);
   blend_shader.UseTexture(blur_texture, "u_blur_texture", 1);
   blend_shader.SetUniform1f("u_exposure", 1.0);

   EXIT("Fix")
//   _ScreenQuad.Render();

   hdr_texture.Unbind();
   blur_texture.Unbind();
   blend_shader.Unbind();
}

}
