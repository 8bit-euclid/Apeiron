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

#include "../include/ModelGroup.h"
#include "../include/Action.h"
#include "include/Model.h"


namespace aprn::vis {

/***************************************************************************************************************************************************************
* Model Public Interface
***************************************************************************************************************************************************************/
Model::Model()
{

}

Model::Model(const Model& sub_model)
{

}

Model::Model(Model&& sub_model) noexcept
{

}

Model::~Model()
{

}

void
Model::Update(const Real global_time)
{
   Reset();
   FOR_EACH(_, action, _Actions) action->Do(global_time);
}

void
Model::Render()
{
   if(_isInitialised)
   {
      _VAO.Bind();
      _EBO.Bind();

      GLCall(glDrawElements(GL_TRIANGLES, _EBO.IndexCount(), GL_UNSIGNED_INT, nullptr))

      _EBO.Unbind();
      _VAO.Unbind();
   }
}

void
Model::Delete()
{
   _VBO.Delete();
   _VAO.Delete();
   _EBO.Delete();
}

/** Set Model Attributes
***************************************************************************************************************************************************************/
ModelObject&
Model::SetColour(const SVectorR4& rgba_colour)
{
   FOR_EACH(vertex, _Mesh.Vertices) vertex.Colour = SArrayToGlmVec(rgba_colour);
   return *this;
}

ModelObject&
Model::SetMaterial(const std::string& name, const Real specular_intensity, const Real smoothness)
{
   _Material.emplace(name, specular_intensity, smoothness);
   return *this;
}

ModelObject&
Model::SetTexture(const std::string& material, const size_t index, const size_t resolution, const Real dispacement_scale)
{
   SetTexture(material, "", index, resolution ,dispacement_scale);
   return *this;
}

ModelObject&
Model::SetTexture(const std::string& material, const std::string& item, const size_t index, const size_t resolution, const Real dispacement_scale)
{
   _TextureInfo.emplace(std::make_pair(TextureName(material, item, index, resolution), dispacement_scale));
   return *this;
}

/** Set Model Actions
***************************************************************************************************************************************************************/
ModelObject&
Model::OffsetPosition(const SVectorR3& displacement)
{
   SPtr<ActionBase> ptr = std::make_shared<Action<ActionType::OffsetPosition>>(*this, SArrayToGlmVec(displacement));
   _Actions.insert({ActionType::OffsetPosition, ptr});
   return *this;
}

ModelObject&
Model::OffsetOrientation(const Real angle, const SVectorR3& axis)
{
   SPtr<ActionBase> ptr = std::make_shared<Action<ActionType::OffsetOrientation>>(*this, angle, SArrayToGlmVec(axis));
   _Actions.insert({ActionType::OffsetOrientation, ptr});
   return *this;
}

ModelObject&
Model::Scale(const Real factor, const Real start_time, const Real end_time, const std::function<Real(Real)>& reparam)
{
   Scale(SVectorR3(factor), start_time, end_time, reparam);
   return *this;
}

ModelObject&
Model::Scale(const SVectorR3& factors, const Real start_time, const Real end_time, const std::function<Real(Real)>& reparam)
{
   SPtr<ActionBase> ptr = std::make_shared<Action<ActionType::Scale>>(*this, SArrayToGlmVec(factors), start_time, end_time);
   _Actions.insert({ActionType::Scale, ptr});
   return *this;
}

ModelObject&
Model::MoveBy(const SVectorR3& displacement, const Real start_time, const Real end_time, const std::function<Real(Real)>& reparam)
{
   SPtr<ActionBase> ptr = std::make_shared<Action<ActionType::MoveBy>>(*this, SArrayToGlmVec(displacement), start_time, end_time);
   _Actions.insert({ActionType::MoveBy, ptr});
   return *this;
}

ModelObject&
Model::MoveTo(const SVectorR3& position, const Real start_time, const Real end_time, const std::function<Real(Real)>& reparam)
{
   SPtr<ActionBase> ptr = std::make_shared<Action<ActionType::MoveTo>>(*this, SArrayToGlmVec(position), start_time, end_time);
   _Actions.insert({ActionType::MoveTo, ptr});
   return *this;
}

ModelObject&
Model::MoveAt(const SVectorR3& velocity, const Real start_time, const std::function<Real(Real)>& ramp)
{
   EXIT("TODO")
   return *this;
}

ModelObject&
Model::Trace(std::function<SVectorR3(Real)> path, const Real start_time, const Real end_time, const std::function<Real(Real)>& reparam)
{
   SPtr<ActionBase> ptr = std::make_shared<Action<ActionType::Trace>>(*this, path, start_time, end_time);
   _Actions.insert({ActionType::Trace, ptr});
   return *this;
}

ModelObject&
Model::RotateBy(const Real angle, const SVectorR3& axis, const Real start_time, const Real end_time, const std::function<Real(Real)>& reparam)
{
   SPtr<ActionBase> ptr = std::make_shared<Action<ActionType::RotateBy>>(*this, angle, SArrayToGlmVec(axis), start_time, end_time);
   _Actions.insert({ActionType::RotateBy, ptr});
   return *this;
}

ModelObject&
Model::RotateAt(const SVectorR3& angular_velocity, const Real start_time, const std::function<Real(Real)>& ramp)
{
   SPtr<ActionBase> ptr = std::make_shared<Action<ActionType::RotateAt>>(*this, SArrayToGlmVec(angular_velocity), start_time, ramp);
   _Actions.insert({ActionType::RotateAt, ptr});
   return *this;
}

ModelObject&
Model::RevolveBy(const Real angle, const SVectorR3& axis, const SVectorR3& refe_point, const Real start_time, const Real end_time,
                 const std::function<Real(Real)>& reparam)
{
   SPtr<ActionBase> ptr =
      std::make_shared<Action<ActionType::RevolveBy>>(*this, angle, SArrayToGlmVec(axis), SArrayToGlmVec(refe_point), start_time, end_time);
   _Actions.insert({ActionType::RevolveBy, ptr});
   return *this;
}

ModelObject&
Model::RevolveAt(const SVectorR3& angular_velocity, const SVectorR3& refe_point, const Real start_time, const std::function<Real(Real)>& ramp)
{
   EXIT("TODO")
   return *this;
}

/** Assignment Operators
***************************************************************************************************************************************************************/
Model&
Model::operator=(const Model& model)
{
   ASSERT(!_isInitialised, "Cannot yet copy assign to a model if it has already been initialised.")
   ASSERT(!model._isInitialised, "Cannot yet copy assign from a model if it has already been initialised.")

   // NOTE: Should NOT overwrite the original buffer IDs of VAO, VBO, and EBO.
   _Mesh            = model._Mesh;
   _TextureInfo     = model._TextureInfo;
   _Material        = model._Material;
   _Actions         = model._Actions;
   _Centroid        = model._Centroid;
   _StrokeColour    = model._StrokeColour;
   _FillColour      = model._FillColour;
   _ModelMatrix     = model._ModelMatrix;
   _PreviousActions = model._PreviousActions;
   _EntryTime       = model._EntryTime;
   _ExitTime        = model._ExitTime;
   _isInitialised   = model._isInitialised;

   return *this;
}

Model&
Model::operator=(Model&& model) noexcept
{
   ASSERT(!_isInitialised, "Cannot yet move assign to a model if it has already been initialised.")
   ASSERT(!model._isInitialised, "Cannot yet move assign from a model if it has already been initialised.")

   // NOTE: Should NOT overwrite the original buffer IDs of VAO, VBO, and EBO.
   _Mesh            = std::move(model._Mesh);
   _TextureInfo     = std::move(model._TextureInfo);
   _Material        = std::move(model._Material);
   _Centroid        = std::move(model._Centroid);
   _StrokeColour    = std::move(model._StrokeColour);
   _FillColour      = std::move(model._FillColour);
   _ModelMatrix     = std::move(model._ModelMatrix);
   _PreviousActions = std::move(model._PreviousActions);
   _EntryTime       = std::move(model._EntryTime);
   _ExitTime        = std::move(model._ExitTime);
   _isInitialised   = std::move(model._isInitialised);
   _Actions         = std::move(model._Actions);

   // Reset moved-from model as it is now in an undefined state. Note: to avoid an infinite regress, we need to specifically invoke the copy assigment operator
   // here, NOT the move assignment operator.
   model = Unmove(Model());

   // Tricky: when moving actions, need to re-assign the 'Actor' member of each action to the current model
   FOR_EACH(_, action, _Actions) action->_Actor = this;

   return *this;
}

/***************************************************************************************************************************************************************
* Model Protected Interface
***************************************************************************************************************************************************************/
void
Model::Init()
{
   if(_isInitialised) return;
   ASSERT(_Mesh.isLoaded(), "The model cannot be initialised without a mesh.")

   // Compute entry/exit times.
   ComputeLifespan();

   // Compute vertex normals.
   _Mesh.ComputeVertexNormals();

   // Initialise VAO, VBO, and EBO.
   _VAO.Init();
   _VBO.Init(_Mesh.Vertices);
   _EBO.Init(_Mesh.Indices);

   // Add vertex buffer to vertex array object.
   _VAO.Bind();
   _VAO.AddBuffer(_VBO, _Mesh.GetVertexLayout());
   _VAO.Unbind();

   _isInitialised = true;
}

void
Model::ComputeLifespan()
{
   _EntryTime = MaxFloat<>;
   _ExitTime  = LowestFloat<>;

   // Compute lifespan of all model actions.
   FOR_EACH(_, action, _Actions)
   {
      _EntryTime = Min(_EntryTime, action->_StartTime);
      _ExitTime  = Max(_ExitTime , action->_EndTime);
   }
}

}
