#pragma once

#include "../../../include/Global.h"

#include <GL/glew.h>

namespace aprn::vis {

class Material
{
 public:
   Material()
      : Material("none", 0.0, 0.0) {}

   Material(const std::string& _name, const GLfloat _specular_intensity, const GLfloat _smoothness)
      : Name(_name), SpecularIntensity(_specular_intensity), Smoothness(_smoothness) {}

 private:
   friend class Shader;

   std::string Name;
   GLfloat SpecularIntensity;
   GLfloat Smoothness;
};

}
