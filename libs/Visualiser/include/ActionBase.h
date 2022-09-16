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

#pragma once

#include "../../../include/Global.h"
#include "Functional/include/Explicit.h"
#include "Manifold/include/Curve.h"
//#include "Model.h"

#include <functional>
#include <memory>
#include <optional>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace aprn::vis {

/** Reparametriser/Ramp Functors
***************************************************************************************************************************************************************/
constexpr auto Identity = [](Real t){ return One; };
constexpr auto Linear   = [](Real t){ return func::Linear(t, One, Zero); };

//constexpr auto Sigmoid = [](Real t){ return t; };
//
//template<size_t shape, size_t location>
//constexpr auto SigmoidMod = [](Real t)
//                            {
//                               static_assert(isBounded<true, true, true>(location, 0ul, 100ul), "The sigmoid centre location must be a percentage.");
//                               return t;
//                            };

/** Action Types and Sub-Types
***************************************************************************************************************************************************************/
enum class ActionType // NOTE: The order here is important - higher-up actions must be performed first.
{
   /** Model centroid invariant actions. */
   RampUp,
   RampDown,
   Scale,
   OffsetOrientation,
   RotateBy,
   RotateAt,

   /** Model centroid variant actions. */
   OffsetPosition,
   Reflect,
   RevolveBy,
   RevolveAt,
   MoveBy,
   MoveTo,
   MoveAt,
   Trace,
   TrackPositionOf,
   TrackOrientationOf,
   MorphTo,
   MorphFrom,

   /** Model appearance-related actions. */
   SetStrokeColour,
   SetFillColour,
   Glow,
   Blink
};

enum class RampType { Trace, Scale, Fade, Blur };
enum class BlinkType { Sine, Triangle, Square };

/** NOTE: This custom comparator must order the above actions in reverse order, as OpenGL post-multiplies the model matrix for each new action. */
struct ActionTypeComparator { bool operator()(const ActionType& a, const ActionType& b) const { return static_cast<size_t>(a) > static_cast<size_t>(b); } };

bool isTimeParametrised(ActionType type);

/** Action Concepts
***************************************************************************************************************************************************************/
typedef ActionType AT;
template<AT type> concept Ramp        = isEnumSame<type, AT::RampUp>() ||
                                        isEnumSame<type, AT::RampDown>();
template<AT type> concept Scale       = isEnumSame<type, AT::Scale>();
template<AT type> concept Rotation    = isEnumSame<type, AT::RotateBy>() ||
                                        isEnumSame<type, AT::RevolveBy>() ||
                                        isEnumSame<type, AT::RotateAt>() ||
                                        isEnumSame<type, AT::RevolveAt>();
template<AT type> concept Offset      = isEnumSame<type, AT::OffsetPosition>() ||
                                        isEnumSame<type, AT::OffsetOrientation>();
template<AT type> concept Reflection  = isEnumSame<type, AT::Reflect>();
template<AT type> concept Translation = isEnumSame<type, AT::MoveBy>() ||
                                        isEnumSame<type, AT::MoveTo>() ||
                                        isEnumSame<type, AT::MoveAt>() ||
                                        isEnumSame<type, AT::Trace>();
template<AT type> concept Morph       = isEnumSame<type, AT::MorphTo>() ||
                                        isEnumSame<type, AT::MorphFrom>();
template<AT type> concept SetColour   = isEnumSame<type, AT::SetStrokeColour>() ||
                                        isEnumSame<type, AT::SetFillColour>();

/** Abstract Action Base Class
***************************************************************************************************************************************************************/
class Model;
class ActionBase
{
 public:
   ActionBase(Model& model, ActionType action_type);

   ActionBase(Model& model, ActionType action_type, Real start_time, Real end_time, std::function<Real(Real)> reparam = Linear);

   ActionBase(Model& model, ActionType action_type, Real start_time, std::function<Real(Real)> ramp = Identity);

   virtual void
   Do(const Real global_time) = 0;

   inline auto Type() const { return _Type; }

   inline bool isDone() const { return _isDone; }

 protected:
   friend class Model;

   std::optional<Real>
   ComputeParameter(const Real global_time);

   RWpr<Model>               _Actor;
   ActionType                _Type;
   const Real                _StartTime;
   const Real                _EndTime;
   Real                      _ParamNormaliser;
   std::function<Real(Real)> _Reparametriser;
   std::function<Real(Real)> _Ramp;
   bool                      _isDone{false};
};

}
