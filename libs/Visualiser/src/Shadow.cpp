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

Shadow::Shadow(const bool is_point_light)
   : _DepthMap(is_point_light ? TextureType::PointDepth : TextureType::DirectionalDepth, true), _FBO(), _isPointLightShadow(is_point_light) {}

Shadow::Shadow(Shadow&& shadow) noexcept
   : _DepthMap(std::move(shadow._DepthMap)), _FBO(std::move(shadow._FBO)), _isPointLightShadow(std::move(shadow._isPointLightShadow)) {}

void Shadow::Init(const GLsizei width, const GLsizei height)
{
   _DepthMap.Init(width, height, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT, _isPointLightShadow ? GL_CLAMP_TO_EDGE : GL_CLAMP_TO_BORDER);

   _FBO.Init();
   _FBO.Bind();

   _isPointLightShadow ? _FBO.AttachTexture(GL_DEPTH_ATTACHMENT, _DepthMap.ID()) :
   _FBO.AttachTexture2D(GL_DEPTH_ATTACHMENT, _DepthMap.ID());

   _FBO.Draw(GL_NONE);
   _FBO.Read(GL_NONE);

   ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Could not initialise frame buffer object.")

   _FBO.Unbind();
}

void Shadow::WriteTo() const
{
   _FBO.Bind();
   GLCall(glClear(GL_DEPTH_BUFFER_BIT));
}

void Shadow::Finalise() const
{
   _DepthMap.Unbind();
   _FBO.Unbind();
}

void Shadow::ReadFrom(UInt texture_slot) const
{
   _DepthMap.Bind(texture_slot);
}

Shadow&
Shadow::operator=(Shadow&& shadow) noexcept
{
   _DepthMap           = std::move(shadow._DepthMap);
   _FBO                = std::move(shadow._FBO);
   _isPointLightShadow = std::move(shadow._isPointLightShadow);
   return *this;
}

}

