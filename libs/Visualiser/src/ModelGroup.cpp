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
ModelGroup::Update(const Real global_time) { FOR_EACH(_, sub_model, SubModels_) sub_model->Update(global_time); }

void
ModelGroup::Render() { FOR_EACH(_, sub_model, SubModels_) sub_model->Render(); }

void
ModelGroup::Delete() { FOR_EACH(_, sub_model, SubModels_) sub_model->Delete(); }

/** Set Model Attributes
***************************************************************************************************************************************************************/
ModelGroup&
ModelGroup::SetColour(const SVectorR4& rgba_colour) { return SetColour(Colour{rgba_colour}); }

ModelGroup&
ModelGroup::SetColour(const Colour& colour)
{
   FOR_EACH(_, sub_model, SubModels_) sub_model->SetColour(colour);
   return *this;
}

ModelGroup&
ModelGroup::SetMaterial(const std::string& name, const Real specular_intensity, const Real smoothness)
{
   FOR_EACH(_, sub_model, SubModels_) sub_model->SetMaterial(name, specular_intensity, smoothness);
   return *this;
}

ModelGroup&
ModelGroup::SetTexture(const std::string& material, size_t index, size_t resolution, const Real dispacement_scale)
{
   SetTexture(material, "", index, resolution ,dispacement_scale);
   return *this;
}

ModelGroup&
ModelGroup::SetTexture(const std::string& material, const std::string& item, const size_t index, const size_t resolution, const Real dispacement_scale)
{
   FOR_EACH(_, sub_model, SubModels_) sub_model->SetTexture(material, item, index, resolution, dispacement_scale);
   return *this;
}

/** Set Model Actions
***************************************************************************************************************************************************************/
ModelGroup&
ModelGroup::OffsetPosition(const SVectorR3& displacement)
{
   FOR_EACH(_, sub_model, SubModels_) sub_model->OffsetPosition(displacement);
   return *this;
}

ModelGroup&
ModelGroup::OffsetOrientation(const Real angle, const SVectorR3& axis)
{
   FOR_EACH(_, sub_model, SubModels_) sub_model->OffsetOrientation(angle, axis);
   return *this;
}

ModelGroup&
ModelGroup::Scale(const Real factor, const Real start_time, Real end_time, const std::function<Real(Real)>& reparam)
{
   Scale(SVectorR3(factor), start_time, end_time, reparam);
   return *this;
}

ModelGroup&
ModelGroup::Scale(const SVectorR3& factors, const Real start_time, const Real end_time, const std::function<Real(Real)>& reparam)
{
   FOR_EACH(_, sub_model, SubModels_) sub_model->Scale(factors, start_time, end_time, reparam);
   return *this;
}

ModelGroup&
ModelGroup::MoveBy(const SVectorR3& displacement, const Real start_time, const Real end_time, const std::function<Real(Real)>& reparam)
{
   FOR_EACH(_, sub_model, SubModels_) sub_model->MoveBy(displacement, start_time, end_time, reparam);
   return *this;
}

ModelGroup&
ModelGroup::MoveTo(const SVectorR3& position, const Real start_time, const Real end_time, const std::function<Real(Real)>& reparam)
{
   FOR_EACH(_, sub_model, SubModels_) sub_model->MoveTo(position, start_time, end_time, reparam);
   return *this;
}

ModelGroup&
ModelGroup::MoveAt(const SVectorR3& velocity, Real start_time, const std::function<Real(Real)>& ramp)
{
   FOR_EACH(_, sub_model, SubModels_) sub_model->MoveAt(velocity, start_time, ramp);
   return *this;
}

ModelGroup&
ModelGroup::Trace(std::function<SVectorR3(Real)> path, const Real start_time, const Real end_time, const std::function<Real(Real)>& reparam)
{
   FOR_EACH(_, sub_model, SubModels_) sub_model->Trace(path, start_time, end_time, reparam);
   return *this;
}

ModelGroup&
ModelGroup::RotateBy(const Real angle, const SVectorR3& axis, const Real start_time, const Real end_time, const std::function<Real(Real)>& reparam)
{
   FOR_EACH(_, sub_model, SubModels_) sub_model->RotateBy(angle, axis, start_time, end_time, reparam);
   return *this;
}

ModelGroup&
ModelGroup::RotateAt(const SVectorR3& angular_velocity, const Real start_time, const std::function<Real(Real)>& ramp)
{
   FOR_EACH(_, sub_model, SubModels_) sub_model->RotateAt(angular_velocity, start_time, ramp);
   return *this;
}

ModelGroup&
ModelGroup::RevolveBy(const Real angle, const SVectorR3& axis, const SVectorR3& refe_point, const Real start_time, const Real end_time,
                      const std::function<Real(Real)>& reparam)
{
   FOR_EACH(_, sub_model, SubModels_) sub_model->RevolveBy(angle, axis, refe_point, start_time, end_time, reparam);
   return *this;
}

ModelGroup&
ModelGroup::RevolveAt(const SVectorR3& angular_velocity, const SVectorR3& refe_point, Real start_time, const std::function<Real(Real)>& ramp)
{
   FOR_EACH(_, sub_model, SubModels_) sub_model->RevolveAt(angular_velocity, refe_point, start_time, ramp);
   return *this;
}

/** Part/sub-model Addition
***************************************************************************************************************************************************************/
ModelGroup&
ModelGroup::Add(Model& part, const std::string& name) { return Add(std::move(part), name); }

ModelGroup&
ModelGroup::Add(Model&& part, const std::string& name)
{
   const std::string& id = name.empty() ? "SubModel_" + ToString(SubModels_.size()) : name;
   SubModels_[id] = std::make_shared<Model>(std::move(part));
   return *this;
}

ModelGroup&
ModelGroup::Add(ModelGroup& sub_model, const std::string& name) { return Add(std::move(sub_model), name); }

ModelGroup&
ModelGroup::Add(ModelGroup&& sub_model, const std::string& name)
{
   const std::string& id = name.empty() ? "SubModel_" + ToString(SubModels_.size()) : name;
   SubModels_[id] = std::make_shared<ModelGroup>(std::move(sub_model));
   return *this;
}

/***************************************************************************************************************************************************************
* Model Protected Interface
***************************************************************************************************************************************************************/
bool
ModelGroup::isInitialised() const
{
   if(!isInitialised_) isInitialised_ = std::all_of(SubModels_.begin(), SubModels_.end(), [](auto& model){ return model.second->isInitialised(); });
   return isInitialised_;
}

void
ModelGroup::Init() { FOR_EACH(_, sub_model, SubModels_) sub_model->Init(); }

void
ModelGroup::ComputeLifespan()
{
   EntryTime_ = MaxFloat<>;
   ExitTime_  = LowestFloat<>;

   // Compute lifespan of all sub-models and add contributions to parent model.
   FOR_EACH(_, sub_model, SubModels_)
   {
      sub_model->ComputeLifespan();
      EntryTime_ = Min(EntryTime_, sub_model->EntryTime());
      ExitTime_  = Max(ExitTime_ , sub_model->ExitTime());
   }
}

}

