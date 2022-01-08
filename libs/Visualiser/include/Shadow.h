#pragma once

#include "../../../include/Global.h"
#include "GLDebug.h"
#include "GLTypes.h"
#include "Buffers.h"
#include "Texture.h"

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace Apeiron {

class Shadow
{
public:
  Shadow(const bool _is_point_light);

  ~Shadow();

  virtual void Init(GLsizei _width, GLsizei _height);

  virtual void WriteTo() const;

  virtual void Finalise() const;

  virtual void ReadFrom(UInt _texture_slot) const;

  const Texture& GetDepthMap() const { return DepthMap; }

protected:
  bool isPointLightShadow;

  Texture DepthMap;
  FrameBuffer FBO;
};

}

