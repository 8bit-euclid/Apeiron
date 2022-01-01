#pragma once

#include <GL/glew.h>
#include "../../../include/Global.h"
#include "GLDebug.h"
#include "GLTypes.h"

//#pragma GCC diagnostic push
//#pragma GCC diagnostic ignored "-Wold-style-cast"
//#pragma GCC diagnostic ignored "-Wcast-qual"
//#include "Visualiser/resources/external/stb_image/stb_image.h"
//#pragma GCC diagnostic pop

namespace Apeiron {

class Texture
{
  friend class Model;
  friend class Shadow;

public:
  Texture();

  Texture(const std::string& _file_path);

  ~Texture();

  void Init(const GLuint _width, const GLuint _height, const GLint _format, const GLenum _data_type, const GLint _wrap_type);

  void ReadFromFile(const std::string& _file_path, const GLint _wrap_type);

  void Bind(UInt _slot = 0) const;

  void Unbind() const;

  inline UInt GetID() const { return ID; }

  inline int GetWidth() const { return Width; }

  inline int GetHeight() const { return Height; }

private:
  UInt ID;
  UChar* LocalBuffer;

  int Width;
  int Height;
  int BitsPerPixel;
};

}
