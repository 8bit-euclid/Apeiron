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

   Light(LightType _light_type, glm::vec4 rgba_colour, GLfloat ambient_intensity, GLfloat diffuse_intensity);

 public:
   ~Light() = default;

   virtual UInt GetIndex() const = 0;

   virtual UInt GetLightCount() const = 0;

   Shadow& GetShadowMap() { return ShadowMap; }

   Light& operator=(const Light& light) = delete;

   Light& operator=(Light&& light) noexcept;

 protected:
   glm::vec4 Colour;
   GLfloat AmbientIntensity;
   GLfloat DiffuseIntensity;
   Shadow ShadowMap;
   LightType Type;
};

/***************************************************************************************************************************************************************
* Directional Light Class
***************************************************************************************************************************************************************/
class DirectionalLight : public Light
{
 public:
   DirectionalLight();

   DirectionalLight(glm::vec3 direction, glm::vec4 rgba_colour, GLfloat ambient_intensity, GLfloat diffuse_intensity);

   UInt GetIndex() const override { return 0; }

   UInt GetLightCount() const override { return 1; }

   const glm::mat4& GetLightSpaceMatrix() const { return LightSpaceMatrix; }

 private:
   friend class Shader;

   glm::vec3 Direction;
   glm::mat4 LightSpaceMatrix;
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

   PointLightBase(LightType _light_type, const glm::vec3& position, const glm::vec4& rgba_colour, GLfloat ambient_intensity, GLfloat diffuse_intensity,
                  const SVector3<GLfloat>& attenuation_coefficients);

   PointLightBase(const PointLightBase<derived>& light) = delete;

   PointLightBase(PointLightBase<derived>&& light) noexcept;

 public:
   ~PointLightBase();

   UInt GetIndex() const override { return iPointLight; }

   UInt GetLightCount() const override { return nPointLights; }

   constexpr static GLfloat
   GetFarPlane() { return FarPlane; }

   const glm::vec3&
   GetPosition() const { return Position; }

   const StaticArray<glm::mat4, 6>&
   GetLightSpaceMatrices() const { return LightSpaceMatrices; }

   PointLightBase<derived>&
   operator=(const PointLightBase<derived>& _other) = delete;

   PointLightBase<derived>&
   operator=(PointLightBase<derived>&& _other) = default;

 protected:
   constexpr static UInt     MaxPointLights{4};
   constexpr static GLfloat  FarPlane{25.0f};
   inline static UInt        nPointLights = 0;
   UInt                      iPointLight;
   glm::vec3                 Position;
   StaticVector<GLfloat, 3>  AttenuationCoefficients; // [0]: constant term, [1]: linear term, [2]: quadratic term
   StaticArray<glm::mat4, 6> LightSpaceMatrices;
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
   SpotLight(const glm::vec3& position, const glm::vec3& direction, const glm::vec4& rgba_colour, GLfloat _coneangle, GLfloat ambient_intensity,
             GLfloat diffuse_intensity, const SVector3<GLfloat>& attenuation_coefficients);

 private:
   friend class Shader;

   glm::vec3 Direction;
   GLfloat ConeAngle;
   GLfloat CosConeAngle;
};

}
