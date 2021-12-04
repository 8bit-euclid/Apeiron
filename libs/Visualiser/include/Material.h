#pragma once

#include "../../../include/Global.h"

#include <GL/glew.h>

namespace Apeiron {

class Material
{
  friend class Shader;

public:
  Material() : SpecularIntensity(0.0), Smoothness(0.0) {}

  Material(const GLfloat _specular_intensity, const GLfloat _smoothness) : SpecularIntensity(_specular_intensity), Smoothness(_smoothness) {}

  ~Material() = default;

private:
  GLfloat SpecularIntensity;
  GLfloat Smoothness;
};

}
