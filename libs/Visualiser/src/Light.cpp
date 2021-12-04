#include "../include/Light.h"

namespace Apeiron {

/***************************************************************************************************************************************************************
* Light Abstract Base Class
***************************************************************************************************************************************************************/
Light::Light()
  : Light(LightType::None, glm::vec4(1.0, 1.0, 1.0, 1.0), 1.0, 0.0)
{

}

Light::Light(LightType _light_type, glm::vec4 _rgba_colour, GLfloat _ambient_intensity, GLfloat _diffuse_intensity)
  : Type(_light_type), Colour(_rgba_colour), AmbientIntensity(_ambient_intensity), DiffuseIntensity(_diffuse_intensity)
{

}

/***************************************************************************************************************************************************************
* Directional Light Class
***************************************************************************************************************************************************************/
DirectionalLight::DirectionalLight()
  : Light(), Direction(glm::vec3(0.0, -1.0, 0.0))
{

}

DirectionalLight::DirectionalLight(glm::vec3 _direction, glm::vec4 _rgba_colour, GLfloat _ambient_intensity, GLfloat _diffuse_intensity)
  : Light(LightType::Directional, _rgba_colour, _ambient_intensity, _diffuse_intensity), Direction(_direction)
{

}

/***************************************************************************************************************************************************************
* Point Light Class
***************************************************************************************************************************************************************/
PointLight::PointLight()
  : Light()
{
  iPointLight = nPointLights++;
  ASSERT(nPointLights <= MaxPointLights, "Currently cannot create more than ", MaxPointLights, " point lights.")
}

PointLight::PointLight(glm::vec3 _position, glm::vec4 _rgba_colour, GLfloat _ambient_intensity, GLfloat _diffuse_intensity,
                       const StaticArray<GLfloat, 3>& _attenuation_coefficients)
  : Light(LightType::Point, _rgba_colour, _ambient_intensity, _diffuse_intensity), Position(_position), AttenuationCoefficients(_attenuation_coefficients)
{
  iPointLight = nPointLights++;
  ASSERT(nPointLights <= MaxPointLights, "Currently cannot create more than ", MaxPointLights, " point lights.")
}

PointLight::PointLight(const PointLight& _point_light)
{
  iPointLight = _point_light.iPointLight;
  Position = _point_light.Position;
  AttenuationCoefficients = _point_light.AttenuationCoefficients;
}

PointLight::~PointLight()
{
  nPointLights--;
}

PointLight& PointLight::operator=(const PointLight& _point_light)
{
  iPointLight = _point_light.iPointLight;
  Position = _point_light.Position;
  AttenuationCoefficients = _point_light.AttenuationCoefficients;
}

UInt PointLight::nPointLights = 0;

}
