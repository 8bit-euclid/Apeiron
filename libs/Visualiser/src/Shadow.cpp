#include "../include/Shadow.h"

namespace Apeiron {

Shadow::Shadow()
  : Map(), FBO()
{

}

Shadow::~Shadow()
{

}

void Shadow::Init(GLsizei _width, GLsizei _height)
{
  Map.Width = _width;
  Map.Height = _height;

  Map.Bind();

  GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, _width, _height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr));

  float border_colour[] = {1.0f, 1.0f, 1.0f, 1.0f};
  GLCall(glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_colour));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

  FBO.Bind();
  FBO.Load(GL_DEPTH_ATTACHMENT, Map.GetID());
  FBO.Draw(GL_NONE);
  FBO.Read(GL_NONE);

  ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Could not initialise frame buffer object.")

  FBO.Unbind();
  Map.Unbind();
}

void Shadow::Write() const
{
  FBO.Bind();
  GLCall(glClear(GL_DEPTH_BUFFER_BIT));
}

void Shadow::Finalise() const
{
  FBO.Unbind();
}

void Shadow::Read(UInt _texture_slot) const
{
  Map.Bind(_texture_slot);
}

}

