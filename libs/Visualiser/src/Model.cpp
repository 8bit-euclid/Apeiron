#include "../include/Model.h"

namespace aprn::vis {

/***************************************************************************************************************************************************************
* Public Interface
***************************************************************************************************************************************************************/
Model::Model()
   : VAO(), VBO(), EBO(){}

Model::Model(const Model& _model)
   : Model()
{
   ASSERT(!isInitialised, "Cannot copy construct a model if it has already been initialised.")
   *this = _model;
}

Model::Model(Model&& _model) noexcept
   : Model()
{
   ASSERT(!isInitialised, "Cannot move construct a model if it has already been initialised.")
   *this = std::move(_model);
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
Model::Animate(const Float _global_time)
{
   for(auto& [_, action] : Actions) action->Do(_global_time);
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
Model::SetTexture(const std::string& _material, const std::string& _item, size_t _index, size_t _resolution)
{
   TextureSpec.emplace(GetTextureName(_material, _item, _index, _resolution));
   return *this;
}

Model&
Model::SetMaterial(const std::string& _name, Float _specular_intensity, Float _smoothness)
{
   MaterialSpec.emplace(_name, _specular_intensity, _smoothness);
   return *this;
}

/** Set Model Actions
***************************************************************************************************************************************************************/
Model&
Model::Scale(Float _factor, Float _start_time, Float _end_time, const std::function<Float(Float)>& _reparam)
{
   Scale(SVectorF3{_factor}, _start_time, _end_time, _reparam);
   return *this;
}

Model&
Model::Scale(const SVectorF3& _factors, Float _start_time, Float _end_time, const std::function<Float(Float)>& _reparam)
{
   SPtr<ActionBase> ptr = std::make_shared<Action<ActionType::Scale>>(*this, SArrayToGlmVec(_factors), _start_time, _end_time);
   Actions.insert({ActionType::Scale, ptr});
   return *this;
}

Model&
Model::OffsetPosition(const SVectorF3& _displacement)
{
   SPtr<ActionBase> ptr = std::make_shared<Action<ActionType::OffsetPosition>>(*this, SArrayToGlmVec(_displacement));
   Actions.insert({ActionType::OffsetPosition, ptr});
   return *this;
}

Model&
Model::OffsetOrientation(Float _angle, const SVectorF3& _axis)
{
   SPtr<ActionBase> ptr = std::make_shared<Action<ActionType::OffsetOrientation>>(*this, _angle, SArrayToGlmVec(_axis));
   Actions.insert({ActionType::OffsetOrientation, ptr});
   return *this;
}

Model&
Model::Rotate(Float _angle, const SVectorF3& _axis, Float _start_time, Float _end_time, const std::function<Float(Float)>& _reparam)
{
   SPtr<ActionBase> ptr = std::make_shared<Action<ActionType::Rotate>>(*this, _angle, SArrayToGlmVec(_axis), _start_time, _end_time);
   Actions.insert({ActionType::Rotate, ptr});
   return *this;
}

Model&
Model::RotateAbout(Float _angle, const SVectorF3& _axis, const SVectorF3& _refe_point, Float _start_time, Float _end_time,
                        const std::function<Float(Float)>& _reparam)
{
   SPtr<ActionBase> ptr =
      std::make_shared<Action<ActionType::RotateAbout>>(*this, _angle, SArrayToGlmVec(_axis), SArrayToGlmVec(_refe_point), _start_time, _end_time);
   Actions.insert({ActionType::RotateAbout, ptr});
   return *this;
}

Model&
Model::Move(const SVectorF3& _displacement, Float _start_time, Float _end_time, const std::function<Float(Float)>& _reparam)
{
   SPtr<ActionBase> ptr = std::make_shared<Action<ActionType::Move>>(*this, SArrayToGlmVec(_displacement), _start_time, _end_time);
   Actions.insert({ActionType::Move, ptr});
   return *this;
}

Model&
Model::MoveTo(const SVectorF3& _position, Float _start_time, Float _end_time, const std::function<Float(Float)>& _reparam)
{
   SPtr<ActionBase> ptr = std::make_shared<Action<ActionType::MoveTo>>(*this, SArrayToGlmVec(_position), _start_time, _end_time);
   Actions.insert({ActionType::MoveTo, ptr});
   return *this;
}

template<class D>
Model&
Model::MoveAlong(const mnfld::Curve<D, 3>& _path, Float _start_time, Float _end_time, const std::function<Float(Float)>& _reparam)
{
   SPtr<ActionBase> ptr = std::make_shared<Action<ActionType::MoveAlong>>(*this, _path, _start_time, _end_time);
   Actions.insert({ActionType::MoveAlong, ptr});
   return *this;
}

/** Getters
***************************************************************************************************************************************************************/
const glm::mat4&
Model::GetModelMatrix() const { return ModelMatrix; }

/** Assignment Operators
***************************************************************************************************************************************************************/
Model&
Model::operator=(const Model& _model)
{
   ASSERT(!isInitialised, "Cannot copy assign a model if it has already been initialised.")

   Geometry        = _model.Geometry;
   SubModels       = _model.SubModels;
   TextureSpec     = _model.TextureSpec;
   MaterialSpec    = _model.MaterialSpec;
   Actions         = _model.Actions;
   Centroid        = _model.Centroid;
   StrokeColour    = _model.StrokeColour;
   FillColour      = _model.FillColour;
   ModelMatrix     = _model.ModelMatrix;
   PreviousActions = _model.PreviousActions;
   EntryTime       = _model.EntryTime;
   ExitTime        = _model.ExitTime;
   isInitialised   = _model.isInitialised;

   // NOTE: Should NOT overwrite the original buffer IDs of VAO, VBO, and EBO.

   return *this;
}

Model&
Model::operator=(Model&& _model) noexcept
{
   ASSERT(!isInitialised, "Cannot move assign a model if it has already been initialised.")

   Geometry        = std::move(_model.Geometry);
   SubModels       = std::move(_model.SubModels);
   TextureSpec     = std::move(_model.TextureSpec);
   MaterialSpec    = std::move(_model.MaterialSpec);
   Actions         = std::move(_model.Actions);
   Centroid        = std::move(_model.Centroid);
   StrokeColour    = std::move(_model.StrokeColour);
   FillColour      = std::move(_model.FillColour);
   ModelMatrix     = std::move(_model.ModelMatrix);
   PreviousActions = std::move(_model.PreviousActions);
   EntryTime       = std::move(_model.EntryTime);
   ExitTime        = std::move(_model.ExitTime);
   isInitialised   = std::move(_model.isInitialised);

   // NOTE: Should NOT overwrite the original buffer IDs of VAO, VBO, and EBO.

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
Model::Scale(const glm::vec3& _factors) { ModelMatrix = glm::scale(ModelMatrix, _factors); }

void
Model::Translate(const glm::vec3& _displacement) { ModelMatrix = glm::translate(ModelMatrix, _displacement); }

void
Model::Rotate(const GLfloat _angle, const glm::vec3& _axis) { ModelMatrix = glm::rotate(ModelMatrix, _angle, _axis); }

}

