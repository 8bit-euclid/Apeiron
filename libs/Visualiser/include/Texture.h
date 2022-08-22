/***************************************************************************************************************************************************************
* GPL-3.0 License
* Copyright (C) 2022 Niran A. Ilangakoon
*
* This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as
* published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
* of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program.
* If not, see <https://www.gnu.org/licenses/>.
***************************************************************************************************************************************************************/

#pragma once

#include "../../../include/Global.h"
#include "LinearAlgebra/include/Vector.h"
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
   Displacement,
   Roughness,
   AmbientOcclusion,

   /** Frame buffer object types */
   DirectionalDepth,
   PointDepth
};

class Texture
{
 public:
   Texture(const TextureType type, const std::string& file_path);

   Texture(const TextureType type, const bool is_fbo_attachment);

   Texture(const Texture& texture) = delete;

   Texture(Texture&& texture) noexcept;

   ~Texture();

   void Init(const GLuint width, const GLuint height, const GLint internal_format, const GLenum format, const GLenum data_type, const GLint wrap_type,
             const SVector4<GLfloat>& border_colour = { 1.0f, 1.0f, 1.0f, 1.0f });

   void Read(const std::string& file_path, const GLint wrap_type);

   void Bind(UInt slot = 0) const;

   void Unbind() const;

   void Delete();

   inline UInt ID() const { return _ID; }

   inline int Width() const { return _Width; }

   inline int Height() const { return _Height; }

   inline const std::optional<Float>& MapScale() const { return _MapScale; }

   inline void SetMapScale(Float scale) { _MapScale.emplace(scale); }

   Texture& operator=(const Texture& texture) = delete;

   Texture& operator=(Texture&& texture) noexcept;

 private:
   friend class Model;
   friend class Shadow;

   UInt                 _ID;
   int                  _Width;
   int                  _Height;
   int                  _BitsPerPixel;
   UPtr<UChar>          _LocalBuffer;
   std::optional<Float> _MapScale;
   TextureType          _Type;
   bool                 _isFBOAttachment;
};

GLint
OpenGLType(TextureType type);

std::string
TextureName(const std::string& material, const std::string& item, const size_t index, const size_t resolution);

std::string
TextureTypeString(TextureType type);

TextureType
GetTextureType(const std::string& type_string);

std::string
TextureUniformString(const TextureType type);

std::string
TextureUniformString(const std::string& type_string);

std::string
TextureDirectory(const std::string& name);

std::optional<std::string>
TexturePath(const std::string& file_directory, TextureType type);

}
