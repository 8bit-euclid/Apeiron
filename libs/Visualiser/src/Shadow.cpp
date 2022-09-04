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

Shadow::Shadow(const bool is_point_source)
   : _FBO(), _DepthMap(is_point_source ? TextureType::PointDepth : TextureType::DirectionalDepth, true), _isPointSource(is_point_source) {}

void Shadow::Init(const GLsizei width, const GLsizei height)
{
   _DepthMap.Init(width, height, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT, _isPointSource ? GL_CLAMP_TO_EDGE : GL_CLAMP_TO_BORDER);

   _FBO.Init();
   _FBO.Bind();
   _isPointSource ? _FBO.AttachTexture(GL_DEPTH_ATTACHMENT, _DepthMap.ID()) :
                    _FBO.AttachTexture2D(GL_DEPTH_ATTACHMENT, _DepthMap.ID());
   _FBO.Draw(GL_NONE);
   _FBO.Read(GL_NONE);
   _FBO.Check();
   _FBO.Unbind();
}

void Shadow::StartWrite() const
{
   _FBO.Bind();
   GLCall(glClear(GL_DEPTH_BUFFER_BIT));
}

}

