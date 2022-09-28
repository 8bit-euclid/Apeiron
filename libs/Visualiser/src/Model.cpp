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
   FOR_EACH(_, action, Actions_) action->Do(global_time);
}

void
Model::Render()
{
   if(isInitialised_)
   {
      VAO_.Bind();
      EBO_.Bind();

      GLCall(glDrawElements(GL_TRIANGLES, EBO_.IndexCount(), GL_UNSIGNED_INT, nullptr))

      EBO_.Unbind();
      VAO_.Unbind();
   }
}

void
Model::Delete()
{
   VBO_.Delete();
   VAO_.Delete();
   EBO_.Delete();
}

/** Set Model Attributes
***************************************************************************************************************************************************************/
Model&
Model::SetColour(const SVectorR4& rgba_colour) { return SetColour(Colour{rgba_colour}); }

Model&
Model::SetColour(const Colour& colour)
{
   FOR_EACH(vertex, Mesh_.Vertices) vertex.Colour = SVectorToGlmVec(colour.Values);
   return *this;
}

Model&
Model::SetMaterial(const std::string& name, const Real specular_intensity, const Real smoothness)
{
   Material_.emplace(name, specular_intensity, smoothness);
   return *this;
}

Model&
Model::SetTexture(const std::string& material, const size_t index, const size_t resolution, const Real dispacement_scale)
{
   SetTexture(material, "", index, resolution ,dispacement_scale);
   return *this;
}

Model&
Model::SetTexture(const std::string& material, const std::string& item, const size_t index, const size_t resolution, const Real dispacement_scale)
{
   TextureInfo_.emplace(std::make_pair(TextureName(material, item, index, resolution), dispacement_scale));
   return *this;
}

/** Set Model Actions
***************************************************************************************************************************************************************/
Model&
Model::OffsetPosition(const SVectorR3& displacement)
{
   SPtr<ActionBase> ptr = std::make_shared<Action<ActionType::OffsetPosition>>(*this, SVectorToGlmVec(displacement));
   Actions_.insert({ActionType::OffsetPosition, ptr});
   return *this;
}

Model&
Model::OffsetOrientation(const Real angle, const SVectorR3& axis)
{
   SPtr<ActionBase> ptr = std::make_shared<Action<ActionType::OffsetOrientation>>(*this, angle, SVectorToGlmVec(axis));
   Actions_.insert({ActionType::OffsetOrientation, ptr});
   return *this;
}

Model&
Model::Scale(const Real factor, const Real start_time, const Real end_time, const std::function<Real(Real)>& reparam)
{
   Scale(SVectorR3(factor), start_time, end_time, reparam);
   return *this;
}

Model&
Model::Scale(const SVectorR3& factors, const Real start_time, const Real end_time, const std::function<Real(Real)>& reparam)
{
   SPtr<ActionBase> ptr = std::make_shared<Action<ActionType::Scale>>(*this, SVectorToGlmVec(factors), start_time, end_time);
   Actions_.insert({ActionType::Scale, ptr});
   return *this;
}

Model&
Model::MoveBy(const SVectorR3& displacement, const Real start_time, const Real end_time, const std::function<Real(Real)>& reparam)
{
   SPtr<ActionBase> ptr = std::make_shared<Action<ActionType::MoveBy>>(*this, SVectorToGlmVec(displacement), start_time, end_time);
   Actions_.insert({ActionType::MoveBy, ptr});
   return *this;
}

Model&
Model::MoveTo(const SVectorR3& position, const Real start_time, const Real end_time, const std::function<Real(Real)>& reparam)
{
   SPtr<ActionBase> ptr = std::make_shared<Action<ActionType::MoveTo>>(*this, SVectorToGlmVec(position), start_time, end_time);
   Actions_.insert({ActionType::MoveTo, ptr});
   return *this;
}

Model&
Model::MoveAt(const SVectorR3& velocity, const Real start_time, const std::function<Real(Real)>& ramp)
{
   EXIT("TODO")
   return *this;
}

Model&
Model::Trace(std::function<SVectorR3(Real)> path, const Real start_time, const Real end_time, const std::function<Real(Real)>& reparam)
{
   SPtr<ActionBase> ptr = std::make_shared<Action<ActionType::Trace>>(*this, path, start_time, end_time);
   Actions_.insert({ActionType::Trace, ptr});
   return *this;
}

Model&
Model::RotateBy(const Real angle, const SVectorR3& axis, const Real start_time, const Real end_time, const std::function<Real(Real)>& reparam)
{
   SPtr<ActionBase> ptr = std::make_shared<Action<ActionType::RotateBy>>(*this, angle, SVectorToGlmVec(axis), start_time, end_time);
   Actions_.insert({ActionType::RotateBy, ptr});
   return *this;
}

