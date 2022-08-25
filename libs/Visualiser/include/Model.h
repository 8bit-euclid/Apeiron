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
#include "DataContainer/include/List.h"

#include "ActionBase.h"
#include "Buffers.h"
#include "Colour.h"
#include "Mesh.h"
#include "Material.h"
#include "Texture.h"

#include <map>
#include <memory>
#include <optional>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace aprn::vis {

class Model
{
 public:
   Model();

   Model(const Model& model);

   Model(Model&& model) noexcept;

   ~Model();

   void Update(Float global_time);

   void Render();

   void Delete();

   /** Set Model Attributes
   ************************************************************************************************************************************************************/
   Model& SetColour(const SVectorF3& rgb_colour);

   Model& SetMaterial(const std::string& name, Float specular_intensity, Float smoothness);

   Model& SetTexture(const std::string& material, const std::string& item, size_t index, size_t resolution);

   Model& Add(Model& sub_model, const std::string& name);

   Model& Add(Model&& sub_model, const std::string& name);

   /** Set Model Actions
   ************************************************************************************************************************************************************/
   Model& OffsetPosition(const SVectorF3& displacement);

   Model& OffsetOrientation(Float angle, const SVectorF3& axis);

   Model& Scale(Float factor, Float start_time, Float end_time, const std::function<Float(Float)>& reparam = Linear);

   Model& Scale(const SVectorF3& factors, Float start_time, Float end_time, const std::function<Float(Float)>& reparam = Linear);

   Model& MoveBy(const SVectorF3& displacement, Float start_time, Float end_time, const std::function<Float(Float)>& reparam = Linear);

   Model& MoveTo(const SVectorF3& position, Float start_time, Float end_time, const std::function<Float(Float)>& reparam = Linear);

   Model& MoveAt(const SVectorF3& velocity, Float start_time = Zero, const std::function<Float(Float)>& ramp = Identity);

   Model& Trace(StaticArray<std::function<Float(Float)>, 3> path, Float start_time, Float end_time = InfFloat<>);

   Model& Trace(std::function<SVectorF3(Float)> path, Float start_time, Float end_time = InfFloat<>);

   template<class D>
   Model& Trace(const mnfld::Curve<D, 3>& path, Float start_time, Float end_time, const std::function<Float(Float)>& reparam = Linear);

   Model& RotateBy(Float angle, const SVectorF3& axis, Float start_time, Float end_time, const std::function<Float(Float)>& reparam = Linear);

   Model& RotateAt(const SVectorF3& angular_velocity, Float start_time = Zero, const std::function<Float(Float)>& ramp = Identity);

   Model& RevolveBy(Float angle, const SVectorF3& axis, const SVectorF3& refe_point, Float start_time, Float end_time,
                    const std::function<Float(Float)>& reparam = Linear);

   Model& RevolveAt(const SVectorF3& angular_velocity, const SVectorF3& refe_point, Float start_time = Zero,
                    const std::function<Float(Float)>& ramp = Identity);

   /** Assignment Operators
   ************************************************************************************************************************************************************/
   Model& operator=(const Model& model);

   Model& operator=(Model&& model) noexcept;

   /** Members Accessors
   ************************************************************************************************************************************************************/
   inline const glm::mat4& ModelMatrix() const { return _ModelMatrix; }

 protected:
   friend class Visualiser;
   friend class Scene;
   friend class FrameTexture;
   friend class ModelFactory;
   friend class ActionBase;
   template<ActionType type> friend class Action;

   void Init();

   void ComputeLifespan();

   void Reset();

   void Scale(const glm::vec3& factors);

   void Translate(const glm::vec3& displacement);

   void Rotate(const GLfloat angle, const glm::vec3& axis);

   /** Model Attributes
   ************************************************************************************************************************************************************/
   template<class T> using Map = std::unordered_map<std::string, T>;
   using ATComp                = ActionTypeComparator;

   Mesh                                           _Mesh;
   Map<SPtr<Model>>                               _SubModels;
   std::map<ActionType, SPtr<ActionBase>, ATComp> _Actions;
   std::optional<std::string>                     _Texture;
   std::optional<Material>                        _Material;
   Colour                                         _StrokeColour;
   Colour                                         _FillColour;
   glm::vec3                                      _Centroid;
   glm::mat4                                      _ModelMatrix{1.0f};
   glm::mat4                                      _PreviousActions{1.0f};
   Float                                          _EntryTime{Zero};
   Float                                          _ExitTime{InfFloat<>};
   bool                                           _isInitialised{false};

   /** Data Buffers
   ************************************************************************************************************************************************************/
   VertexArray                                    _VAO;
   VertexBuffer                                   _VBO;
   IndexBuffer                                    _EBO;
   std::optional<ShaderStorageBuffer>             _SSBO;
};

}
