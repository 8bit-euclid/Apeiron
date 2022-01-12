#include "../include/Texture.h"

#include "../resources/external/stb_image/stb_image.h"

namespace Apeiron {

Texture::Texture(const GLint _texture_type, const bool _is_fbo_attachment)
  : Type(_texture_type), ID(0), isFrameBufferAttachment(_is_fbo_attachment), LocalBuffer(nullptr), Width(0), Height(0), BitsPerPixel(0)
{
  ASSERT(Type == GL_TEXTURE_2D || Type == GL_TEXTURE_CUBE_MAP, "The passed texture type is currently not supported.")
  GLCall(glGenTextures(1, &ID));
}

Texture::Texture(const std::string& _file_path)
  : Texture(GL_TEXTURE_2D)
{
  ReadFromFile(_file_path, GL_CLAMP_TO_EDGE);
}

Texture::~Texture()
{
  GLCall(glDeleteTextures(1, &ID));
}

void Texture::Init(const GLuint _width, const GLuint _height, const GLint _format, const GLenum _data_type, const GLint _wrap_type,
                   const StaticArray<GLfloat, 4>& _border_colour)
{
  Print<'\0'>("Initialising a ", _width, "x", _height, " texture.");

  Width = _width;
  Height = _height;

  Bind();

  // Common settings
  GLCall(glTexParameteri(Type, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
  GLCall(glTexParameteri(Type, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

  // Type-specific settings
  if(Type == GL_TEXTURE_2D)
  {
    if(_wrap_type == GL_CLAMP_TO_BORDER) GLCall(glTexParameterfv(Type, GL_TEXTURE_BORDER_COLOR, _border_colour.data()));

    GLCall(glTexParameteri(Type, GL_TEXTURE_WRAP_S, _wrap_type));
    GLCall(glTexParameteri(Type, GL_TEXTURE_WRAP_T, _wrap_type));

    GLCall(glTexImage2D(Type, 0, _format, Width, Height, 0, _format, _data_type, isFrameBufferAttachment ? nullptr : LocalBuffer));
  }
  else if(Type == GL_TEXTURE_CUBE_MAP)
  {
    ASSERT(_wrap_type == GL_CLAMP_TO_EDGE, "Only GL_CLAMP_TO_EDGE is currently supported for cube maps.")

    GLCall(glTexParameteri(Type, GL_TEXTURE_WRAP_S, _wrap_type));
    GLCall(glTexParameteri(Type, GL_TEXTURE_WRAP_T, _wrap_type));
    GLCall(glTexParameteri(Type, GL_TEXTURE_WRAP_R, _wrap_type));

    FOR(i, 6) { GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, _format, Width, Height, 0, _format, _data_type, nullptr)); }
  }

  if(!isFrameBufferAttachment) GLCall(glGenerateMipmap(Type));

  Unbind();
}

void Texture::ReadFromFile(const std::string& _file_path, const GLint _wrap_type)
{
  stbi_set_flip_vertically_on_load(1);
  int width, height;
  LocalBuffer = stbi_load(_file_path.c_str(), &width, &height, &BitsPerPixel, 0);

  ASSERT(BitsPerPixel == 3 || BitsPerPixel == 4, "Currently only 3 or 4 bits per pixel are supported.")
  ASSERT(LocalBuffer, "Could not load file \"", _file_path, "\" to texture.")

  Init(width, height, BitsPerPixel == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, _wrap_type);

  stbi_image_free(LocalBuffer);
}

void Texture::Bind(UInt _slot) const
{
  GLCall(glActiveTexture(GL_TEXTURE0 + _slot));
  GLCall(glBindTexture(Type, ID));
}

void Texture::Unbind() const
{
  GLCall(glBindTexture(Type, 0));
}

}