Model&
Model::RotateAt(const SVectorR3& angular_velocity, const Real start_time, const std::function<Real(Real)>& ramp)
{
   SPtr<ActionBase> ptr = std::make_shared<Action<ActionType::RotateAt>>(*this, SVectorToGlmVec(angular_velocity), start_time, ramp);
   Actions_.insert({ActionType::RotateAt, ptr});
   return *this;
}

Model&
Model::RevolveBy(const Real angle, const SVectorR3& axis, const SVectorR3& refe_point, const Real start_time, const Real end_time,
                 const std::function<Real(Real)>& reparam)
{
   SPtr<ActionBase> ptr =
      std::make_shared<Action<ActionType::RevolveBy>>(*this, angle, SVectorToGlmVec(axis), SVectorToGlmVec(refe_point), start_time, end_time);
   Actions_.insert({ActionType::RevolveBy, ptr});
   return *this;
}

Model&
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
   ASSERT(!isInitialised_, "Cannot yet copy assign to a model if it has already been initialised.")
   ASSERT(!model.isInitialised_, "Cannot yet copy assign from a model if it has already been initialised.")

   // NOTE: Should NOT overwrite the original buffer IDs of VAO, VBO, and EBO.
   Mesh_            = model.Mesh_;
   TextureInfo_     = model.TextureInfo_;
   Material_        = model.Material_;
   Actions_         = model.Actions_;
   Centroid_        = model.Centroid_;
   StrokeColour_    = model.StrokeColour_;
   FillColour_      = model.FillColour_;
   ModelMatrix_     = model.ModelMatrix_;
   PreviousActions_ = model.PreviousActions_;
   EntryTime_       = model.EntryTime_;
   ExitTime_        = model.ExitTime_;
   isInitialised_   = model.isInitialised_;

   return *this;
}

Model&
Model::operator=(Model&& model) noexcept
{
   ASSERT(!isInitialised_, "Cannot yet move assign to a model if it has already been initialised.")
   ASSERT(!model.isInitialised_, "Cannot yet move assign from a model if it has already been initialised.")

   // NOTE: Should NOT overwrite the original buffer IDs of VAO, VBO, and EBO.
   Mesh_            = std::move(model.Mesh_);
   TextureInfo_     = std::move(model.TextureInfo_);
   Material_        = std::move(model.Material_);
   Centroid_        = std::move(model.Centroid_);
   StrokeColour_    = std::move(model.StrokeColour_);
   FillColour_      = std::move(model.FillColour_);
   ModelMatrix_     = std::move(model.ModelMatrix_);
   PreviousActions_ = std::move(model.PreviousActions_);
   EntryTime_       = std::move(model.EntryTime_);
   ExitTime_        = std::move(model.ExitTime_);
   isInitialised_   = std::move(model.isInitialised_);
   Actions_         = std::move(model.Actions_);

   // Reset moved-from model as it is now in an undefined state. Note: to avoid an infinite regress, we need to specifically invoke the copy assigment operator
   // here, NOT the move assignment operator.
   model = Unmove(Model());

   // Tricky: when moving actions, need to re-assign the 'Actor' member of each action to the current model
   FOR_EACH(_, action, Actions_) action->_Actor = this;

   return *this;
}

/***************************************************************************************************************************************************************
* Model Protected Interface
***************************************************************************************************************************************************************/
void
Model::Init()
{
   if(isInitialised_) return;
   ASSERT(Mesh_.isLoaded(), "The model cannot be initialised without a mesh.")

   // Compute entry/exit times.
   ComputeLifespan();

   // Compute vertex normals.
   Mesh_.ComputeVertexNormals();

   // Initialise VAO, VBO, and EBO.
   VAO_.Init();
   VBO_.Init(Mesh_.Vertices);
   EBO_.Init(Mesh_.Indices);

   // Add vertex buffer to vertex array object.
   VAO_.Bind();
   VAO_.AddBuffer(VBO_, Mesh_.GetVertexLayout());
   VAO_.Unbind();

   isInitialised_ = true;
}

void
Model::ComputeLifespan()
{
   EntryTime_ = MaxFloat<>;
   ExitTime_  = LowestFloat<>;

   // Compute lifespan of all model actions.
   FOR_EACH(_, action, Actions_)
   {
      EntryTime_ = Min(EntryTime_, action->_StartTime);
      ExitTime_  = Max(ExitTime_ , action->_EndTime);
   }
}

}
