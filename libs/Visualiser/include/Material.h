#pragma once

#include "../../../include/Global.h"
#include "Shader.h"

#include <GL/glew.h>

namespace Apeiron {

class Material
{
public:
  Material() : SpecularIntensity(0.0), Smoothness(0.0) {}

  Material(const GLfloat _specular_intensity, const GLfloat _smoothness) : SpecularIntensity(_specular_intensity), Smoothness(_smoothness) {}

  ~Material() = default;

  inline void Apply(Shader& _shader)
  {
    _shader.SetUniform1f("u_material.SpecularIntensity", SpecularIntensity);
    _shader.SetUniform1f("u_material.Smoothness", Smoothness);
  }

private:
  GLfloat SpecularIntensity;
  GLfloat Smoothness;
};

}
