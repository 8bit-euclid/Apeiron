#pragma once

#include "../../../include/Global.h"
#include "../../LinearAlgebra/include/Vector.h"
#include "GLDebug.h"
#include "GLTypes.h"

#include <GL/glew.h>

namespace Apeiron {

class Texture
{
  friend class Model;
  friend class Shadow;

public:
  Texture(const GLint _texture_type, const bool _is_fbo_attachment = false);

  Texture(const std::string& _file_path);

  ~Texture();

  void Init(const GLuint _width, const GLuint _height, const GLint _internal_format, const GLenum _format, const GLenum _data_type, const GLint _wrap_type,
            const SVector4<GLfloat>& _border_colour = {1.0f, 1.0f, 1.0f, 1.0f});

  void ReadFromFile(const std::string& _file_path, const GLint _wrap_type);

  void Bind(UInt _slot = 0) const;

  void Unbind() const;

  inline UInt GetID() const { return ID; }

  const int& GetWidth() const { return Width; }

  const int& GetHeight() const { return Height; }

private:
  GLint Type;
  UInt ID;
  bool isFrameBufferAttachment;
  UChar* LocalBuffer;

  int Width;
  int Height;
  int BitsPerPixel;
};

}
