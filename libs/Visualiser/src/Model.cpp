#include "../include/Model.h"

namespace aprn::vis {

/***************************************************************************************************************************************************************
* Public Interface
***************************************************************************************************************************************************************/
Model::Model()
   : VAO(), VBO(), EBO() {}

Model::Model(const Model& model)
   : Model()
{
   ASSERT(!isInitialised, "Cannot copy construct a model if it has already been initialised.")
   *this = model;
}

Model::Model(Model&& model) noexcept
   : Model()
{
   ASSERT(!isInitialised, "Cannot move construct a model if it has already been initialised.")
   *this = std::move(model);
}

Model::~Model() { Delete(); }

void
Model::Init()
{
   if(isInitialised) return;

   // Compute vertex normals
   Geometry.ComputeVertexNormals();

   // Load vertex and index buffer objects
   VAO.Bind();
   VBO.Init(Geometry.Vertices);
   EBO.Init(Geometry.Indices);

   // Add vertex buffer to vertex array object
   VAO.AddBuffer(VBO, Geometry.GetVertexLayout());
   VAO.Unbind();

   // Compute entry/exit times
   ComputeLifespan();

   isInitialised = true;
}

void
Model::Update(Float global_time)
{
   Reset();
   for(auto& [_, action] : Actions) action->Do(global_time);
}

void
Model::Render()
{
   VAO.Bind();
   EBO.Bind();

   GLCall(glDrawElements(GL_TRIANGLES, EBO.GetIndexCount(), GL_UNSIGNED_INT, nullptr));

   EBO.Unbind();
   VAO.Unbind();
}

void
Model::Delete()
{
   VBO.Delete();
   VAO.Delete();
   EBO.Delete();
}

/** Set Model Attributes
***************************************************************************************************************************************************************/
Model&
Model::SetColour(const SVectorF3& _rgb_colour)
{
   FOR_EACH(vertex, Geometry.Vertices) vertex.Colour = SArrayToGlmVec(_rgb_colour);
   return *this;
}

Model&
Model::SetMaterial(const std::string& name, Float _specular_intensity, Float _smoothness)
{
   MaterialSpec.emplace(name, _specular_intensity, _smoothness);
   return *this;
}

Model&
Model::SetTexture(const std::string& _material, const std::string& _item, size_t _index, size_t _resolution)
{
   TextureSpec.emplace(GetTextureName(_material, _item, _index, _resolution));
   return *this;
}

/** Set Model Actions
***************************************************************************************************************************************************************/
Model&
Model::OffsetPosition(const SVectorF3& _displacement)
{
   SPtr<ActionBase> ptr = std::make_shared<Action<ActionType::OffsetPosition>>(*this, SArrayToGlmVec(_displacement));
   Actions.insert({ActionType::OffsetPosition, ptr});
   return *this;
}

Model&
Model::OffsetOrientation(Float angle, const SVectorF3& axis)
{
   SPtr<ActionBase> ptr = std::make_shared<Action<ActionType::OffsetOrientation>>(*this, angle, SArrayToGlmVec(axis));
   Actions.insert({ActionType::OffsetOrientation, ptr});
   return *this;
}

Model&
Model::Scale(Float _factor, Float start_time, Float end_time, const std::function<Float(Float)>& reparam)
{
   Scale(SVectorF3(_factor), start_time, end_time, reparam);
   return *this;
}

Model&
Model::Scale(const SVectorF3& _factors, Float start_time, Float end_time, const std::function<Float(Float)>& reparam)
{
   SPtr<ActionBase> ptr = std::make_shared<Action<ActionType::Scale>>(*this, SArrayToGlmVec(_factors), start_time, end_time);
   Actions.insert({ActionType::Scale, ptr});
   return *this;
}

Model&
Model::MoveBy(const SVectorF3& _displacement, Float start_time, Float end_time, const std::function<Float(Float)>& reparam)
{
   SPtr<ActionBase> ptr = std::make_shared<Action<ActionType::MoveBy>>(*this, SArrayToGlmVec(_displacement), start_time, end_time);
   Actions.insert({ActionType::MoveBy, ptr});
   return *this;
}

Model&
Model::MoveTo(const SVectorF3& _position, Float start_time, Float end_time, const std::function<Float(Float)>& reparam)
{
   SPtr<ActionBase> ptr = std::make_shared<Action<ActionType::MoveTo>>(*this, SArrayToGlmVec(_position), start_time, end_time);
   Actions.insert({ActionType::MoveTo, ptr});
   return *this;
}

//Model&
//Model::MoveAt(const SVectorF3& velocity, Float start_time, const std::function<Float(Float)>& ramp)
//{
//   SPtr<ActionBase> ptr = std::make_shared<Action<ActionType::MoveAt>>(*this, SArrayToGlmVec(_position), start_time, end_time);
//   Actions.insert({ActionType::MoveAt, ptr});
//   return *this;
//}

Model&
Model::Trace(StaticArray<std::function<Float(Float)>, 3> path, Float start_time, Float end_time)
{
   SPtr<ActionBase> ptr = std::make_shared<Action<ActionType::Trace>>(*this, path, start_time, end_time);
   Actions.insert({ActionType::Trace, ptr});
   return *this;
}

template<class D>
Model&
Model::Trace(const mnfld::Curve<D, 3>& path, Float start_time, Float end_time, const std::function<Float(Float)>& reparam)
{
   SPtr<ActionBase> ptr = std::make_shared<Action<ActionType::Trace>>(*this, path, start_time, end_time);
   Actions.insert({ActionType::Trace, ptr});
   return *this;
}

Model&
Model::RotateBy(Float angle, const SVectorF3& axis, Float start_time, Float end_time, const std::function<Float(Float)>& reparam)
{
   SPtr<ActionBase> ptr = std::make_shared<Action<ActionType::RotateBy>>(*this, angle, SArrayToGlmVec(axis), start_time, end_time);
   Actions.insert({ActionType::RotateBy, ptr});
   return *this;
}

Model&
Model::RotateAt(const SVectorF3& angular_velocity, Float start_time, const std::function<Float(Float)>& ramp)
{
   SPtr<ActionBase> ptr = std::make_shared<Action<ActionType::RotateAt>>(*this, SArrayToGlmVec(angular_velocity), start_time, ramp);
   Actions.insert({ActionType::RotateAt, ptr});
   return *this;
}

Model&
Model::RevolveBy(Float angle, const SVectorF3& axis, const SVectorF3& refe_point, Float start_time, Float end_time,
                 const std::function<Float(Float)>& reparam)
{
   SPtr<ActionBase> ptr =
      std::make_shared<Action<ActionType::RevolveBy>>(*this, angle, SArrayToGlmVec(axis), SArrayToGlmVec(refe_point), start_time, end_time);
   Actions.insert({ActionType::RevolveBy, ptr});
   return *this;
}

//Model&
//Model::RevolveAt(const SVectorF3& angular_velocity, const SVectorF3& refe_point, Float start_time, const std::function<Float(Float)>& ramp)
//{
//   return <#initializer#>;
//}

/** Getters
***************************************************************************************************************************************************************/
const glm::mat4&
Model::GetModelMatrix() const { return ModelMatrix; }

/** Assignment Operators
***************************************************************************************************************************************************************/
Model&
Model::operator=(const Model& model)
{
   ASSERT(!isInitialised, "Cannot yet copy assign a model if it has already been initialised.")

   // NOTE: Should NOT overwrite the original buffer IDs of VAO, VBO, and EBO.
   Geometry        = model.Geometry;
   SubModels       = model.SubModels;
   TextureSpec     = model.TextureSpec;
   MaterialSpec    = model.MaterialSpec;
   Actions         = model.Actions;
   Centroid        = model.Centroid;
   StrokeColour    = model.StrokeColour;
   FillColour      = model.FillColour;
   ModelMatrix     = model.ModelMatrix;
   PreviousActions = model.PreviousActions;
   EntryTime       = model.EntryTime;
   ExitTime        = model.ExitTime;
   isInitialised   = model.isInitialised;

   return *this;
}

Model&
Model::operator=(Model&& model) noexcept
{
   ASSERT(!isInitialised, "Cannot yet move assign a model if it has already been initialised.")

   // NOTE: Should NOT overwrite the original buffer IDs of VAO, VBO, and EBO.
   Geometry        = std::move(model.Geometry);
   SubModels       = std::move(model.SubModels);
   TextureSpec     = std::move(model.TextureSpec);
   MaterialSpec    = std::move(model.MaterialSpec);
   Centroid        = std::move(model.Centroid);
   StrokeColour    = std::move(model.StrokeColour);
   FillColour      = std::move(model.FillColour);
   ModelMatrix     = std::move(model.ModelMatrix);
   PreviousActions = std::move(model.PreviousActions);
   EntryTime       = std::move(model.EntryTime);
   ExitTime        = std::move(model.ExitTime);
   isInitialised   = std::move(model.isInitialised);
   Actions         = std::move(model.Actions);

   // Tricky: when moving actions, need to re-assign the 'Actor' member of each action to the current model
   for(auto& [_, action] : Actions) action->Actor = std::ref(*this);

   return *this;
}


/***************************************************************************************************************************************************************
* Private Interface
***************************************************************************************************************************************************************/
void
Model::ComputeLifespan()
{
   for(auto& [_, action] : Actions)
   {
      EntryTime = Min(EntryTime, action->StartTime);
      ExitTime  = Max(ExitTime,  action->EndTime);
   }
}

void
Model::Reset() { ModelMatrix = glm::mat4(1.0); }

void
Model::Scale(const glm::vec3& factors) { ModelMatrix = glm::scale(ModelMatrix, factors); }

void
Model::Translate(const glm::vec3& displacement) { ModelMatrix = glm::translate(ModelMatrix, displacement); }

void
Model::Rotate(const GLfloat angle, const glm::vec3& axis) { ModelMatrix = glm::rotate(ModelMatrix, angle, axis); }

}

