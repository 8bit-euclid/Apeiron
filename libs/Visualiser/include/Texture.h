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

/***************************************************************************************************************************************************************
* Texture Types
***************************************************************************************************************************************************************/
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

/***************************************************************************************************************************************************************
* Texture Class
***************************************************************************************************************************************************************/
class Texture
{
 public:
   Texture(const TextureType type, const std::string& file_path);

   Texture(const TextureType type, const bool is_fbo_attachment);

   Texture(const Texture& texture) = delete;

   Texture(Texture&& texture) noexcept;

   ~Texture();

   void Read(const std::string& file_path, const GLint wrap_type);

   void Init(GLuint width, GLuint height, GLint internal_format, GLenum format, GLenum data_type, GLint wrap_type, size_t n_samples = 1,
             const SVector4<GLfloat>& border_colour = { 1.0f, 1.0f, 1.0f, 1.0f });

   void Bind(UInt slot = 0) const;

   void Unbind() const;

   void Delete();

   inline UInt ID() const { return _ID; }

   inline int Width() const { return _Width; }

   inline int Height() const { return _Height; }

   inline const std::optional<Real>& MapScale() const { return _MapScale; }

   inline void SetMapScale(const Real scale) { _MapScale.emplace(scale); }

   Texture& operator=(const Texture& texture) = delete;

   Texture& operator=(Texture&& texture) noexcept;

 private:
   friend class Model;
   friend class Shadow;

   GLint OpenGLType() const;

   UInt                 _ID;
   int                  _Width;
   int                  _Height;
   int                  _ChannelCount;
   UPtr<UChar>          _LocalBuffer;
   std::optional<Real> _MapScale;
   TextureType          _Type;
   size_t               _SampleCount;
   bool                 _isMultiSampled;
   bool                 _isFBOAttachment;
};

/***************************************************************************************************************************************************************
* Texture Stand-alone Functions
***************************************************************************************************************************************************************/
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
