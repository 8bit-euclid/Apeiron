#pragma once

#include "../../../include/Global.h"
#include "../../LinearAlgebra/include/Vector.h"
#include "GLDebug.h"
#include "GLTypes.h"

#include <memory>
#include <optional>
#include <GL/glew.h>

namespace aprn::vis {

enum class TextureType
{
   /** Texture file types */
   Diffuse,
   Normal,
   Height,
   Roughness,
   AmbientOcclusion,

   /** Frame buffer object types */
   DirectionalDepth,
   PointDepth
};

class Texture
{
 public:
   Texture(const TextureType _type, const std::string& _file_path);

   Texture(const TextureType _type, const bool _is_fbo_attachment);

   Texture(const Texture& _texture) = delete;

   Texture(Texture&& _texture) noexcept;

   ~Texture();

   void Init(const GLuint _width, const GLuint _height, const GLint _internal_format, const GLenum _format, const GLenum _data_type, const GLint _wrap_type,
             const SVector4<GLfloat>& _border_colour = {1.0f, 1.0f, 1.0f, 1.0f});

   void Read(const std::string& _file_path, const GLint _wrap_type);

   void Bind(UInt _slot = 0) const;

   void Unbind() const;

   void Delete();

   inline UInt GetID() const { return ID; }

   inline const int& GetWidth() const { return Width; }

   inline const int& GetHeight() const { return Height; }

   inline void SetMapScale(Float _scale) { MapScale.emplace(_scale); }

   Texture& operator=(const Texture& _texture) = delete;

   Texture& operator=(Texture&& _texture) noexcept;

 private:
   friend class Model;
   friend class Shadow;

   GLint
   GetOpenGLType() const;

   std::string
   GetUniformString() const;

   UInt                 ID;
   int                  Width;
   int                  Height;
   int                  BitsPerPixel;
   SPtr<UChar>          LocalBuffer;
   std::optional<Float> MapScale;
   TextureType          Type;
   bool                 isFrameBufferAttachment;
};

std::string
GetTextureName(const std::string& _material, const std::string& _item, size_t _index, size_t _resolution);

std::string
GetTextureTypeString(TextureType _type);

TextureType
GetTextureType(const std::string& _type_string);

std::string
GetTextureUniformString(TextureType _type);

std::string
GetTextureUniformString(const std::string& _type_string);

std::string
GetTextureFileDirectory(const std::string& _name);

std::optional<std::string>
GetTextureFilePath(const std::string& _file_directory, TextureType _type);

}
