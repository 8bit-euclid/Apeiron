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