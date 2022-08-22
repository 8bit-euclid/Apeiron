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
  : Colour(std::move(light.Colour)), AmbientIntensity(std::move(light.AmbientIntensity)), DiffuseIntensity(std::move(light.DiffuseIntensity)),
    ShadowMap(std::move(light.ShadowMap)), Type(std::move(light.Type)) {}

Light::Light(LightType _light_type, glm::vec4 rgba_colour, GLfloat ambient_intensity, GLfloat diffuse_intensity)
  : Colour(rgba_colour), AmbientIntensity(ambient_intensity), DiffuseIntensity(diffuse_intensity),
    ShadowMap(_light_type == LightType::Point || _light_type == LightType::Spot), Type(_light_type)
{
   ShadowMap.Init(2048, 2048);
}

Light&
Light::operator=(Light&& light) noexcept
{
  Colour           = std::move(light.Colour);
  AmbientIntensity = std::move(light.AmbientIntensity);
  DiffuseIntensity = std::move(light.DiffuseIntensity);
  ShadowMap        = std::move(light.ShadowMap);
  Type             = std::move(light.Type);

  return *this;
}

/***************************************************************************************************************************************************************
* Directional Light Class
***************************************************************************************************************************************************************/
DirectionalLight::DirectionalLight()
  : Light(), Direction(glm::vec3(0.0, -1.0, 0.0)) {}

DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec4 rgba_colour, GLfloat ambient_intensity, GLfloat diffuse_intensity)
  : Light(LightType::Directional, rgba_colour, ambient_intensity, diffuse_intensity), Direction(direction)
{
  const glm::mat4&& proj_matrix = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 1.0f, 20.0f);
  const glm::mat4&& view_matrix = glm::lookAt(-10.0f * Direction, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f});
  LightSpaceMatrix = proj_matrix * view_matrix;
}

/***************************************************************************************************************************************************************
* Point Light Abstract Base Class
***************************************************************************************************************************************************************/
namespace detail {

template<class derived>
PointLightBase<derived>::PointLightBase(LightType _light_type, const glm::vec3& position, const glm::vec4& rgba_colour,
                                        GLfloat ambient_intensity, GLfloat diffuse_intensity,
                                        const SVector3<GLfloat>& attenuation_coefficients)
  : Light(_light_type, rgba_colour, ambient_intensity, diffuse_intensity), Position(position), AttenuationCoefficients(attenuation_coefficients)
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
PointLightBase<derived>::PointLightBase(PointLightBase<derived>&& light) noexcept
   : Light(std::move(light)), iPointLight(std::move(light.iPointLight)), Position(std::move(light.Position)),
     AttenuationCoefficients(std::move(light.AttenuationCoefficients)), LightSpaceMatrices(std::move(light.LightSpaceMatrices))
{
   nPointLights++; // Note: only incremented because the destructor of the 'moved' object will decrement.
}

template<class derived>
PointLightBase<derived>::~PointLightBase() { nPointLights--; }

}

template class detail::PointLightBase<PointLight>;
template class detail::PointLightBase<SpotLight>;
template<class derived> UInt detail::PointLightBase<derived>::nPointLights = 0;

/***************************************************************************************************************************************************************
* Point Light Class
***************************************************************************************************************************************************************/
PointLight::PointLight(const glm::vec3& position, const glm::vec4& rgba_colour, GLfloat ambient_intensity, GLfloat diffuse_intensity,
                       const SVector3<GLfloat>& attenuation_coefficients)
  : PointLightBase(LightType::Point, position, rgba_colour, ambient_intensity, diffuse_intensity, attenuation_coefficients) {}

/***************************************************************************************************************************************************************
* Spotlight Class
***************************************************************************************************************************************************************/
SpotLight::SpotLight(const glm::vec3& position, const glm::vec3& direction, const glm::vec4& rgba_colour, GLfloat _coneangle, GLfloat ambient_intensity,
                     GLfloat diffuse_intensity, const SVector3<GLfloat>& attenuation_coefficients)
  : PointLightBase(LightType::Spot, position, rgba_colour, ambient_intensity, diffuse_intensity, attenuation_coefficients),
                   Direction(glm::normalize(direction)), ConeAngle(_coneangle), CosConeAngle(std::cos(DegToRad(_coneangle))) {}

}
