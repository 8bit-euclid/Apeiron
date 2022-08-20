#pragma once

#include "../../../include/Global.h"
#include "GLDebug.h"
#include "GLTypes.h"
#include "Buffers.h"
#include "Texture.h"

//#include <GL/glew.h>
//#include <glm/glm.hpp>

namespace aprn::vis {

class HDR
{
 public:
   HDR();

   void Init(const GLsizei width, const GLsizei height);

   void WriteTo() const;

   void Finalise() const;

   void ReadFrom(const UInt texture_slot) const;

   const Texture& ColourBuffer() const { return _ColourBuffer; }

 protected:
   Texture     _ColourBuffer;
   FrameBuffer _FBO;
};

}
