#pragma once

#include "../../../include/Global.h"
#include "../../DataContainers/include/Array.h"
#include "GLDebug.h"
#include "GLTypes.h"

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
  friend class Shader;

protected:
  Light();

  Light(LightType _light_type, glm::vec4 _rgba_colour, GLfloat _ambient_intensity, GLfloat _diffuse_intensity);

public:
  ~Light() = default;

  virtual UInt GetIndex() const = 0;

  virtual UInt GetLightCount() const = 0;

protected:
  LightType Type;
  glm::vec4 Colour;
  GLfloat AmbientIntensity;
  GLfloat DiffuseIntensity;
};

/***************************************************************************************************************************************************************
* Directional Light Class
***************************************************************************************************************************************************************/
class DirectionalLight : public Light
{
  friend class Shader;

public:
  DirectionalLight();

  DirectionalLight(glm::vec3 _direction, glm::vec4 _rgba_colour, GLfloat _ambient_intensity, GLfloat _diffuse_intensity);

  ~DirectionalLight() = default;

  UInt GetIndex() const override { return 0; }

  UInt GetLightCount() const override { return 1; }

private:
  glm::vec3 Direction;
};

/***************************************************************************************************************************************************************
* Point Light Class
***************************************************************************************************************************************************************/
class PointLight : public Light
{
  friend class Shader;

public:
  PointLight();

  PointLight(glm::vec3 _position, glm::vec4 _rgba_colour, GLfloat _ambient_intensity, GLfloat _diffuse_intensity,
             const StaticArray<GLfloat, 3>& _attenuation_coefficients);

  PointLight(const PointLight& _point_light);

  ~PointLight();

  PointLight& operator=(const PointLight& _point_light);

  UInt GetIndex() const override { return iPointLight; }

  UInt GetLightCount() const override { return nPointLights; }

private:
  constexpr static UInt MaxPointLights = 4;
  static UInt nPointLights;
  UInt iPointLight;

  glm::vec3 Position;
  StaticArray<GLfloat, 3> AttenuationCoefficients; // 0: constant term, 1: linear term, 2: quadratic term
};

}
