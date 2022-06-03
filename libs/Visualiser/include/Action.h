#pragma once

#include "../../../include/Global.h"
#include "../../Functional/include/Explicit.h"
#include "../../Manifold/include/Curve.h"

#include "ActionBase.h"

#include <functional>
#include <memory>
#include <optional>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace aprn::vis {

/***************************************************************************************************************************************************************
* Concrete Action Class Full Specialisations
***************************************************************************************************************************************************************/
template<ActionType type>
struct Action : public ActionBase
{
   Action(Model& _model) : ActionBase(_model, type, Zero, Zero, nullptr) { EXIT("The given action has not yet been implemented.") }

   void Do(const Float _global_time) override {}
};

///** Ramp
//***************************************************************************************************************************************************************/
//template<ActionType type>
//requires Ramp<type>
//class Action<type> final : public ActionBase
//{
// public:
//   Action(Model& _model, RampType _ramp_type, Float _start_time, Float _end_time, std::function<Float(Float)> _reparam = Linear);
//
//   void Do(const Float _global_time) override;
//
// private:
//   RampType Ramp{RampType::Scale};
//};

/** Scale
***************************************************************************************************************************************************************/
template<ActionType type>
requires Scale<type>
class Action<type> final : public ActionBase
{
 public:
   Action(Model& _model, Float _scale, Float _start_time, Float _end_time, std::function<Float(Float)> _reparam = Linear);

   Action(Model& _model, const glm::vec3& _scales, Float _start_time, Float _end_time, std::function<Float(Float)> _reparam = Linear);

   void Do(const Float _global_time) override;

 private:
   const glm::vec3 Scales{One};
};

/** Rotate
***************************************************************************************************************************************************************/
template<ActionType type>
requires Rotate<type>
class Action<type> final : public ActionBase
{
 public:
   Action(Model& _model, Float _angle, const glm::vec3& _axis, Float _start_time, Float _end_time, std::function<Float(Float)> _reparam = Linear);

   Action(Model& _model, Float _angle, const glm::vec3& _axis, const glm::vec3& _ref_point, Float _start_time, Float _end_time,
          std::function<Float(Float)> _reparam = Linear);

   void Do(const Float _global_time) override;

 private:
   std::function<Float(Float)> Angle;
   const glm::vec3             Axis;
   const glm::vec3             Reference;
};

/** Offset
***************************************************************************************************************************************************************/
template<ActionType type>
requires Offset<type>
class Action<type> final : public ActionBase
{
 public:
   Action(Model& _model, const glm::vec3& _position_offset);

   Action(Model& _model, Float _angle_offset, const glm::vec3& _axis);

   void Do(const Float _global_time) override;

 private:
   glm::vec3 Position;
   Float Angle;
   glm::vec3 Axis;
};

///** Reflect
//***************************************************************************************************************************************************************/
//template<ActionType type>
//requires Reflect<type>
//class Action<type> final : public ActionBase
//{
// public:
//   Action(Model& _model, const SVectorF3& _normal, const SVectorF3& _ref_point, Float _start_time, Float _end_time, std::function<Float(Float)> _reparam = Linear);
//
//   void Do() override;
//
// private:
//   const SVectorF3 Normal;
//   const SVectorF3 ReferencePoint;
//   Action<ActionType::Scale> Scale;
//   Action<ActionType::MoveBy> Move;
//};
//

/** Move
***************************************************************************************************************************************************************/
template<ActionType type>
requires Move<type>
class Action<type> final : public ActionBase
{
 public:
   Action(Model& _model, const glm::vec3& _disp_or_posi, Float _start_time, Float _end_time, std::function<Float(Float)> _reparam = Linear);

   template<class D>
   Action(Model& _model, const mnfld::Curve<D, 3>& _path, Float _start_time, Float _end_time, std::function<Float(Float)> _reparam = Linear);

   void Do(const Float _global_time) override;

 private:
   std::function<glm::vec3(Float)> Displacement;
};

///** Morph
//***************************************************************************************************************************************************************/
//template<ActionType type>
//requires Morph<type>
//class Action<type> final : public ActionBase
//{
// public:
//   Action(Model& _model, const Model& _other_actor, Float _start_time, Float _end_time, std::function<Float(Float)> _reparam = Linear);
//
//   void Do() override;
//
// private:
//   const Model& OtherActor;
//   Action<ActionType::Scale> Scales;
//   Action<ActionType::MoveBy> Move;
//   Action<ActionType::RotateAbout> Rotate;
//};

///** Set Stroke/Fill Colour
//***************************************************************************************************************************************************************/
//template<ActionType type>
//requires SetColour<type>
//class Action<type> final : public ActionBase
//{
// public:
//   Action(Model& _model, Float _new_colour, Float _start_time, Float _end_time, std::function<Float(Float)> _reparam = Linear);
//
//   void
//   Do() override;
//
// private:
//   const SVectorF4 OldColour;
//   const SVectorF4 NewColour;
//};

///** Glow/Blink
//***************************************************************************************************************************************************************/
//template<ActionType type>
//requires SetColour<type>
//class Action<type> final : public ActionBase
//{
// public:
//   Action(Model& _model, Float _new_colour, Float _start_time, Float _end_time, std::function<Float(Float)> _reparam = Linear);
//
//   void
//   Do() override;
//
// private:
//   const SVectorF4 OldColour;
//   const SVectorF4 NewColour;
//};

}

#include "../src/Action.cpp"
