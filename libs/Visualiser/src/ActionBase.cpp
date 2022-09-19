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

#include "../include/ActionBase.h"
#include "../include/Model.h"

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
ActionBase::ActionBase(Model& model, ActionType action_type)
   : ActionBase(model, action_type, -One, -One, nullptr)
{
   ASSERT(action_type == ActionType::OffsetOrientation || action_type == ActionType::OffsetPosition, "This contructor only accepts static actions.")
}

ActionBase::ActionBase(Model& model, ActionType action_type, Real start_time, Real end_time, std::function<Real(Real)> reparam)
   : _Actor(&model), _Type(action_type), _StartTime(start_time), _EndTime(end_time), _ParamNormaliser(One / (_EndTime - _StartTime)), _Reparametriser(reparam) {}

ActionBase::ActionBase(Model& model, ActionType action_type, Real start_time, std::function<Real(Real)> ramp)
   : _Actor(&model), _Type(action_type), _StartTime(start_time), _EndTime(InfFloat<>), _Ramp(ramp) {}

std::optional<Real>
ActionBase::ComputeParameter(const Real global_time)
{
   const auto local_time = global_time - _StartTime;

   if(isTimeParametrised(_Type)) return std::optional(isNegative(local_time) ? -One : local_time);
   else
   {
      const auto param =
         global_time <= _EndTime ? std::optional(isNegative(local_time) ? -One : _Reparametriser(_ParamNormaliser * local_time)) : std::nullopt;

      if(param.has_value() && isPositive(param.value()))
      ASSERT((isBounded<true, true, true>(param.value(), Zero, One)), "The parameter must be in the [0, 1] range.")
      return param;
   }

}

}