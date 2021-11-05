#include "../include/Texture.h"
#include "Visualiser/resources/external/stb_image/stb_image.h"

namespace Apeiron {

Texture::Texture(const std::string& _file_path)
  : RendererID(0), FilePath(_file_path), LocalBuffer(nullptr), Width(0), Height(0), BitsPerPixel(0)
{
  stbi_set_flip_vertically_on_load(1);
  LocalBuffer = stbi_load(_file_path.c_str(), &Width, &Height, &BitsPerPixel, 4);

  GLCall(glGenTextures(1, &RendererID));
  GLCall(glBindTexture(GL_TEXTURE_2D, RendererID));

  // Specify texture parameters.
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

  GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, LocalBuffer));
  Unbind();

  if(LocalBuffer) stbi_image_free(LocalBuffer);
}

Texture::~Texture()
{
  GLCall(glDeleteTextures(1, &RendererID));
}

void Texture::Bind(UInt _slot) const
{
  GLCall(glActiveTexture(GL_TEXTURE0 + _slot));
  GLCall(glBindTexture(GL_TEXTURE_2D, RendererID));
}

void Texture::Unbind() const
{
  GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

}

