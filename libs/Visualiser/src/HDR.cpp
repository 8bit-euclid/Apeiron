#include "../include/Shadow.h"
#include "include/HDR.h"


namespace aprn::vis {

HDR::HDR()
  : ColourBuffer(TextureType::Diffuse, true), FBO()
{

}

void
HDR::Init(GLsizei width, GLsizei height)
{
//  ColourBuffer.Init(width, height, GL_RGBA16F, GL_RGBA, GL_FLOAT, GL_CLAMP_TO_BORDER);
//
//  FBO.Bind();
//
//  FBO.AttachTexture2D(GL_COLOR_ATTACHMENT0, ColourBuffer.GetID());
//  FBO.Draw(GL_NONE);
//  FBO.Read(GL_NONE);
//
//  ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Could not initialise frame buffer object.")
//
//  FBO.Unbind();
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
HDR::ReadFrom(UInt _texture_slot) const
{

}

}