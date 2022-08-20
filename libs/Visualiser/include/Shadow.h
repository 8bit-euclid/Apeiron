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

