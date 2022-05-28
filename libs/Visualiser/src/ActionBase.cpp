#include "../include/ActionBase.h"

namespace aprn::vis {

/***************************************************************************************************************************************************************
* Abstract Action Base Class
***************************************************************************************************************************************************************/
ActionBase::ActionBase(Model& _model, ActionType _action_type)
   : ActionBase(_model, _action_type, -One, -One, nullptr)
{
   ASSERT(_action_type == ActionType::OffsetOrientation || _action_type == ActionType::OffsetPosition, "Only static actions are accepted.")
}

ActionBase::ActionBase(Model& _model, ActionType _action_type, Float _start_time, Float _end_time, std::function<Float(Float)> _reparam)
   : Actor(_model), Type(_action_type), StartTime(_start_time), EndTime(_end_time), ParameterNormaliser(One / (EndTime - StartTime)), Reparametriser(_reparam) {}

std::optional<Float>
ActionBase::ComputeParameter(const Float _global_time)
{
   const auto param =
      _global_time <= EndTime ? std::optional(_global_time < StartTime ? -One : Reparametriser(ParameterNormaliser * (_global_time - StartTime))) : std::nullopt;

   if(param.has_value() && isPositive(param.value()))
   ASSERT((isBounded<true, true, true>(param.value(), Zero, One)), "The parameter must be in the [0, 1] range.")
   return param;
}

}