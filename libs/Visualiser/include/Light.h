#pragma once

#include "../../../include/Global.h"
#include "../../DataContainers/include/Array.h"
#include "GLDebug.h"
#include "GLTypes.h"
#include "Shader.h"

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace Apeiron {

/***************************************************************************************************************************************************************
* Light Abstract Base Class
***************************************************************************************************************************************************************/
enum class LightType
{
  Directional,
  Point,
  Spot,
  None
};

class Light
{
protected:
  Light();

  Light(glm::vec4 _rgba_colour, GLfloat _ambient_intensity, GLfloat _diffuse_intensity);

public:
  ~Light() = default;

  void SwitchOn(Shader& _shader, LightType _light_type = LightType::None, UInt _index = -1);

protected:
  glm::vec4 Colour;
  GLfloat AmbientIntensity;
  GLfloat DiffuseIntensity;
};

/***************************************************************************************************************************************************************
* Directional Light Class
***************************************************************************************************************************************************************/
class DirectionalLight : public Light
{
public:
  DirectionalLight();

  DirectionalLight(glm::vec3 _direction, glm::vec4 _rgba_colour, GLfloat _ambient_intensity, GLfloat _diffuse_intensity);

  ~DirectionalLight() = default;

  void SwitchOn(Shader& _shader);

private:
  glm::vec3 Direction;
};

/***************************************************************************************************************************************************************
* Point Light Class
***************************************************************************************************************************************************************/
class PointLight : public Light
{
public:
  PointLight();

  PointLight(glm::vec3 _position, glm::vec4 _rgba_colour, GLfloat _ambient_intensity, GLfloat _diffuse_intensity,
             const StaticArray<GLfloat, 3>& _attenuation_coefficients);

  ~PointLight() = default;

  void SwitchOn(Shader& _shader);

private:
  constexpr static UInt MaxPointLights = 4;
  static UInt nPointLights;
  UInt iPointLight;

  glm::vec3 Position;
  StaticArray<GLfloat, 3> AttenuationCoefficients; // 0: constant term, 1: linear term, 2: quadratic term
};

}
