#ifndef ACTION_TEMPLATE_DEF
#define ACTION_TEMPLATE_DEF

#include "../include/Action.h"
#include "../include/Model.h"

namespace aprn::vis {

/***************************************************************************************************************************************************************
* Action Class Full Specialisations
***************************************************************************************************************************************************************/

///** Ramp Up/Down
//***************************************************************************************************************************************************************/
//template<ActionType type>
//requires Ramp<type>
//Action<type>::Action(Model& _model, RampType _ramp_type, Float _start_time, Float _end_time, std::function<Float(Float)> _reparam)
//   : ActionBase(_model, type, _start_time, _end_time, _reparam), Ramp(_ramp_type) {}
//
//template<ActionType type>
//requires Ramp<type>
//void Action<type>::Do(const Float _global_time)
//{
//
//}

/** Scale
***************************************************************************************************************************************************************/
template<ActionType type>
requires Scale<type>
Action<type>::Action(Model& _model, Float _scale, Float _start_time, Float _end_time, std::function<Float(Float)> _reparam)
   : ActionBase(_model, type, _start_time, _end_time, _reparam), Scales(_scale) {}

template<ActionType type>
requires Scale<type>
Action<type>::Action(Model& _model, const glm::vec3& _scales, Float _start_time, Float _end_time, std::function<Float(Float)> _reparam)
   : ActionBase(_model, type, _start_time, _end_time, _reparam), Scales(_scales) {}

template<ActionType type>
requires Scale<type>
void
Action<type>::Do(const Float _global_time)
{
   const auto param = this->ComputeParameter(_global_time);
   if(param.has_value() && isPositive(param.value())) Actor.Scale(static_cast<float>(param.value()) * Scales);
   else {} // Remove action
}

/** Rotate By/About
***************************************************************************************************************************************************************/
template<ActionType type>
requires Rotate<type>
Action<type>::Action(Model& _model, Float _angle, const glm::vec3& _axis, Float _start_time, Float _end_time, std::function<Float(Float)> _reparam)
   : Action<type>::Action(_model, _angle, _axis, {0.0, 0.0, 0.0}, _start_time, _end_time, _reparam) {}

template<ActionType type>
requires Rotate<type>
Action<type>::Action(Model& _model, Float _angle, const glm::vec3& _axis, const glm::vec3& _ref_point, Float _start_time, Float _end_time,
                     std::function<Float(Float)> _reparam)
   : ActionBase(_model, type, _start_time, _end_time, _reparam), Axis(_axis), Reference(_ref_point)
{
   this->Angle = [_angle](Float _t){ return _t * _angle; };
}

template<ActionType type>
requires Rotate<type>
void
Action<type>::Do(const Float _global_time)
{
   const auto param = this->ComputeParameter(_global_time);
   if(param.has_value() && isPositive(param.value()))
   {
      if(type == ActionType::RotateAbout) Actor.Translate(-Reference);
      Actor.Rotate(this->Angle(param.value()), Axis);
      if(type == ActionType::RotateAbout) Actor.Translate(Reference);
   }
   else {} // Remove action
}

/** Offset
***************************************************************************************************************************************************************/
template<ActionType type>
requires Offset<type>
Action<type>::Action(Model& _model, const glm::vec3& _position_offset)
   : ActionBase(_model, type)
{

}

template<ActionType type>
requires Offset<type>
Action<type>::Action(Model& _model, Float _angle_offset, const glm::vec3& _axis)
   : ActionBase(_model, type)
{

}

template<ActionType type>
requires Offset<type>
void
Action<type>::Do(const Float _global_time)
{

}

///** Reflect
//***************************************************************************************************************************************************************/
//template<ActionType type>
//requires Reflect<type>
//Action<type>::Action(Model& _model, const SVectorF3& _normal, const SVectorF3& _ref_point, Float _start_time, Float _end_time,
//                     std::function<Float(Float)> _reparam)
//   : ActionBase(_model, type, _start_time, _end_time, _reparam), Normal(_normal), ReferencePoint(_ref_point)
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

/** Move By/To/Along
***************************************************************************************************************************************************************/
template<ActionType type>
requires Move<type>
Action<type>::Action(Model& _model, const glm::vec3& _disp_or_posi, Float _start_time, Float _end_time, std::function<Float(Float)> _reparam)
   : ActionBase(_model, type, _start_time, _end_time, _reparam)
{
   switch(type) 
   {
      case ActionType::Move:   this->Displacement = [&_disp_or_posi](Float _t){ return (float)_t * _disp_or_posi; }; break;
      case ActionType::MoveTo: this->Displacement = [&_model, &_disp_or_posi](Float _t){ return (float)_t * (_disp_or_posi - _model.Centroid); }; break;
      default: throw std::invalid_argument("Unrecognised action type.");
   }
}

template<ActionType type>
requires Move<type>
template<class D>
Action<type>::Action(Model& _model, const mnfld::Curve<D, 3>& _path, Float _start_time, Float _end_time, std::function<Float(Float)> _reparam)
   : ActionBase(_model, type, _start_time, _end_time, _reparam)
{
   if(type == ActionType::MoveAlong) this->Displacement = [&_model, &_path](Float _t){ return _path.Point(_t) - _model.Centroid; };
   else throw std::invalid_argument("Unrecognised action type.");
}

template<ActionType type>
requires Move<type>
void
Action<type>::Do(const Float _global_time)
{
   const auto param = this->ComputeParameter(_global_time);
   if(param.has_value() && isPositive(param.value())) Actor.Translate(this->Displacement(param.value()));
   else {} // Remove action
}

///** Morph
//***************************************************************************************************************************************************************/
//template<ActionType type>
//requires Morph<type>
//Action<type>::Action(Model& _model, const Model& _other_actor, Float _start_time, Float _end_time, std::function<Float()> _global_time,
//                     std::function<Float(Float)> _reparam)
//   : ActionBase(_model, _start_time, _end_time, _global_time, _reparam)
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
//Action<type>::Action(Model& _model, Float _new_colour, Float _start_time, Float _end_time, std::function<Float()> _global_time,
//                     std::function<Float(Float)> _reparam)
//   : ActionBase(_model, _start_time, _end_time, _global_time, _reparam)
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

