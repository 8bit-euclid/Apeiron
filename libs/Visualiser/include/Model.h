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
#include "DataContainer/include/Array.h"

#include "ActionBase.h"
#include "Buffers.h"
#include "Colour.h"
#include "Material.h"
#include "Mesh.h"
#include "Model.h"

#include <map>
#include <memory>
#include <optional>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace aprn::vis {

class Model : public ModelObject
{
 public:
   Model();

   Model(const Model& sub_model);

   Model(Model&& sub_model) noexcept;

   ~Model();

   void Update(Real global_time) override;

   void Render() override;

   void Delete() override;

   /** Set Model Attributes
   ************************************************************************************************************************************************************/
   Model& SetColour(const SVectorR4& rgba_colour) override;

   Model& SetColour(const Colour& colour) override;

   Model& SetMaterial(const std::string& name, Real specular_intensity, Real smoothness) override;

   Model& SetTexture(const std::string& material, size_t index, size_t resolution, Real dispacement_scale) override;

   Model& SetTexture(const std::string& material, const std::string& item, size_t index, size_t resolution, Real dispacement_scale) override;

   /** Set Model Actions
   ************************************************************************************************************************************************************/
   Model& OffsetPosition(const SVectorR3& displacement) override;

   Model& OffsetOrientation(Real angle, const SVectorR3& axis) override;

   Model& Scale(Real factor, Real start_time, Real end_time, const std::function<Real(Real)>& reparam = Linear) override;

   Model& Scale(const SVectorR3& factors, Real start_time, Real end_time, const std::function<Real(Real)>& reparam = Linear) override;

   Model& MoveBy(const SVectorR3& displacement, Real start_time, Real end_time, const std::function<Real(Real)>& reparam = Linear) override;

   Model& MoveTo(const SVectorR3& position, Real start_time, Real end_time, const std::function<Real(Real)>& reparam = Linear) override;

   Model& MoveAt(const SVectorR3& velocity, Real start_time = Zero, const std::function<Real(Real)>& ramp = Identity) override;

   Model& Trace(std::function<SVectorR3(Real)> path, Real start_time, Real end_time = InfFloat<>, const std::function<Real(Real)>& reparam = Linear) override;

   Model& RotateBy(Real angle, const SVectorR3& axis, Real start_time, Real end_time, const std::function<Real(Real)>& reparam = Linear) override;

   Model& RotateAt(const SVectorR3& angular_velocity, Real start_time = Zero, const std::function<Real(Real)>& ramp = Identity) override;

   Model& RevolveBy(Real angle, const SVectorR3& axis, const SVectorR3& refe_point, Real start_time, Real end_time,
                    const std::function<Real(Real)>& reparam = Linear) override;

   Model& RevolveAt(const SVectorR3& angular_velocity, const SVectorR3& refe_point, Real start_time = Zero,
                    const std::function<Real(Real)>& ramp = Identity) override;

   /** Assignment Operators
   ************************************************************************************************************************************************************/
   Model& operator=(const Model& model);

   Model& operator=(Model&& model) noexcept;

   /** Other
   ************************************************************************************************************************************************************/
   inline bool isInitialised() const override { return _isInitialised; }

 protected:
   void Init() override;

   void ComputeLifespan() override;

   inline void Reset() { _ModelMatrix = glm::mat4(1.0); }

   inline void Scale(const glm::vec3& factors) { _ModelMatrix = glm::scale(_ModelMatrix, factors); }

   inline void Translate( const glm::vec3& displacement) { _ModelMatrix = glm::translate(_ModelMatrix, displacement); }

   inline void Rotate(const GLfloat angle, const glm::vec3& axis) { _ModelMatrix = glm::rotate(_ModelMatrix, angle, axis); }

 protected:
   using ATComp = ActionTypeComparator;

   Mesh                                           _Mesh;
   VertexArray                                    _VAO;
   VertexBuffer                                   _VBO;
   IndexBuffer                                    _EBO;
   std::optional<ShaderStorageBuffer>             _SSBO;
   std::map<ActionType, SPtr<ActionBase>, ATComp> _Actions;
   std::optional<Pair<std::string, Real>>         _TextureInfo;
   std::optional<Material>                        _Material;
   Colour                                         _StrokeColour;
   Colour                                         _FillColour;
   glm::vec3                                      _Centroid;
   glm::mat4                                      _ModelMatrix{1.0f};
   glm::mat4                                      _PreviousActions{1.0f};
};

}
