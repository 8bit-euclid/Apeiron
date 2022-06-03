#pragma once

#include "../../../include/Global.h"
#include "../../Functional/include/Explicit.h"
#include "../../Manifold/include/Curve.h"

#include <functional>
#include <memory>
#include <optional>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace aprn::vis {

/** Reparametriser Functors
***************************************************************************************************************************************************************/
constexpr auto Linear = [](Float _t){ return func::Linear(_t, One, Zero); };

//constexpr auto Sigmoid = [](Float _t){ return _t; };
//
//template<size_t shape, size_t location>
//constexpr auto SigmoidMod = [](Float _t)
//                            {
//                               static_assert(isBounded<true, true, true>(location, 0ul, 100ul), "The sigmoid centre location must be a percentage.");
//                               return _t;
//                            };

/** Action Types and Sub-Types
***************************************************************************************************************************************************************/
enum class ActionType // NOTE: The order is important - higher-up actions must be performed first.
{
   /** Static actions, i.e. the model centroid position is unchanged. */
   RampUp,
   RampDown,
   Scale,
   OffsetOrientation,
   Rotate,

   /** Dynamic actions, i.e. the model centroid position is changed. */
   OffsetPosition,
   Reflect,
   RotateAbout,
   ReflectAbout,
   Move,
   MoveTo,
   MoveAlong,
   TrackMotionOf,
   TrackOrientationOf,
   MorphTo,
   MorphFrom,

   /** Appearance-related actions. */
   SetStrokeColour,
   SetFillColour,
   Glow,
   Blink
};

enum class RampType { Trace, Scale, Fade, Blur };
enum class BlinkType { Sine, Triangle, Square };

/** Action Concepts
***************************************************************************************************************************************************************/
typedef ActionType AT;
template<AT type> concept Ramp      = isEnumSame<type, AT::RampUp>()          || isEnumSame<type, AT::RampDown>();
template<AT type> concept Scale     = isEnumSame<type, AT::Scale>();
template<AT type> concept Rotate    = isEnumSame<type, AT::Rotate>()          || isEnumSame<type, AT::RotateAbout>();
template<AT type> concept Offset    = isEnumSame<type, AT::OffsetPosition>()  || isEnumSame<type, AT::OffsetOrientation>();
template<AT type> concept Reflect   = isEnumSame<type, AT::Reflect>()         || isEnumSame<type, AT::ReflectAbout>();
template<AT type> concept Move      = isEnumSame<type, AT::Move>()            || isEnumSame<type, AT::MoveTo>()            || isEnumSame<type, AT::MoveAlong>();
template<AT type> concept Morph     = isEnumSame<type, AT::MorphTo>()         || isEnumSame<type, AT::MorphFrom>();
template<AT type> concept SetColour = isEnumSame<type, AT::SetStrokeColour>() || isEnumSame<type, AT::SetFillColour>();

/** Abstract Action Base Class
***************************************************************************************************************************************************************/
class Model;
class ActionBase
{
 public:
   ActionBase(Model& _model, ActionType _action_type);

   ActionBase(Model& _model, ActionType _action_type, Float _start_time, Float _end_time, std::function<Float(Float)> _reparam = Linear);

   virtual void
   Do(const Float _global_time) = 0;

   inline ActionType
   GetType() const { return Type; }

   inline bool
   isComplete() const { return Status; }

 protected:
   friend class Model;

   std::optional<Float>
   ComputeParameter(const Float _global_time);

   std::reference_wrapper<Model>     Actor;
   ActionType                        Type;
   const Float                       StartTime;
   const Float                       EndTime;
   const Float                       ParameterNormaliser;
   const std::function<Float(Float)> Reparametriser;
   bool Status{false};
};

}
