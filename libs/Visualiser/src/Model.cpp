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

#include "../include/Model.h"
#include "../include/Action.h"

namespace aprn::vis {

/***************************************************************************************************************************************************************
* Model Public Interface
***************************************************************************************************************************************************************/
Model::Model()
   : _VAO(), _VBO(), _EBO() {}

Model::Model(const Model& model)
   : Model()
{
   ASSERT(!_isInitialised, "Cannot copy construct a model if it has already been initialised.")
   *this = model;
}

Model::Model(Model&& model) noexcept
   : Model()
{
   ASSERT(!_isInitialised, "Cannot yet move construct a model if it has already been initialised.")
   *this = std::move(model);
}

Model::~Model() { Delete(); }

void
Model::Update(Float global_time)
{
   Reset();
   FOR_EACH(_, action, _Actions) action->Do(global_time);
}

void
Model::Render()
{
   _VAO.Bind();
   _EBO.Bind();

   GLCall(glDrawElements(GL_TRIANGLES, _EBO.IndexCount(), GL_UNSIGNED_INT, nullptr));

   _EBO.Unbind();
   _VAO.Unbind();
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
Model&
Model::SetColour(const SVectorF4& rgb_colour)
{
   FOR_EACH(vertex, _Mesh.Vertices) vertex.Colour = SArrayToGlmVec(rgb_colour);
   return *this;
}

Model&
Model::SetMaterial(const std::string& name, const Float specular_intensity, const Float smoothness)
{
   _Material.emplace(name, specular_intensity, smoothness);
   return *this;
}

Model&
Model::SetTexture(const std::string& material, size_t index, size_t resolution, const Float dispacement_scale)
{
   return SetTexture(material, "", index, resolution ,dispacement_scale);
}

Model&
Model::SetTexture(const std::string& material, const std::string& item, const size_t index, const size_t resolution, const Float dispacement_scale)
{
   _TextureInfo.emplace(std::make_pair(TextureName(material, item, index, resolution), dispacement_scale));
   return *this;
}

Model&
Model::Add(Model& sub_model, const std::string& name) { return Add(std::move(sub_model), name); }

Model&
Model::Add(Model&& sub_model, const std::string& name)
{
   const std::string& id = name.empty() ? "SubModel_" + ToString(_SubModels.size()) : name;
   auto pmodel = std::make_shared<Model>(std::move(sub_model));
   _SubModels[id] = pmodel;
   return *this;
}

/** Set Model Actions
***************************************************************************************************************************************************************/
Model&
Model::OffsetPosition(const SVectorF3& displacement)
{
   SPtr<ActionBase> ptr = std::make_shared<Action<ActionType::OffsetPosition>>(*this, SArrayToGlmVec(displacement));
   _Actions.insert({ActionType::OffsetPosition, ptr});
   return *this;
}

Model&
Model::OffsetOrientation(const Float angle, const SVectorF3& axis)
{
   SPtr<ActionBase> ptr = std::make_shared<Action<ActionType::OffsetOrientation>>(*this, angle, SArrayToGlmVec(axis));
   _Actions.insert({ActionType::OffsetOrientation, ptr});
   return *this;
}

Model&
Model::Scale(const Float factor, const Float start_time, Float end_time, const std::function<Float(Float)>& reparam)
{
   Scale(SVectorF3(factor), start_time, end_time, reparam);
   return *this;
}

Model&
Model::Scale(const SVectorF3& factors, const Float start_time, const Float end_time, const std::function<Float(Float)>& reparam)
{
   SPtr<ActionBase> ptr = std::make_shared<Action<ActionType::Scale>>(*this, SArrayToGlmVec(factors), start_time, end_time);
   _Actions.insert({ActionType::Scale, ptr});
   return *this;
}

Model&
Model::MoveBy(const SVectorF3& displacement, const Float start_time, const Float end_time, const std::function<Float(Float)>& reparam)
{
   SPtr<ActionBase> ptr = std::make_shared<Action<ActionType::MoveBy>>(*this, SArrayToGlmVec(displacement), start_time, end_time);
   _Actions.insert({ActionType::MoveBy, ptr});
   return *this;
}

Model&
Model::MoveTo(const SVectorF3& position, const Float start_time, const Float end_time, const std::function<Float(Float)>& reparam)
{
   SPtr<ActionBase> ptr = std::make_shared<Action<ActionType::MoveTo>>(*this, SArrayToGlmVec(position), start_time, end_time);
   _Actions.insert({ActionType::MoveTo, ptr});
   return *this;
}

//Model&
//Model::MoveAt(const SVectorF3& velocity, Float start_time, const std::function<Float(Float)>& ramp)
//{
//   SPtr<ActionBase> ptr = std::make_shared<Action<ActionType::MoveAt>>(*this, SArrayToGlmVec(position), start_time, end_time);
//   Actions.insert({ActionType::MoveAt, ptr});
//   return *this;
//}

Model&
Model::Trace(std::function<SVectorF3(Float)> path, const Float start_time, const Float end_time)
{
   SPtr<ActionBase> ptr = std::make_shared<Action<ActionType::Trace>>(*this, path, start_time, end_time);
   _Actions.insert({ActionType::Trace, ptr});
   return *this;
}

template<class D>
Model&
Model::Trace(const mnfld::Curve<D, 3>& path, const Float start_time, const Float end_time, const std::function<Float(Float)>& reparam)
{
   SPtr<ActionBase> ptr = std::make_shared<Action<ActionType::Trace>>(*this, path, start_time, end_time);
   _Actions.insert({ActionType::Trace, ptr});
   return *this;
}

Model&
Model::RotateBy(Float angle, const SVectorF3& axis, const Float start_time, const Float end_time, const std::function<Float(Float)>& reparam)
{
   SPtr<ActionBase> ptr = std::make_shared<Action<ActionType::RotateBy>>(*this, angle, SArrayToGlmVec(axis), start_time, end_time);
   _Actions.insert({ActionType::RotateBy, ptr});
   return *this;
}

Model&
Model::RotateAt(const SVectorF3& angular_velocity, const Float start_time, const std::function<Float(Float)>& ramp)
{
   SPtr<ActionBase> ptr = std::make_shared<Action<ActionType::RotateAt>>(*this, SArrayToGlmVec(angular_velocity), start_time, ramp);
   _Actions.insert({ActionType::RotateAt, ptr});
   return *this;
}

Model&
Model::RevolveBy(const Float angle, const SVectorF3& axis, const SVectorF3& refe_point, const Float start_time, const Float end_time,
                 const std::function<Float(Float)>& reparam)
{
   SPtr<ActionBase> ptr =
      std::make_shared<Action<ActionType::RevolveBy>>(*this, angle, SArrayToGlmVec(axis), SArrayToGlmVec(refe_point), start_time, end_time);
   _Actions.insert({ActionType::RevolveBy, ptr});
   return *this;
}

//Model&
//Model::RevolveAt(const SVectorF3& angular_velocity, const SVectorF3& refe_point, Float start_time, const std::function<Float(Float)>& ramp)
//{
//   return <#initializer#>;
//}

/** Assignment Operators
***************************************************************************************************************************************************************/
Model&
Model::operator=(const Model& model)
{
   ASSERT(!_isInitialised, "Cannot yet copy assign to a model if it has already been initialised.")
   ASSERT(!model._isInitialised, "Cannot yet copy assign from a model if it has already been initialised.")

   // NOTE: Should NOT overwrite the original buffer IDs of VAO, VBO, and EBO.
   _Mesh        = model._Mesh;
   _SubModels       = model._SubModels;
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
   _Mesh        = std::move(model._Mesh);
   _SubModels       = std::move(model._SubModels);
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

   // Reset moved-from model as it is now in an undefined state. Note: need to specifically invoke the copy assigment operator here,
   // NOT the move assignment operator.
   model = Unmove(Model());

   // Very tricky: when moving actions, need to re-assign the 'Actor' member of each action to the current model
   FOR_EACH(_, action, _Actions) action->Actor = std::ref(*this);

   return *this;
}


/***************************************************************************************************************************************************************
* Model Protected Interface
***************************************************************************************************************************************************************/
void
Model::Init()
{
   if(_isInitialised || !_Mesh.isInitialised()) return;

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

   // Initialise each sub-model.
   FOR_EACH(_, sub_model, _SubModels) sub_model->Init();

   // Compute entry/exit times.
   ComputeLifespan();

   _isInitialised = true;
}

void
Model::ComputeLifespan()
{
   FOR_EACH(_, action, _Actions)
   {
      _EntryTime = Min(_EntryTime, action->StartTime);
      _ExitTime  = Max(_ExitTime, action->EndTime);
   }
}

void
Model::SetTeXBoxTexture()
{

}

}

