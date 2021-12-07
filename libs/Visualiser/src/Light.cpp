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
* Point Light Abstract Base Class
***************************************************************************************************************************************************************/
namespace LightSupport {

template<class t_derived_class>
PointLightBase<t_derived_class>::PointLightBase(const LightType _light_type, const glm::vec3& _position, const glm::vec4& _rgba_colour,
                                                const GLfloat _ambient_intensity, const GLfloat _diffuse_intensity,
                                                const StaticArray<GLfloat, 3>& _attenuation_coefficients)
  : Light(_light_type, _rgba_colour, _ambient_intensity, _diffuse_intensity), Position(_position), AttenuationCoefficients(_attenuation_coefficients)
{
  iPointLight = nPointLights++;
}

template<class t_derived_class>
PointLightBase<t_derived_class>::PointLightBase(const PointLightBase<t_derived_class>& _point_light_base)
  : iPointLight(_point_light_base.iPointLight), Position(_point_light_base.Position), AttenuationCoefficients(_point_light_base.AttenuationCoefficients)
{
  nPointLights++;
}

template<class t_derived_class>
PointLightBase<t_derived_class>::~PointLightBase()
{
  nPointLights--;
}

}

template class LightSupport::PointLightBase<PointLight>;
template class LightSupport::PointLightBase<SpotLight>;
template<class t_derived_class> UInt LightSupport::PointLightBase<t_derived_class>::nPointLights = 0;

/***************************************************************************************************************************************************************
* Point Light Class
***************************************************************************************************************************************************************/
PointLight::PointLight(const glm::vec3& _position, const glm::vec4& _rgba_colour, const GLfloat _ambient_intensity, const GLfloat _diffuse_intensity,
                       const StaticArray<GLfloat, 3>& _attenuation_coefficients)
  : PointLightBase(LightType::Point, _position, _rgba_colour, _ambient_intensity, _diffuse_intensity, _attenuation_coefficients)
{

}

/***************************************************************************************************************************************************************
* Spotlight Class
***************************************************************************************************************************************************************/
SpotLight::SpotLight(const glm::vec3& _position, const glm::vec3& _direction, const glm::vec4& _rgba_colour, GLfloat _cone_angle, GLfloat _ambient_intensity,
                     GLfloat _diffuse_intensity, const StaticArray<GLfloat, 3>& _attenuation_coefficients)
  : PointLightBase(LightType::Spot, _position, _rgba_colour, _ambient_intensity, _diffuse_intensity, _attenuation_coefficients),
                   Direction(glm::normalize(_direction)), ConeAngle(_cone_angle), CosConeAngle(std::cos(ToRadians(_cone_angle)))
{

}

}
