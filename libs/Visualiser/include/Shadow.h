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
  friend class Shader;
  friend class Visualiser;

public:
  Shadow();

  ~Shadow();

  virtual void Init(GLsizei _width, GLsizei _height);

  virtual void Write() const;

  virtual void Finalise() const;

  virtual void Read(UInt _texture_slot) const;

  const Texture& GetMap() const { return Map; }

protected:
  Texture Map;
  FrameBuffer FBO;
};

}

