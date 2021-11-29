#include "../include/Light.h"

namespace Apeiron {

/***************************************************************************************************************************************************************
* Light Abstract Base Class
***************************************************************************************************************************************************************/
Light::Light()
  : Light(glm::vec4(1.0, 1.0, 1.0, 1.0), 1.0, 0.0)
{

}

Light::Light(glm::vec4 _rgba_colour, GLfloat _ambient_intensity, GLfloat _diffuse_intensity)
  : Colour(_rgba_colour), AmbientIntensity(_ambient_intensity), DiffuseIntensity(_diffuse_intensity)
{

}

void Light::SwitchOn(Shader& _shader, const LightType _light_type, const UInt _index)
{
  std::string light_uniform = _light_type == LightType::Directional ? "u_directional_light" :
                              _light_type == LightType::Point ? "u_point_lights[" + To_Str(_index) + "]" :
                              _light_type == LightType::Spot ? "u_spot_lights[" + To_Str(_index) + "]" :
                              throw std::invalid_argument("The lighting type was either not recognised or not specified.");

  _shader.SetUniform4f(light_uniform + ".Base.Colour", Colour.r, Colour.g, Colour.b, Colour.a);
  _shader.SetUniform1f(light_uniform + ".Base.AmbientIntensity", AmbientIntensity);
  _shader.SetUniform1f(light_uniform + ".Base.DiffuseIntensity", DiffuseIntensity);
}

/***************************************************************************************************************************************************************
* Directional Light Class
***************************************************************************************************************************************************************/
DirectionalLight::DirectionalLight()
  : Light(), Direction(glm::vec3(0.0, -1.0, 0.0))
{

}

DirectionalLight::DirectionalLight(glm::vec3 _direction, glm::vec4 _rgba_colour, GLfloat _ambient_intensity, GLfloat _diffuse_intensity)
  : Light(_rgba_colour, _ambient_intensity, _diffuse_intensity), Direction(_direction)
{

}

void DirectionalLight::SwitchOn(Shader& _shader)
{
  Light::SwitchOn(_shader, LightType::Directional);
  _shader.SetUniform3f("u_directional_light.Direction", Direction.x, Direction.y, Direction.z);
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
  : Light(_rgba_colour, _ambient_intensity, _diffuse_intensity), Position(_position), AttenuationCoefficients(_attenuation_coefficients)
{
  iPointLight = nPointLights++;
  ASSERT(nPointLights <= MaxPointLights, "Currently cannot create more than ", MaxPointLights, " point lights.")
}

void PointLight::SwitchOn(Shader& _shader)
{
  Light::SwitchOn(_shader, LightType::Point, iPointLight);
  _shader.SetUniform3f("u_point_lights[" + To_Str(iPointLight) + "].Position", Position.x, Position.y, Position.z);
  _shader.SetUniform3f("u_point_lights[" + To_Str(iPointLight) + "].AttenuationCoefficients", AttenuationCoefficients[0], AttenuationCoefficients[1], AttenuationCoefficients[2]);
}

UInt PointLight::nPointLights = 0;

}
