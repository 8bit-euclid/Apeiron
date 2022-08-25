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

#pragma once

#include "../../../include/Global.h"
#include "../../DataContainer/include/Array.h"
#include "../../LinearAlgebra/include/Vector.h"
#include "GLDebug.h"
#include "GLTypes.h"
#include "Shadow.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace aprn::vis {

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

   Light(const Light& light) = delete;

   Light(Light&& light) noexcept;

   Light(LightType type, glm::vec4 rgba_colour, GLfloat ambient_intensity, GLfloat diffuse_intensity);

 public:
   ~Light() = default;

   virtual UInt Index() const = 0;

   virtual UInt LightCount() const = 0;

   inline Shadow& ShadowMap() { return _ShadowMap; }

   Light& operator=(const Light& light) = delete;

   Light& operator=(Light&& light) noexcept;

 protected:
   glm::vec4 _Colour;
   GLfloat   _AmbientIntensity;
   GLfloat   _DiffuseIntensity;
   Shadow    _ShadowMap;
   LightType _Type;
};

/***************************************************************************************************************************************************************
* Directional Light Class
***************************************************************************************************************************************************************/
class DirectionalLight : public Light
{
 public:
   DirectionalLight();

   DirectionalLight(glm::vec3 direction, glm::vec4 rgba_colour, GLfloat ambient_intensity, GLfloat diffuse_intensity);

   UInt Index() const override { return 0; }

   UInt LightCount() const override { return 1; }

   inline const glm::mat4& LightSpaceMatrix() const { return _LightSpaceMatrix; }

 private:
   friend class Shader;

   glm::vec3 _Direction;
   glm::mat4 _LightSpaceMatrix;
};

/***************************************************************************************************************************************************************
* Point Light Abstract Base Class
***************************************************************************************************************************************************************/
namespace detail {

template<class derived>
class PointLightBase : public Light
{
 protected:
   PointLightBase() = delete;

   PointLightBase(LightType type, const glm::vec3& position, const glm::vec4& rgba_colour, GLfloat ambient_intensity, GLfloat diffuse_intensity,
                  const SVector3<GLfloat>& attenuation_coefficients);

   PointLightBase(const PointLightBase<derived>& light) = delete;

   PointLightBase(PointLightBase<derived>&& light) noexcept;

 public:
   ~PointLightBase();

   UInt Index() const override { return _iPointLight; }

   UInt LightCount() const override { return _PointLightCount; }

   constexpr static GLfloat FarPlane() { return _FarPlane; }

   inline const glm::vec3& Position() const { return _Position; }

   inline const StaticArray<glm::mat4, 6>& LightSpaceMatrices() const { return _LightSpaceMatrices; }

   PointLightBase<derived>& operator=(const PointLightBase<derived>& _other) = delete;

   PointLightBase<derived>& operator=(PointLightBase<derived>&& _other) = default;

 protected:
   constexpr static UInt     _MaxPointLights{4};
   constexpr static GLfloat  _FarPlane{25.0f};
   inline static UInt        _PointLightCount = 0;
   UInt                      _iPointLight;
   glm::vec3                 _Position;
   StaticVector<GLfloat, 3>  _AttenuationCoefficients; // [0]: constant term, [1]: linear term, [2]: quadratic term
   StaticArray<glm::mat4, 6> _LightSpaceMatrices;
};

}

/***************************************************************************************************************************************************************
* Point Light Class
***************************************************************************************************************************************************************/
class PointLight : public detail::PointLightBase<PointLight>
{
 public:
   PointLight(const glm::vec3& position, const glm::vec4& rgba_colour, GLfloat ambient_intensity, GLfloat diffuse_intensity,
              const SVector3<GLfloat>& attenuation_coefficients);

   friend class Shader;
};

/***************************************************************************************************************************************************************
* Spotlight Class
***************************************************************************************************************************************************************/
class SpotLight : public detail::PointLightBase<SpotLight>
{
 public:
   SpotLight(const glm::vec3& position, const glm::vec3& direction, const glm::vec4& rgba_colour, GLfloat cone_angle, GLfloat ambient_intensity,
             GLfloat diffuse_intensity, const SVector3<GLfloat>& attenuation_coefficients);

 private:
   friend class Shader;

   glm::vec3 _Direction;
   GLfloat   _ConeAngle;
   GLfloat   _CosConeAngle;
};

}
