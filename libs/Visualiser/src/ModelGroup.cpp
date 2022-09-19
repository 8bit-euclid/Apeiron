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

namespace aprn::vis {

/***************************************************************************************************************************************************************
* Model Public Interface
***************************************************************************************************************************************************************/
ModelGroup::~ModelGroup() { Delete(); }

void
ModelGroup::Update(const Real global_time) { FOR_EACH(_, sub_model, _SubModels) sub_model->Update(global_time); }

void
ModelGroup::Render() { FOR_EACH(_, sub_model, _SubModels) sub_model->Render(); }

void
ModelGroup::Delete() { FOR_EACH(_, sub_model, _SubModels) sub_model->Delete(); }

/** Set Model Attributes
***************************************************************************************************************************************************************/
ModelObject&
ModelGroup::SetColour(const SVectorR4& rgba_colour)
{
   FOR_EACH(_, sub_model, _SubModels) sub_model->SetColour(rgba_colour);
   return *this;
}

ModelObject&
ModelGroup::SetMaterial(const std::string& name, const Real specular_intensity, const Real smoothness)
{
   FOR_EACH(_, sub_model, _SubModels) sub_model->SetMaterial(name, specular_intensity, smoothness);
   return *this;
}

ModelObject&
ModelGroup::SetTexture(const std::string& material, size_t index, size_t resolution, const Real dispacement_scale)
{
   SetTexture(material, "", index, resolution ,dispacement_scale);
   return *this;
}

ModelObject&
ModelGroup::SetTexture(const std::string& material, const std::string& item, const size_t index, const size_t resolution, const Real dispacement_scale)
{
   FOR_EACH(_, sub_model, _SubModels) sub_model->SetTexture(material, item, index, resolution, dispacement_scale);
   return *this;
}

/** Set Model Actions
***************************************************************************************************************************************************************/
ModelObject&
ModelGroup::OffsetPosition(const SVectorR3& displacement)
{
   FOR_EACH(_, sub_model, _SubModels) sub_model->OffsetPosition(displacement);
   return *this;
}

ModelObject&
ModelGroup::OffsetOrientation(const Real angle, const SVectorR3& axis)
{
   FOR_EACH(_, sub_model, _SubModels) sub_model->OffsetOrientation(angle, axis);
   return *this;
}

ModelObject&
ModelGroup::Scale(const Real factor, const Real start_time, Real end_time, const std::function<Real(Real)>& reparam)
{
   Scale(SVectorR3(factor), start_time, end_time, reparam);
   return *this;
}

ModelObject&
ModelGroup::Scale(const SVectorR3& factors, const Real start_time, const Real end_time, const std::function<Real(Real)>& reparam)
{
   FOR_EACH(_, sub_model, _SubModels) sub_model->Scale(factors, start_time, end_time, reparam);
   return *this;
}

ModelObject&
ModelGroup::MoveBy(const SVectorR3& displacement, const Real start_time, const Real end_time, const std::function<Real(Real)>& reparam)
{
   FOR_EACH(_, sub_model, _SubModels) sub_model->MoveBy(displacement, start_time, end_time, reparam);
   return *this;
}

ModelObject&
ModelGroup::MoveTo(const SVectorR3& position, const Real start_time, const Real end_time, const std::function<Real(Real)>& reparam)
{
   FOR_EACH(_, sub_model, _SubModels) sub_model->MoveTo(position, start_time, end_time, reparam);
   return *this;
}

ModelObject&
ModelGroup::MoveAt(const SVectorR3& velocity, Real start_time, const std::function<Real(Real)>& ramp)
{
   FOR_EACH(_, sub_model, _SubModels) sub_model->MoveAt(velocity, start_time, ramp);
   return *this;
}

ModelObject&
ModelGroup::Trace(std::function<SVectorR3(Real)> path, const Real start_time, const Real end_time, const std::function<Real(Real)>& reparam)
{
   FOR_EACH(_, sub_model, _SubModels) sub_model->Trace(path, start_time, end_time, reparam);
   return *this;
}

ModelObject&
ModelGroup::RotateBy(const Real angle, const SVectorR3& axis, const Real start_time, const Real end_time, const std::function<Real(Real)>& reparam)
{
   FOR_EACH(_, sub_model, _SubModels) sub_model->RotateBy(angle, axis, start_time, end_time, reparam);
   return *this;
}

ModelObject&
ModelGroup::RotateAt(const SVectorR3& angular_velocity, const Real start_time, const std::function<Real(Real)>& ramp)
{
   FOR_EACH(_, sub_model, _SubModels) sub_model->RotateAt(angular_velocity, start_time, ramp);
   return *this;
}

ModelObject&
ModelGroup::RevolveBy(const Real angle, const SVectorR3& axis, const SVectorR3& refe_point, const Real start_time, const Real end_time,
                 const std::function<Real(Real)>& reparam)
{
   FOR_EACH(_, sub_model, _SubModels) sub_model->RevolveBy(angle, axis, refe_point, start_time, end_time, reparam);
   return *this;
}

ModelObject&
ModelGroup::RevolveAt(const SVectorR3& angular_velocity, const SVectorR3& refe_point, Real start_time, const std::function<Real(Real)>& ramp)
{
   FOR_EACH(_, sub_model, _SubModels) sub_model->RevolveAt(angular_velocity, refe_point, start_time, ramp);
   return *this;
}

/** Sub-model Addition
***************************************************************************************************************************************************************/
ModelGroup&
ModelGroup::Add(Model& model, const std::string& name)
{
   Add(std::move(model), name);
   return *this;
}

ModelGroup&
ModelGroup::Add(ModelGroup& model_group, const std::string& name)
{
   Add(std::move(model_group), name);
   return *this;
}

ModelGroup&
ModelGroup::Add(Model&& model, const std::string& name)
{
   const std::string& id = name.empty() ? "SubModel_" + ToString(_SubModels.size()) : name;
   _SubModels[id] = std::make_shared<Model>(std::move(model));
   return *this;
}

ModelGroup&
ModelGroup::Add(ModelGroup&& model_group, const std::string& name)
{
   const std::string& id = name.empty() ? "SubModel_" + ToString(_SubModels.size()) : name;
   _SubModels[id] = std::make_shared<ModelGroup>(std::move(model_group));
   return *this;
}

/***************************************************************************************************************************************************************
* Model Protected Interface
***************************************************************************************************************************************************************/
bool
ModelGroup::isInitialised() const
{
   if(!_isInitialised) _isInitialised = std::all_of(_SubModels.begin(), _SubModels.end(), [](auto& model){ return model.second->isInitialised(); });
   return _isInitialised;
}

void
ModelGroup::Init() { FOR_EACH(_, sub_model, _SubModels) sub_model->Init(); }

void
ModelGroup::ComputeLifespan()
{
   _EntryTime = MaxFloat<>;
   _ExitTime  = LowestFloat<>;

   // Compute lifespan of all sub-models and add contributions to parent model.
   FOR_EACH(_, sub_model, _SubModels)
   {
      sub_model->ComputeLifespan();
      _EntryTime = Min(_EntryTime, sub_model->EntryTime());
      _ExitTime  = Max(_ExitTime , sub_model->ExitTime());
   }
}

}

