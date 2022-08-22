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

#pragma once

#include "../../../include/Global.h"
#include "GLDebug.h"
#include "GLTypes.h"
#include "Buffers.h"
#include "Texture.h"

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace aprn::vis {

class Shadow
{
 public:
   Shadow(const bool _is_point_light);

   Shadow(const Shadow& _shadow) = delete;

   Shadow(Shadow&& _shadow) noexcept;

   virtual void Init(GLsizei width, GLsizei height);

   virtual void WriteTo() const;

   virtual void Finalise() const;

   virtual void ReadFrom(UInt texture_slot) const;

   inline const Texture& GetDepthMap() const { return DepthMap; }

   Shadow& operator=(const Shadow& _shadow) = delete;

   Shadow& operator=(Shadow&& _shadow) noexcept;

 protected:
   friend class Light;

   Texture DepthMap;
   FrameBuffer FBO;
   bool isPointLightShadow;
};

}

