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
