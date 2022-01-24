#pragma once

#include "../../../include/Global.h"
#include "GLDebug.h"
#include "GLTypes.h"
#include "Buffers.h"
#include "Texture.h"

//#include <GL/glew.h>
//#include <glm/glm.hpp>

namespace Apeiron {

class HDR
{
public:
  HDR();

  virtual void Init(GLsizei _width, GLsizei _height);

  virtual void WriteTo() const;

  virtual void Finalise() const;

  virtual void ReadFrom(UInt _texture_slot) const;

  const Texture& GetColourBuffer() const { return ColourBuffer; }

protected:
  bool isPointLightShadow;

  Texture ColourBuffer;
  FrameBuffer FBO;
};

}
