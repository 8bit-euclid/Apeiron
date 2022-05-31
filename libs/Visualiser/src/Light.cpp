#include "../include/Light.h"

namespace aprn::vis {

/***************************************************************************************************************************************************************
* Light Abstract Base Class
***************************************************************************************************************************************************************/
Light::Light()
  : Light(LightType::None, {1.0f, 1.0f, 1.0f, 1.0f}, 1.0f, 0.0f) {}

Light::Light(Light&& _light) noexcept
  : Colour(std::move(_light.Colour)), AmbientIntensity(std::move(_light.AmbientIntensity)), DiffuseIntensity(std::move(_light.DiffuseIntensity)),
    ShadowMap(std::move(_light.ShadowMap)), Type(std::move(_light.Type)) {}

Light::Light(LightType _light_type, glm::vec4 _rgba_colour, GLfloat _ambient_intensity, GLfloat _diffuse_intensity)
  : Colour(_rgba_colour), AmbientIntensity(_ambient_intensity), DiffuseIntensity(_diffuse_intensity),
    ShadowMap(_light_type == LightType::Point || _light_type == LightType::Spot), Type(_light_type)
{
   ShadowMap.Init(2048, 2048);
}

Light&
Light::operator=(Light&& _light) noexcept
{
  Colour           = std::move(_light.Colour);
  AmbientIntensity = std::move(_light.AmbientIntensity);
  DiffuseIntensity = std::move(_light.DiffuseIntensity);
  ShadowMap        = std::move(_light.ShadowMap);
  Type             = std::move(_light.Type);

  return *this;
}

/***************************************************************************************************************************************************************
* Directional Light Class
***************************************************************************************************************************************************************/
DirectionalLight::DirectionalLight()
  : Light(), Direction(glm::vec3(0.0, -1.0, 0.0)) {}

DirectionalLight::DirectionalLight(glm::vec3 _direction, glm::vec4 _rgba_colour, GLfloat _ambient_intensity, GLfloat _diffuse_intensity)
  : Light(LightType::Directional, _rgba_colour, _ambient_intensity, _diffuse_intensity), Direction(_direction)
{
  const glm::mat4&& proj_matrix = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 1.0f, 20.0f);
  const glm::mat4&& view_matrix = glm::lookAt(-10.0f * Direction, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f});
  LightSpaceMatrix = proj_matrix * view_matrix;
}

/***************************************************************************************************************************************************************
* Point Light Abstract Base Class
***************************************************************************************************************************************************************/
namespace Detail {

template<class derived>
PointLightBase<derived>::PointLightBase(LightType _light_type, const glm::vec3& _position, const glm::vec4& _rgba_colour,
                                        GLfloat _ambient_intensity, GLfloat _diffuse_intensity,
                                        const SVector3<GLfloat>& _attenuation_coefficients)
  : Light(_light_type, _rgba_colour, _ambient_intensity, _diffuse_intensity), Position(_position), AttenuationCoefficients(_attenuation_coefficients)
{
  const glm::mat4&& proj_matrix = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, FarPlane); // Note the aspect ratio of 1.0f

  LightSpaceMatrices[0] = proj_matrix * glm::lookAt(Position, Position + glm::vec3( 1.0,  0.0,  0.0), {0.0, -1.0,  0.0}); // Right face of cube map
  LightSpaceMatrices[1] = proj_matrix * glm::lookAt(Position, Position + glm::vec3(-1.0,  0.0,  0.0), {0.0, -1.0,  0.0}); // Left face of cube map
  LightSpaceMatrices[2] = proj_matrix * glm::lookAt(Position, Position + glm::vec3( 0.0,  1.0,  0.0), {0.0,  0.0,  1.0}); // Top face of cube map
  LightSpaceMatrices[3] = proj_matrix * glm::lookAt(Position, Position + glm::vec3( 0.0, -1.0,  0.0), {0.0,  0.0, -1.0}); // Bottom face of cube map
  LightSpaceMatrices[4] = proj_matrix * glm::lookAt(Position, Position + glm::vec3( 0.0,  0.0,  1.0), {0.0, -1.0,  0.0}); // Near face of cube map
  LightSpaceMatrices[5] = proj_matrix * glm::lookAt(Position, Position + glm::vec3( 0.0,  0.0, -1.0), {0.0, -1.0,  0.0}); // Far face of cube map

  iPointLight = nPointLights++;
}

template<class derived>
PointLightBase<derived>::PointLightBase(PointLightBase<derived>&& _light) noexcept
   : Light(std::move(_light)), iPointLight(std::move(_light.iPointLight)), Position(std::move(_light.Position)),
     AttenuationCoefficients(std::move(_light.AttenuationCoefficients)), LightSpaceMatrices(std::move(_light.LightSpaceMatrices))
{
   nPointLights++; // Note: only incremented because the destructor on the 'moved' object will decrement.
}

template<class derived>
PointLightBase<derived>::~PointLightBase() { nPointLights--; }

}

template class Detail::PointLightBase<PointLight>;
template class Detail::PointLightBase<SpotLight>;
template<class derived> UInt Detail::PointLightBase<derived>::nPointLights = 0;

/***************************************************************************************************************************************************************
* Point Light Class
***************************************************************************************************************************************************************/
PointLight::PointLight(const glm::vec3& _position, const glm::vec4& _rgba_colour, GLfloat _ambient_intensity, GLfloat _diffuse_intensity,
                       const SVector3<GLfloat>& _attenuation_coefficients)
  : PointLightBase(LightType::Point, _position, _rgba_colour, _ambient_intensity, _diffuse_intensity, _attenuation_coefficients) {}

/***************************************************************************************************************************************************************
* Spotlight Class
***************************************************************************************************************************************************************/
SpotLight::SpotLight(const glm::vec3& _position, const glm::vec3& _direction, const glm::vec4& _rgba_colour, GLfloat _cone_angle, GLfloat _ambient_intensity,
                     GLfloat _diffuse_intensity, const SVector3<GLfloat>& _attenuation_coefficients)
  : PointLightBase(LightType::Spot, _position, _rgba_colour, _ambient_intensity, _diffuse_intensity, _attenuation_coefficients),
                   Direction(glm::normalize(_direction)), ConeAngle(_cone_angle), CosConeAngle(std::cos(DegToRad(_cone_angle))) {}

}
