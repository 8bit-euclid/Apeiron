/***************************************************************************************************************************************************************
* GPL-3.0 License
* Copyright (C) 2022 Niran A. Ilangakoon
*
* This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as
* published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
* of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program.
* If not, see <https://www.gnu.org/licenses/>.
***************************************************************************************************************************************************************/

#include "../include/Light.h"

namespace aprn::vis {

/***************************************************************************************************************************************************************
* Light Abstract Base Class
***************************************************************************************************************************************************************/
Light::Light()
  : Light(LightType::None, {1.0f, 1.0f, 1.0f, 1.0f}, 1.0f, 0.0f) {}

Light::Light(Light&& light) noexcept
  : _Colour(std::move(light._Colour)), _AmbientIntensity(std::move(light._AmbientIntensity)), _DiffuseIntensity(std::move(light._DiffuseIntensity)),
    _ShadowMap(std::move(light._ShadowMap)), _Type(std::move(light._Type)) {}

Light::Light(LightType type, glm::vec4 rgba_colour, GLfloat ambient_intensity, GLfloat diffuse_intensity)
  : _Colour(rgba_colour), _AmbientIntensity(ambient_intensity), _DiffuseIntensity(diffuse_intensity),
    _ShadowMap(type == OneOf(LightType::Point, LightType::Spot)), _Type(type) {}

Light&
Light::operator=(Light&& light) noexcept
{
   _Colour           = std::move(light._Colour);
   _AmbientIntensity = std::move(light._AmbientIntensity);
   _DiffuseIntensity = std::move(light._DiffuseIntensity);
   _ShadowMap        = std::move(light._ShadowMap);
   _Type             = std::move(light._Type);

  return *this;
}

/***************************************************************************************************************************************************************
* Directional Light Class
***************************************************************************************************************************************************************/
DirectionalLight::DirectionalLight()
  : Light(), _Direction(glm::vec3(0.0, -1.0, 0.0)) {}

DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec4 rgba_colour, GLfloat ambient_intensity, GLfloat diffuse_intensity)
  : Light(LightType::Directional, rgba_colour, ambient_intensity, diffuse_intensity), _Direction(direction)
{
  const glm::mat4&& proj_matrix = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 1.0f, 20.0f);
  const glm::mat4&& view_matrix = glm::lookAt(-10.0f * _Direction, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f});
   _LightSpaceMatrix = proj_matrix * view_matrix;
}

/***************************************************************************************************************************************************************
* Point Light Abstract Base Class
***************************************************************************************************************************************************************/
namespace detail {

template<class derived>
PointLightBase<derived>::PointLightBase(LightType type, const glm::vec3& position, const glm::vec4& rgba_colour,
                                        GLfloat ambient_intensity, GLfloat diffuse_intensity,
                                        const SVector3<GLfloat>& attenuation_coefficients)
  : Light(type, rgba_colour, ambient_intensity, diffuse_intensity), _Position(position), _AttenuationCoefficients(attenuation_coefficients)
{
  const glm::mat4&& proj_matrix = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, _FarPlane); // Note the aspect ratio of 1.0f

  _LightSpaceMatrices[0] = proj_matrix * glm::lookAt(_Position, _Position + glm::vec3(1.0, 0.0, 0.0), {0.0, -1.0, 0.0}); // Right face of cube map
  _LightSpaceMatrices[1] = proj_matrix * glm::lookAt(_Position, _Position + glm::vec3(-1.0, 0.0, 0.0), {0.0, -1.0, 0.0}); // Left face of cube map
  _LightSpaceMatrices[2] = proj_matrix * glm::lookAt(_Position, _Position + glm::vec3(0.0, 1.0, 0.0), {0.0, 0.0, 1.0}); // Top face of cube map
  _LightSpaceMatrices[3] = proj_matrix * glm::lookAt(_Position, _Position + glm::vec3(0.0, -1.0, 0.0), {0.0, 0.0, -1.0}); // Bottom face of cube map
  _LightSpaceMatrices[4] = proj_matrix * glm::lookAt(_Position, _Position + glm::vec3(0.0, 0.0, 1.0), {0.0, -1.0, 0.0}); // Near face of cube map
  _LightSpaceMatrices[5] = proj_matrix * glm::lookAt(_Position, _Position + glm::vec3(0.0, 0.0, -1.0), {0.0, -1.0, 0.0}); // Far face of cube map

  _iPointLight = _PointLightCount++;
}

template<class derived>
PointLightBase<derived>::PointLightBase(PointLightBase<derived>&& light) noexcept
   : Light(std::move(light)), _iPointLight(std::move(light._iPointLight)), _Position(std::move(light._Position)),
     _AttenuationCoefficients(std::move(light._AttenuationCoefficients)), _LightSpaceMatrices(std::move(light._LightSpaceMatrices))
{
   _PointLightCount++; // Note: only incremented because the destructor of the 'moved' object will decrement.
}

template<class derived>
PointLightBase<derived>::~PointLightBase() { _PointLightCount--; }

}

template class detail::PointLightBase<PointLight>;
template class detail::PointLightBase<SpotLight>;

/***************************************************************************************************************************************************************
* Point Light Class
***************************************************************************************************************************************************************/
PointLight::PointLight(const glm::vec3& position, const glm::vec4& rgba_colour, GLfloat ambient_intensity, GLfloat diffuse_intensity,
                       const SVector3<GLfloat>& attenuation_coefficients)
  : PointLightBase(LightType::Point, position, rgba_colour, ambient_intensity, diffuse_intensity, attenuation_coefficients) {}

/***************************************************************************************************************************************************************
* Spotlight Class
***************************************************************************************************************************************************************/
SpotLight::SpotLight(const glm::vec3& position, const glm::vec3& direction, const glm::vec4& rgba_colour, GLfloat cone_angle, GLfloat ambient_intensity,
                     GLfloat diffuse_intensity, const SVector3<GLfloat>& attenuation_coefficients)
  : PointLightBase(LightType::Spot, position, rgba_colour, ambient_intensity, diffuse_intensity, attenuation_coefficients),
    _Direction(glm::normalize(direction)), _ConeAngle(cone_angle), _CosConeAngle(std::cos(DegToRad(cone_angle))) {}

}
