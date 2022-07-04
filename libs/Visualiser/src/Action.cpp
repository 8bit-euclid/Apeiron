#ifndef ACTION_TEMPLATE_DEF
#define ACTION_TEMPLATE_DEF

#include "../include/Action.h"
#include "../include/Model.h"
#include "../include/GLTypes.h"

namespace aprn::vis {

/***************************************************************************************************************************************************************
* Action Class Full Specialisations
***************************************************************************************************************************************************************/

///** Ramp
//***************************************************************************************************************************************************************/
//template<ActionType type>
//requires Ramp<type>
//Action<type>::Action(Model& model, RampType _ramp_type, Float start_time, Float end_time, std::function<Float(Float)> reparam)
//   : ActionBase(model, type, start_time, end_time, reparam), Ramp(_ramp_type) {}
//
//template<ActionType type>
//requires Ramp<type>
//void Action<type>::Do(const Float global_time)
//{
//
//}

/** Offset
***************************************************************************************************************************************************************/
template<ActionType type>
requires Offset<type>
Action<type>::Action(Model& model, const glm::vec3& position_offset)
   : ActionBase(model, type), Position(position_offset) {}

template<ActionType type>
requires Offset<type>
Action<type>::Action(Model& model, Float angle_offset, const glm::vec3& axis)
   : ActionBase(model, type), Angle(angle_offset), Axis(axis) {}

template<ActionType type>
requires Offset<type>
void
Action<type>::Do(const Float global_time)
{
   if(type == ActionType::OffsetPosition) Actor.get().Translate(Position);
   else if(type == ActionType::OffsetOrientation) Actor.get().Rotate(Angle, Axis);
}

/** Scale
***************************************************************************************************************************************************************/
template<ActionType type>
requires Scale<type>
Action<type>::Action(Model& model, Float _scale, Float start_time, Float end_time, std::function<Float(Float)> reparam)
   : ActionBase(model, type, start_time, end_time, reparam), Scales(_scale) {}

template<ActionType type>
requires Scale<type>
Action<type>::Action(Model& model, const glm::vec3& scales, Float start_time, Float end_time, std::function<Float(Float)> reparam)
   : ActionBase(model, type, start_time, end_time, reparam), Scales(scales) {}

template<ActionType type>
requires Scale<type>
void
Action<type>::Do(const Float global_time)
{
   const auto param = this->ComputeParameter(global_time);
   if(param.has_value() && isPositive(param.value())) Actor.get().Scale(static_cast<float>(param.value()) * Scales);
   else {} // Remove action
}

/** Translation
***************************************************************************************************************************************************************/
template<ActionType type>
requires Translation<type>
Action<type>::Action(Model& model, const glm::vec3& disp_or_posi, Float start_time, Float end_time, std::function<Float(Float)> reparam)
   : ActionBase(model, type, start_time, end_time, reparam)
{
   switch(type)
   {
      case ActionType::MoveBy: this->Displacement = [&disp_or_posi](Float t){ return (float)t * disp_or_posi; }; break;
      case ActionType::MoveTo: this->Displacement = [&model, &disp_or_posi](Float t){ return (float)t * (disp_or_posi - model.Centroid); }; break;
      default: throw std::invalid_argument("Unrecognised action type.");
   }
}

template<ActionType type>
requires Translation<type>
Action<type>::Action(Model& model, StaticArray<std::function<Float(Float)>, 3> path, Float start_time, Float end_time)
   : ActionBase(model, type, start_time, end_time, [](Float){return Zero;})
{
   if(type == ActionType::Trace) this->Displacement = [path](Float t){ return glm::vec3(path[0](t), path[1](t), path[2](t)); };
   else throw std::invalid_argument("Unrecognised action type.");
}

template<ActionType type>
requires Translation<type>
Action<type>::Action(Model& model, std::function<SVectorF3(Float)> path, Float start_time, Float end_time)
   : ActionBase(model, type, start_time, end_time, [](Float){return Zero;})
{
   if(type == ActionType::Trace) this->Displacement = [path](Float t){ return SArrayToGlmVec(path(t)); };
   else throw std::invalid_argument("Unrecognised action type.");
}

template<ActionType type>
requires Translation<type>
template<class D>
Action<type>::Action(Model& model, const mnfld::Curve<D, 3>& path, Float start_time, Float end_time, std::function<Float(Float)> reparam)
   : ActionBase(model, type, start_time, end_time, reparam)
{
   if(type == ActionType::Trace) this->Displacement = [centroid = model.Centroid, path](Float t){ return path.Point(t) - centroid; };
   else throw std::invalid_argument("Unrecognised action type.");
}

template<ActionType type>
requires Translation<type>
void
Action<type>::Do(const Float global_time)
{
   const auto param = this->ComputeParameter(global_time);
   if(param.has_value() && isPositive(param.value())) Actor.get().Translate(this->Displacement(param.value()));
   else {} // Remove action
}

/** Rotation
***************************************************************************************************************************************************************/
template<ActionType type>
requires Rotation<type>
Action<type>::Action(Model& model, Float angle, const glm::vec3& axis, Float start_time, Float end_time, std::function<Float(Float)> reparam)
   : Action<type>::Action(model, angle, axis, {0.0, 0.0, 0.0}, start_time, end_time, reparam) {}

template<ActionType type>
requires Rotation<type>
Action<type>::Action(Model& model, Float angle, const glm::vec3& axis, const glm::vec3& ref_point, Float start_time, Float end_time,
                     std::function<Float(Float)> reparam)
   : ActionBase(model, type, start_time, end_time, reparam), Axis(axis), Reference(ref_point)
{
   this->Angle = [angle](Float t){ return t * angle; };
}

template<ActionType type>
requires Rotation<type>
Action<type>::Action(Model& model, const glm::vec3& angular_velocity, Float start_time, std::function<Float(Float)> ramp)
   : ActionBase(model, type, start_time, ramp), Axis(glm::normalize(angular_velocity)), Reference({0.0, 0.0, 0.0}),
     AngularSpeed(glm::length(angular_velocity))
{
   this->Angle = [this](Float t){ return t * AngularSpeed * Ramp(t); };
}

template<ActionType type>
requires Rotation<type>
void
Action<type>::Do(const Float global_time)
{
   const auto param = this->ComputeParameter(global_time);
   if(param.has_value() && isPositive(param.value()))
   {
      const bool is_revolution = type == ActionType::RevolveBy || type == ActionType::RevolveAt;
      if(is_revolution) Actor.get().Translate(-Reference);
      Actor.get().Rotate(this->Angle(param.value()), Axis);
      if(is_revolution) Actor.get().Translate(Reference);
   }
   else {} // Remove action
}

///** Reflection
//***************************************************************************************************************************************************************/
//template<ActionType type>
//requires Reflect<type>
//Action<type>::Action(Model& model, const SVectorF3& _normal, const SVectorF3& ref_point, Float start_time, Float end_time,
//                     std::function<Float(Float)> reparam)
//   : ActionBase(model, type, start_time, end_time, reparam), Normal(_normal), ReferencePoint(ref_point)
//{
//
//}
//
//template<ActionType type>
//requires Reflect<type>
//void Action<type>::Do()
//{
//
//}
//

///** Morph
//***************************************************************************************************************************************************************/
//template<ActionType type>
//requires Morph<type>
//Action<type>::Action(Model& model, const Model& _other_actor, Float start_time, Float end_time, std::function<Float()> global_time,
//                     std::function<Float(Float)> reparam)
//   : ActionBase(model, start_time, end_time, global_time, reparam)
//{
//
//}
//
//template<ActionType type>
//requires Morph<type>
//void Action<type>::Do()
//{
//
//}
//
///** Set Stroke/Fill Colour
//***************************************************************************************************************************************************************/
//template<ActionType type>
//requires SetColour<type>
//Action<type>::Action(Model& model, Float _new_colour, Float start_time, Float end_time, std::function<Float()> global_time,
//                     std::function<Float(Float)> reparam)
//   : ActionBase(model, start_time, end_time, global_time, reparam)
//{
//
//}
//
//template<ActionType type>
//requires SetColour<type>
//void Action<type>::Do()
//{
//
//}

}

#endif //ACTION_TEMPLATE_DEF

