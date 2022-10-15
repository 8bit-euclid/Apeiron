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

#include "../include/Action.h"
#include "../include/Model.h"
#include "../include/Shader.h"

namespace aprn::vis {

/***************************************************************************************************************************************************************
* Model Public Interface
***************************************************************************************************************************************************************/
Model::Model(const Model& other)
{
   ASSERT(!Init_, "Cannot yet copy construct a model if it has already been initialised.")
   *this = other;
}

Model::Model(Model&& other) noexcept
{
   ASSERT(!Init_, "Cannot yet move construct a model if it has already been initialised.")
   *this = std::move(other);
}

Model::~Model() { Delete(); }

void
Model::Update(const Real global_time)
{
   Reset();
   FOR_EACH(_, action, Actions_) action->Do(global_time);
}

void
Model::Render(Shader& shader)
{
   constexpr int slot_offset(3); // TODO - currently hard-coded.

   if(Initialised())
   {
      if(Material_.has_value()) shader.UseMaterial(Material_.value());
      if(!Textures_.empty())
      {
         size_t texture_index = 0;
         FOR_EACH(type_string, texture, Textures_)
         {
            // Configure respective texture uniform.
            const auto& uniform_name = TextureUniformString(type_string);
            shader.UseTexture(texture, "u_" + uniform_name, slot_offset + texture_index++);
            shader.SetUniform1i("u_use_" + uniform_name, 1);

            // Set scale if this is a displacement map.
            if(GetTextureType(type_string) == TextureType::Displacement)
            {
               const auto scale = texture.MapScale();
               shader.SetUniform1f("u_" + uniform_name + "_scale", scale);
            }
         }
      }

      shader.UseModel(*this);
      DrawElements();

      // Switch off texture maps and unbind texture.
      if(TextureRequest_.has_value())
         FOR_EACH(type_string, texture, Textures_)
         {
            shader.SetUniform1i("u_use_" + TextureUniformString(type_string), 0);
            texture.Unbind();
         }
   }
}

void
Model::Delete()
{
   VBO_.Delete();
   VAO_.Delete();
   EBO_.Delete();
   if(SSBO_.has_value()) SSBO_->Delete();
}

/** Set Model Attributes
***************************************************************************************************************************************************************/
Model&
Model::SetColour(const SVectorR4& rgba_colour) { return SetColour(Colour{rgba_colour}); }

Model&
Model::SetColour(const Colour& colour)
{
   FOR_EACH(vertex, Mesh_.Vertices_) vertex.Colour = SVectorToGlmVec(colour.Values);
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
   SetTexture(material, "", index, resolution, dispacement_scale);
   return *this;
}

Model&
Model::SetTexture(const std::string& material, const std::string& item, const size_t index, const size_t resolution, const Real dispacement_scale)
{
   TextureRequest_.emplace(std::make_pair(TextureName(material, item, index, resolution), dispacement_scale));
   return *this;
}

/** Set Model Actions
***************************************************************************************************************************************************************/
Model&
Model::OffsetPosition(const SVectorR3& displacement)
{
   SPtr<ActionBase> ptr = std::make_shared<Action<ActionType::OffsetPosition>>(*this, SVectorToGlmVec(displacement));
   Actions_.insert({ ActionType::OffsetPosition, ptr });
   return *this;
}

Model&
Model::OffsetOrientation(const Real angle, const SVectorR3& axis)
{
   SPtr<ActionBase> ptr = std::make_shared<Action<ActionType::OffsetOrientation>>(*this, angle, SVectorToGlmVec(axis));
   Actions_.insert({ ActionType::OffsetOrientation, ptr });
   return *this;
}

Model&
Model::Scale(const Real factor, const Real start_time, const Real end_time, Reparametriser reparam)
{
   Scale(SVectorR3(factor), start_time, end_time, reparam);
   return *this;
}

Model&
Model::Scale(const SVectorR3& factors, const Real start_time, const Real end_time, Reparametriser reparam)
{
   SPtr<ActionBase> ptr = std::make_shared<Action<ActionType::Scale>>(*this, SVectorToGlmVec(factors), start_time, end_time);
   Actions_.insert({ ActionType::Scale, ptr });
   return *this;
}

Model&
Model::MoveBy(const SVectorR3& displacement, const Real start_time, const Real end_time, Reparametriser reparam)
{
   SPtr<ActionBase> ptr = std::make_shared<Action<ActionType::MoveBy>>(*this, SVectorToGlmVec(displacement), start_time, end_time);
   Actions_.insert({ActionType::MoveBy, ptr});
   return *this;
}

Model&
Model::MoveTo(const SVectorR3& position, const Real start_time, const Real end_time, Reparametriser reparam)
{
   SPtr<ActionBase> ptr = std::make_shared<Action<ActionType::MoveTo>>(*this, SVectorToGlmVec(position), start_time, end_time);
   Actions_.insert({ ActionType::MoveTo, ptr });
   return *this;
}

Model&
Model::MoveAt(const SVectorR3& velocity, const Real start_time, Reparametriser ramp)
{
   EXIT("TODO")
   return *this;
}

Model&
Model::Trace(std::function<SVectorR3(Real)> path, const Real start_time, const Real end_time, Reparametriser reparam)
{
   SPtr<ActionBase> ptr = std::make_shared<Action<ActionType::Trace>>(*this, path, start_time, end_time);
   Actions_.insert({ ActionType::Trace, ptr });
   return *this;
}

Model&
Model::RotateBy(const Real angle, const SVectorR3& axis, const Real start_time, const Real end_time, Reparametriser reparam)
{
   SPtr<ActionBase> ptr = std::make_shared<Action<ActionType::RotateBy>>(*this, angle, SVectorToGlmVec(axis), start_time, end_time);
   Actions_.insert({ ActionType::RotateBy, ptr });
   return *this;
}

Model&
Model::RotateAt(const SVectorR3& angular_velocity, const Real start_time, Reparametriser ramp)
{
   SPtr<ActionBase> ptr = std::make_shared<Action<ActionType::RotateAt>>(*this, SVectorToGlmVec(angular_velocity), start_time, ramp);
   Actions_.insert({ ActionType::RotateAt, ptr });
   return *this;
}

Model&
Model::RevolveBy(const Real angle, const SVectorR3& axis, const SVectorR3& refe_point, const Real start_time, const Real end_time,
                 Reparametriser reparam)
{
   SPtr<ActionBase> ptr =
      std::make_shared<Action<ActionType::RevolveBy>>(*this, angle, SVectorToGlmVec(axis), SVectorToGlmVec(refe_point), start_time, end_time);
   Actions_.insert({ ActionType::RevolveBy, ptr });
   return *this;
}

Model&
Model::RevolveAt(const SVectorR3& angular_velocity, const SVectorR3& refe_point, const Real start_time, Reparametriser ramp)
{
   EXIT("TODO")
   return *this;
}

/** Assignment Operators
***************************************************************************************************************************************************************/
Model&
Model::operator=(const Model& model)
{
   ASSERT(!Init_,       "Cannot yet copy assign to a model if it has already been initialised.")
   ASSERT(!model.Init_, "Cannot yet copy assign from a model if it has already been initialised.")

   // NOTE: Should NOT overwrite the original buffer IDs of VAO, VBO, and EBO.
   Mesh_           = model.Mesh_;
   TextureRequest_ = model.TextureRequest_;
   Material_       = model.Material_;
   Actions_        = model.Actions_;
   Centroid_       = model.Centroid_;
   StrokeColour_   = model.StrokeColour_;
   FillColour_     = model.FillColour_;
   ModelMatrix_    = model.ModelMatrix_;
   PastActions_    = model.PastActions_;
   EntryTime_      = model.EntryTime_;
   ExitTime_       = model.ExitTime_;
   Init_           = model.Init_;

   return *this;
}

Model&
Model::operator=(Model&& model) noexcept
{
   ASSERT(!Init_,       "Cannot yet move assign to a model if it has already been initialised.")
   ASSERT(!model.Init_, "Cannot yet move assign from a model if it has already been initialised.")

   // NOTE: Should NOT overwrite the original buffer IDs of VAO, VBO, and EBO.
   Mesh_           = std::move(model.Mesh_);
   TextureRequest_ = std::move(model.TextureRequest_);
   Material_       = std::move(model.Material_);
   Centroid_       = std::move(model.Centroid_);
   StrokeColour_   = std::move(model.StrokeColour_);
   FillColour_     = std::move(model.FillColour_);
   ModelMatrix_    = std::move(model.ModelMatrix_);
   PastActions_    = std::move(model.PastActions_);
   EntryTime_      = std::move(model.EntryTime_);
   ExitTime_       = std::move(model.ExitTime_);
   Init_           = std::move(model.Init_);
   Actions_        = std::move(model.Actions_);

   // Reset moved-from model as it is now in an undefined state. Note: to avoid an infinite regress, we need to specifically invoke the copy assigment operator
   // here, NOT the move assignment operator.
   model = Unmove(Model());

   // Tricky: when moving actions, need to re-assign the 'Actor' member of each action to the current model
   FOR_EACH(_, action, Actions_) action->Actor_ = this;

   return *this;
}

/***************************************************************************************************************************************************************
* Model Protected Interface
***************************************************************************************************************************************************************/
void
Model::Init()
{
   if(Init_) return;
   ASSERT(Mesh_.Loaded(), "The model cannot be initialised without a mesh.")

   // Compute entry/exit times.
   ComputeLifespan();

   // Compute vertex normals.
   Mesh_.ComputeVertexNormals();

   // Initialise VAO, VBO, and EBO.
   VAO_.Init();
   VBO_.Init(Mesh_.Vertices_);
   EBO_.Init(Mesh_.Indices_);

   // Add vertex buffer to vertex array object.
   VAO_.Bind();
   VAO_.AddBuffer(VBO_, Mesh_.GetVertexLayout());
   VAO_.Unbind();

   Init_ = true;
}

void
Model::ComputeLifespan()
{
   EntryTime_ = MaxFloat<>;
   ExitTime_  = LowestFloat<>;

   // Compute lifespan of all model actions.
   FOR_EACH(_, action, Actions_)
   {
      EntryTime_ = Min(EntryTime_, action->StartTime_);
      ExitTime_  = Max(ExitTime_ , action->EndTime_);
   }
}

void
Model::LoadTextureMap(const std::unordered_map<std::string, Texture&>& texture_map) { Textures_ = texture_map; }

void
Model::DrawElements() const
{
   if(Init_)
   {
      VAO_.Bind();
      EBO_.Bind();
      GLCall(glDrawElements(GL_TRIANGLES, EBO_.IndexCount(), GL_UNSIGNED_INT, nullptr))
      EBO_.Unbind();
      VAO_.Unbind();
   }
}

}
