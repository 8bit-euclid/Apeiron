#include "../include/HDR.h"

namespace aprn::vis {

HDR::HDR()
  : _ColourBuffer(TextureType::Diffuse, true), _FBO() {}

void
HDR::Init(const GLsizei width, const GLsizei height)
{
  _ColourBuffer.Init(width, height, GL_RGBA16F, GL_RGBA, GL_FLOAT, GL_CLAMP_TO_BORDER);

  _FBO.Bind();

  _FBO.AttachTexture2D(GL_COLOR_ATTACHMENT0, _ColourBuffer.ID());
  _FBO.Draw(GL_NONE);
  _FBO.Read(GL_NONE);

  ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Could not initialise frame buffer object.")

  _FBO.Unbind();
}

void
HDR::WriteTo() const
{

}

void
HDR::Finalise() const
{

}

void
HDR::ReadFrom(const UInt texture_slot) const
{

}

}