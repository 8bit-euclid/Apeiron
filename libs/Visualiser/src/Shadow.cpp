#include "../include/Shadow.h"

namespace Apeiron {

Shadow::Shadow(const bool _is_point_light)
  : isPointLightShadow(_is_point_light), DepthMap(_is_point_light ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D, true), FBO()
{

}

Shadow::~Shadow()
{

}

void Shadow::Init(GLsizei _width, GLsizei _height)
{
  DepthMap.Init(_width, _height, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT, isPointLightShadow ? GL_CLAMP_TO_EDGE : GL_CLAMP_TO_BORDER);

  FBO.Bind();

  if(isPointLightShadow) FBO.AttachTexture(GL_DEPTH_ATTACHMENT, DepthMap.GetID());
  else FBO.AttachTexture2D(GL_DEPTH_ATTACHMENT, DepthMap.GetID());

  FBO.Draw(GL_NONE);
  FBO.Read(GL_NONE);

  ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Could not initialise frame buffer object.")

  FBO.Unbind();
}

void Shadow::WriteTo() const
{
  FBO.Bind();
  GLCall(glClear(GL_DEPTH_BUFFER_BIT));
}

void Shadow::Finalise() const
{
  DepthMap.Unbind();
  FBO.Unbind();
}

void Shadow::ReadFrom(UInt _texture_slot) const
{
  DepthMap.Bind(_texture_slot);
}

}

