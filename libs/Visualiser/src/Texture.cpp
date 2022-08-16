#include "../include/Texture.h"
#include "../resources/external/stb_image/stb_image.h"

#include "FileManager/include/FileSystem.h"

namespace aprn::vis {

/***************************************************************************************************************************************************************
* TEXTURE CLASS IMPLEMENTATION
***************************************************************************************************************************************************************/

/***************************************************************************************************************************************************************
* Public Interface
***************************************************************************************************************************************************************/
Texture::Texture(const TextureType _type, const std::string& file_path)
   : Texture(_type, false)
{
   Read(file_path, GL_CLAMP_TO_EDGE);
}

Texture::Texture(const TextureType _type, const bool _is_fbo_attachment)
  : ID(0), Width(0), Height(0), BitsPerPixel(0), LocalBuffer(nullptr), Type(_type), isFrameBufferAttachment(_is_fbo_attachment)
{
  GLCall(glGenTextures(1, &ID));
}

Texture::Texture(Texture&& _texture) noexcept
   : ID(_texture.ID), Width(std::move(_texture.Width)), Height(std::move(_texture.Height)), BitsPerPixel(std::move(_texture.BitsPerPixel)),
     LocalBuffer(std::move(_texture.LocalBuffer)), MapScale(std::move(_texture.MapScale)), Type(std::move(_texture.Type)),
     isFrameBufferAttachment(std::move(_texture.isFrameBufferAttachment))
{
   _texture.ID = 0;
}

Texture::~Texture() { Delete(); }

void
Texture::Init(const GLuint _width, const GLuint _height, const GLint _internal_format, const GLenum _format, const GLenum _data_type,
                   const GLint wrap_type, const SVector4<GLfloat>& _border_colour)
{
  Width = _width;
  Height = _height;
  const auto opengl_type = GetOpenGLType(Type);

  Bind();

  // Common settings
  GLCall(glTexParameteri(opengl_type, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
  GLCall(glTexParameteri(opengl_type, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

  // OpenGL type-specific settings
  if(opengl_type == GL_TEXTURE_2D)
  {
    if(wrap_type == GL_CLAMP_TO_BORDER) { GLCall(glTexParameterfv(opengl_type, GL_TEXTURE_BORDER_COLOR, _border_colour.data())); }

    GLCall(glTexParameteri(opengl_type, GL_TEXTURE_WRAP_S, wrap_type));
    GLCall(glTexParameteri(opengl_type, GL_TEXTURE_WRAP_T, wrap_type));

    GLCall(glTexImage2D(opengl_type, 0, _internal_format, Width, Height, 0, _format, _data_type, isFrameBufferAttachment ? nullptr : LocalBuffer.get()));
  }
  else if(opengl_type == GL_TEXTURE_CUBE_MAP)
  {
    ASSERT(wrap_type == GL_CLAMP_TO_EDGE, "Only GL_CLAMP_TO_EDGE is currently supported for cube maps.")

    GLCall(glTexParameteri(opengl_type, GL_TEXTURE_WRAP_S, wrap_type));
    GLCall(glTexParameteri(opengl_type, GL_TEXTURE_WRAP_T, wrap_type));
    GLCall(glTexParameteri(opengl_type, GL_TEXTURE_WRAP_R, wrap_type));

    FOR(i, 6) { GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, _internal_format, Width, Height, 0, _format, _data_type, nullptr)); }
  }

  if(!isFrameBufferAttachment) GLCall(glGenerateMipmap(opengl_type));

  Unbind();
}

void
Texture::Read(const std::string& file_path, const GLint wrap_type)
{
   stbi_set_flip_vertically_on_load(1);
   int width, height;
   LocalBuffer.reset(stbi_load(file_path.c_str(), &width, &height, &BitsPerPixel, 0));
   ASSERT(LocalBuffer, "Could not load file \"", file_path, "\" to texture.")

   std::pair<GLint, GLenum> format = BitsPerPixel == 2 ? std::make_pair(GL_SRGB, GL_RG) :
                                     BitsPerPixel == 3 ? std::make_pair(GL_SRGB, GL_RGB) :
                                     BitsPerPixel == 4 ? std::make_pair(GL_SRGB_ALPHA, GL_RGBA) :
                                     throw "Currently only 2, 3, or 4 bits per pixel are supported. Got " + ToStr(BitsPerPixel) + ".";
   Init(width, height, format.first, format.second, GL_UNSIGNED_BYTE, wrap_type);
   stbi_image_free(LocalBuffer.get());
}

void
Texture::Bind(UInt _slot) const
{
  GLCall(glActiveTexture(GL_TEXTURE0 + _slot));
  GLCall(glBindTexture(GetOpenGLType(Type), ID));
}

void
Texture::Unbind() const { GLCall(glBindTexture(GetOpenGLType(Type), 0)); }

void
Texture::Delete() { GLCall(glDeleteTextures(1, &ID)); }

Texture&
Texture::operator=(Texture&& _texture) noexcept
{
   ID                      = _texture.ID;
   _texture.ID = 0;

   Width                   = std::move(_texture.Width);
   Height                  = std::move(_texture.Height);
   BitsPerPixel            = std::move(_texture.BitsPerPixel);
   LocalBuffer             = std::move(_texture.LocalBuffer);
   MapScale                = std::move(_texture.MapScale);
   Type                    = std::move(_texture.Type);
   isFrameBufferAttachment = std::move(_texture.isFrameBufferAttachment);

   return *this;
}

/***************************************************************************************************************************************************************
* TEXTURE STAND-ALONE FUNCTIONS
***************************************************************************************************************************************************************/
GLint
GetOpenGLType(TextureType _type)
{
   if(_type == TextureType::Diffuse || _type == TextureType::Normal ||
      _type == TextureType::Displacement || _type == TextureType::DirectionalDepth) return GL_TEXTURE_2D;
   else if(_type == TextureType::PointDepth) return GL_TEXTURE_CUBE_MAP;
   else EXIT("Unrecongnised texture type.")
}

std::string
GetTextureName(const std::string& _material, const std::string& item, size_t index, size_t _resolution)
{
   ASSERT(_resolution == 1 || _resolution == 2 || _resolution == 4 || _resolution == 8, "Only 1K, 2K, 4K, and 8K resolutions are recognised.")
   return _material + "/" + (item.empty() ? "" : item + "/") + ToStr(index) + "/" + ToStr(_resolution) + "K";
}

std::string
GetTextureTypeString(TextureType _type)
{
   switch(_type)
   {
      case TextureType::Diffuse:          return "Diffuse";
      case TextureType::Normal:           return "Normal";
      case TextureType::Displacement:     return "Displacement";
      case TextureType::Roughness:        return "Roughness";
      case TextureType::AmbientOcclusion: return "AmbientOcclusion";
      case TextureType::DirectionalDepth: return "DirectionalDepth";
      case TextureType::PointDepth:       return "PointDepth";
      default: EXIT("Unrecognised texture type.")
   }
}

TextureType
GetTextureType(const std::string& _type_string)
{
   if(_type_string == "Diffuse")               return TextureType::Diffuse;
   else if(_type_string == "Normal")           return TextureType::Normal;
   else if(_type_string == "Displacement")     return TextureType::Displacement;
   else if(_type_string == "Roughness")        return TextureType::Roughness;
   else if(_type_string == "AmbientOcclusion") return TextureType::AmbientOcclusion;
   else if(_type_string == "DirectionalDepth") return TextureType::DirectionalDepth;
   else if(_type_string == "PointDepth")       return TextureType::PointDepth;
   else EXIT("Unrecognised texture type.")
}

std::string
GetTextureUniformString(TextureType _type)
{
   switch(_type)
   {
      case TextureType::Diffuse:      return "diffuse_map";
      case TextureType::Normal:       return "normal_map";
      case TextureType::Displacement: return "displacement_map";
      default: EXIT("The given texture type does not have an associate uniform string.")
   }
}

std::string
GetTextureUniformString(const std::string& _type_string) { return GetTextureUniformString(GetTextureType(_type_string)); }

std::string
GetTextureFileDirectory(const std::string& name) { return "libs/Visualiser/resources/textures/" + name + "/"; }

std::optional<std::string>
GetTextureFilePath(const std::string& _file_directory, TextureType _type)
{
   const auto& texture_str = GetTextureTypeString(_type);
   std::string file_path = _file_directory + texture_str;

   if(EnumToInt(_type) <= EnumToInt(TextureType::AmbientOcclusion))
   {
      // Try finding a .png file first, failing which, find a .jpg file
      for(const auto& ext : {".png", ".jpg"})
         if(flmgr::FileExists(file_path + ext))
         {
            file_path.append(ext);
            return std::make_optional<std::string>(file_path);
         }
      return std::nullopt;
   }
   else EXIT("The texture type ", texture_str, " cannot be read from file.")
}

}

