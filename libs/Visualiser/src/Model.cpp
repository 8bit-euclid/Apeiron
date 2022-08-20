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

   GLCall(glDrawElements(GL_TRIANGLES, _EBO.GetIndexCount(), GL_UNSIGNED_INT, nullptr));

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
Model::SetColour(const SVectorF3& rgb_colour)
{
   FOR_EACH(vertex, _Mesh.Vertices) vertex.Colour = SArrayToGlmVec(rgb_colour);
   return *this;
}

Model&
Model::SetMaterial(const std::string& name, Float _specular_intensity, Float _smoothness)
{
   _Material.emplace(name, _specular_intensity, _smoothness);
   return *this;
}

Model&
Model::SetTexture(const std::string& material, const std::string& item, size_t index, size_t resolution)
{
   _Texture.emplace(TextureName(material, item, index, resolution));
   return *this;
}

Model&
Model::Add(Model& sub_model, const std::string& name) { return Add(std::move(sub_model), name); }

Model&
Model::Add(Model&& sub_model, const std::string& name)
{
   const std::string& id = name.empty() ? "SubModel_" + ToStr(_SubModels.size()) : name;
   auto pmodel = std::make_shared<Model>(std::move(sub_model));
   pmodel->Init();
   _SubModels[id] = pmodel;
   return *this;
}

/** Set Model Actions
***************************************************************************************************************************************************************/
Model&
Model::OffsetPosition(const SVectorF3& _displacement)
{
   SPtr<ActionBase> ptr = std::make_shared<Action<ActionType::OffsetPosition>>(*this, SArrayToGlmVec(_displacement));
   _Actions.insert({ActionType::OffsetPosition, ptr});
   return *this;
}

Model&
Model::OffsetOrientation(Float angle, const SVectorF3& axis)
{
   SPtr<ActionBase> ptr = std::make_shared<Action<ActionType::OffsetOrientation>>(*this, angle, SArrayToGlmVec(axis));
   _Actions.insert({ActionType::OffsetOrientation, ptr});
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
   _Actions.insert({ActionType::Scale, ptr});
   return *this;
}

Model&
Model::MoveBy(const SVectorF3& _displacement, Float start_time, Float end_time, const std::function<Float(Float)>& reparam)
{
   SPtr<ActionBase> ptr = std::make_shared<Action<ActionType::MoveBy>>(*this, SArrayToGlmVec(_displacement), start_time, end_time);
   _Actions.insert({ActionType::MoveBy, ptr});
   return *this;
}

Model&
Model::MoveTo(const SVectorF3& _position, Float start_time, Float end_time, const std::function<Float(Float)>& reparam)
{
   SPtr<ActionBase> ptr = std::make_shared<Action<ActionType::MoveTo>>(*this, SArrayToGlmVec(_position), start_time, end_time);
   _Actions.insert({ActionType::MoveTo, ptr});
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
   _Actions.insert({ActionType::Trace, ptr});
   return *this;
}

Model&
Model::Trace(std::function<SVectorF3(Float)> path, Float start_time, Float end_time)
{
   SPtr<ActionBase> ptr = std::make_shared<Action<ActionType::Trace>>(*this, path, start_time, end_time);
   _Actions.insert({ActionType::Trace, ptr});
   return *this;
}

template<class D>
Model&
Model::Trace(const mnfld::Curve<D, 3>& path, Float start_time, Float end_time, const std::function<Float(Float)>& reparam)
{
   SPtr<ActionBase> ptr = std::make_shared<Action<ActionType::Trace>>(*this, path, start_time, end_time);
   _Actions.insert({ActionType::Trace, ptr});
   return *this;
}

Model&
Model::RotateBy(Float angle, const SVectorF3& axis, Float start_time, Float end_time, const std::function<Float(Float)>& reparam)
{
   SPtr<ActionBase> ptr = std::make_shared<Action<ActionType::RotateBy>>(*this, angle, SArrayToGlmVec(axis), start_time, end_time);
   _Actions.insert({ActionType::RotateBy, ptr});
   return *this;
}

Model&
Model::RotateAt(const SVectorF3& angular_velocity, Float start_time, const std::function<Float(Float)>& ramp)
{
   SPtr<ActionBase> ptr = std::make_shared<Action<ActionType::RotateAt>>(*this, SArrayToGlmVec(angular_velocity), start_time, ramp);
   _Actions.insert({ActionType::RotateAt, ptr});
   return *this;
}

Model&
Model::RevolveBy(Float angle, const SVectorF3& axis, const SVectorF3& refe_point, Float start_time, Float end_time,
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
   _Mesh            = model._Mesh;
   _SubModels       = model._SubModels;
   _Texture         = model._Texture;
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
   _SubModels       = std::move(model._SubModels);
   _Texture         = std::move(model._Texture);
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

   // Tricky: when moving actions, need to re-assign the 'Actor' member of each action to the current model
   FOR_EACH(_, action, _Actions) action->Actor = std::ref(*this);

   return *this;
}


/***************************************************************************************************************************************************************
* Model Protected Interface
***************************************************************************************************************************************************************/
void
Model::Init()
{
   if(_isInitialised) return;

   // Compute vertex normals
   _Mesh.ComputeVertexNormals();

   // Load vertex and index buffer objects
   _VAO.Bind();
   _VBO.Init(_Mesh.Vertices);
   _EBO.Init(_Mesh.Indices);

   // Add vertex buffer to vertex array object
   _VAO.AddBuffer(_VBO, _Mesh.GetVertexLayout());
   _VAO.Unbind();

   // Compute entry/exit times
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
Model::Reset() { _ModelMatrix = glm::mat4(1.0); }

void
Model::Scale(const glm::vec3& factors) { _ModelMatrix = glm::scale(_ModelMatrix, factors); }

void
Model::Translate(const glm::vec3& displacement) { _ModelMatrix = glm::translate(_ModelMatrix, displacement); }

void
Model::Rotate(const GLfloat angle, const glm::vec3& axis) { _ModelMatrix = glm::rotate(_ModelMatrix, angle, axis); }

}

