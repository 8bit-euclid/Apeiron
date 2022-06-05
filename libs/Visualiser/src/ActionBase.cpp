#include "../include/ActionBase.h"

namespace aprn::vis {

bool
isTimeParametrised(ActionType type)
{
   return type == ActionType::RotateAt ||
          type == ActionType::MoveAt ||
          type == ActionType::Trace;
}

/***************************************************************************************************************************************************************
* Abstract Action Base Class
***************************************************************************************************************************************************************/
ActionBase::ActionBase(Model& model, ActionType _action_type)
   : ActionBase(model, _action_type, -One, -One, nullptr)
{
   ASSERT(_action_type == ActionType::OffsetOrientation || _action_type == ActionType::OffsetPosition, "Only static actions are accepted.")
}

ActionBase::ActionBase(Model& model, ActionType _action_type, Float start_time, Float end_time, std::function<Float(Float)> reparam)
   : Actor(model), Type(_action_type), StartTime(start_time), EndTime(end_time), ParameterNormaliser(One / (EndTime - StartTime)), Reparametriser(reparam) {}

ActionBase::ActionBase(Model& model, ActionType _action_type, Float start_time, std::function<Float(Float)> ramp)
   : Actor(model), Type(_action_type), StartTime(start_time), EndTime(InfFloat<>), Ramp(ramp) {}

std::optional<Float>
ActionBase::ComputeParameter(const Float global_time)
{
   const auto local_time = global_time - StartTime;

   if(isTimeParametrised(Type)) return std::optional(isNegative(local_time) ? -One : local_time);
   else
   {
      const auto param =
         global_time <= EndTime ? std::optional(isNegative(local_time) ? -One : Reparametriser(ParameterNormaliser * local_time)) : std::nullopt;

      if(param.has_value() && isPositive(param.value()))
      ASSERT((isBounded<true, true, true>(param.value(), Zero, One)), "The parameter must be in the [0, 1] range.")
      return param;
   }

}

}