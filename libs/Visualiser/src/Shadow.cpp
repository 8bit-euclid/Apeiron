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

#include "../include/Shadow.h"

namespace aprn::vis {

Shadow::Shadow(const bool _is_point_light)
  : DepthMap(_is_point_light ? TextureType::PointDepth : TextureType::DirectionalDepth, true), FBO(), isPointLightShadow(_is_point_light) {}

Shadow::Shadow(Shadow&& _shadow) noexcept
   : DepthMap(std::move(_shadow.DepthMap)), FBO(std::move(_shadow.FBO)), isPointLightShadow(std::move(_shadow.isPointLightShadow)) {}

void Shadow::Init(GLsizei width, GLsizei height)
{
  DepthMap.Init(width, height, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT, isPointLightShadow ? GL_CLAMP_TO_EDGE : GL_CLAMP_TO_BORDER);

  FBO.Init();
  FBO.Bind();

  if(isPointLightShadow) FBO.AttachTexture(GL_DEPTH_ATTACHMENT, DepthMap.ID());
  else FBO.AttachTexture2D(GL_DEPTH_ATTACHMENT, DepthMap.ID());

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

void Shadow::ReadFrom(UInt texture_slot) const
{
  DepthMap.Bind(texture_slot);
}

Shadow&
Shadow::operator=(Shadow&& _shadow) noexcept
{
   DepthMap           = std::move(_shadow.DepthMap);
   FBO                = std::move(_shadow.FBO);
   isPointLightShadow = std::move(_shadow.isPointLightShadow);

   return *this;
}

}

