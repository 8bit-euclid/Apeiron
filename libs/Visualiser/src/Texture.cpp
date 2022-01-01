#include "../include/Texture.h"

#include "../resources/external/stb_image/stb_image.h"

namespace Apeiron {

Texture::Texture()
  : ID(0), LocalBuffer(nullptr), Width(0), Height(0), BitsPerPixel(0)
{
  GLCall(glGenTextures(1, &ID));
}

Texture::Texture(const std::string& _file_path)
  : Texture()
{
  ReadFromFile(_file_path, GL_CLAMP_TO_EDGE);
}

Texture::~Texture()
{
  GLCall(glDeleteTextures(1, &ID));
}

void Texture::Init(const GLuint _width, const GLuint _height, const GLint _format, const GLenum _data_type, const GLint _wrap_type)
{
  Width = _width;
  Height = _height;

  Bind();

//  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, _wrap_type));
//  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, _wrap_type));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

  GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, LocalBuffer));
//  GLCall(glTexImage2D(GL_TEXTURE_2D, 0, _format, Width, Height, 0, _format, _data_type, LocalBuffer));

  GLCall(glGenerateMipmap(GL_TEXTURE_2D));

  Unbind();
}

void Texture::ReadFromFile(const std::string& _file_path, const GLint _wrap_type)
{
  stbi_set_flip_vertically_on_load(1);
  int width, height;
  LocalBuffer = stbi_load(_file_path.c_str(), &width, &height, &BitsPerPixel, 0);

  ASSERT(LocalBuffer, "Could not load file \"", _file_path, "\" to texture.")

  Init(width, height, GL_RGBA, GL_UNSIGNED_BYTE, _wrap_type);

  stbi_image_free(LocalBuffer);
}

void Texture::Bind(UInt _slot) const
{
  GLCall(glActiveTexture(GL_TEXTURE0 + _slot));
  GLCall(glBindTexture(GL_TEXTURE_2D, ID));
}

void Texture::Unbind() const
{
  GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

}

