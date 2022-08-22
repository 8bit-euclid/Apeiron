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

#include <GL/glew.h>

namespace aprn::vis {

class Material
{
 public:
   Material()
      : Material("none", 0.0, 0.0) {}

   Material(const std::string_view& name, const GLfloat _specular_intensity, const GLfloat _smoothness)
      : Name(name), SpecularIntensity(_specular_intensity), Smoothness(_smoothness) {}

 private:
   friend class Shader;

   std::string_view Name;
   GLfloat SpecularIntensity;
   GLfloat Smoothness;
};

}
