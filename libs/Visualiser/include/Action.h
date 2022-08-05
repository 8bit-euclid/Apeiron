#pragma once

#include "../../../include/Global.h"
#include "Functional/include/Explicit.h"
#include "Manifold/include/Curve.h"

#include "ActionBase.h"
//#include "Model.h"

#include <functional>
#include <memory>
#include <optional>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace aprn::vis {

class Model;

/***************************************************************************************************************************************************************
* Action Class Full Specialisations
***************************************************************************************************************************************************************/
template<ActionType type>
struct Action : public ActionBase
{
   Action(Model& model) : ActionBase(model, type, Zero, Zero, nullptr) { EXIT("The given action has not yet been implemented.") }

   void Do(const Float global_time) override {}
};

///** Ramp
//***************************************************************************************************************************************************************/
//template<ActionType type>
//requires Ramp<type>
//class Action<type> final : public ActionBase
//{
// public:
//   Action(Model& model, RampType _ramp_type, Float start_time, Float end_time, std::function<Float(Float)> reparam = Linear);
//
//   void Do(const Float global_time) override;
//
// private:
//   RampType Ramp{RampType::Scale};
//};

/** Offset
***************************************************************************************************************************************************************/
template<ActionType type>
requires Offset<type>
class Action<type> final : public ActionBase
{
 public:
   Action(Model& model, const glm::vec3& position_offset);

   Action(Model& model, Float angle_offset, const glm::vec3& axis);

   void Do(const Float global_time) override;

 private:
   glm::vec3 Position;
   Float     Angle;
   glm::vec3 Axis;
};

/** Scale
***************************************************************************************************************************************************************/
template<ActionType type>
requires Scale<type>
class Action<type> final : public ActionBase
{
 public:
   Action(Model& model, Float _scale, Float start_time, Float end_time, std::function<Float(Float)> reparam = Linear);

   Action(Model& model, const glm::vec3& scales, Float start_time, Float end_time, std::function<Float(Float)> reparam = Linear);

   void Do(const Float global_time) override;

 private:
   const glm::vec3 Scales{One};
};

/** Translation
***************************************************************************************************************************************************************/
template<ActionType type>
requires Translation<type>
class Action<type> final : public ActionBase
{
 public:
   Action(Model& model, const glm::vec3& disp_or_posi, Float start_time, Float end_time, std::function<Float(Float)> reparam = Linear);

   Action(Model& model, StaticArray<std::function<Float(Float)>, 3> path, Float start_time, Float end_time);

   Action(Model& model, std::function<SVectorF3(Float)> path, Float start_time, Float end_time);

   template<class D>
   Action(Model& model, const mnfld::Curve<D, 3>& path, Float start_time, Float end_time, std::function<Float(Float)> reparam = Linear);

   void Do(const Float global_time) override;

 private:
   std::function<glm::vec3(Float)> Displacement;
};

/** Rotation
***************************************************************************************************************************************************************/
template<ActionType type>
requires Rotation<type>
class Action<type> final : public ActionBase
{
 public:
   Action(Model& model, Float angle, const glm::vec3& axis, Float start_time, Float end_time, std::function<Float(Float)> reparam = Linear);

   Action(Model& model, Float angle, const glm::vec3& axis, const glm::vec3& ref_point, Float start_time, Float end_time,
          std::function<Float(Float)> reparam = Linear);

   Action(Model& model, const glm::vec3& angular_velocity, Float start_time = Zero, std::function<Float(Float)> ramp = Identity);

   void Do(const Float global_time) override;

 private:
   std::function<Float(Float)> Angle;
   const glm::vec3             Axis;
   const glm::vec3             Reference;
   Float                       AngularSpeed;
};

///** Reflection
//***************************************************************************************************************************************************************/
//template<ActionType type>
//requires Reflection<type>
//class Action<type> final : public ActionBase
//{
// public:
//   Action(Model& model, const SVectorF3& _normal, const SVectorF3& ref_point, Float start_time, Float end_time, std::function<Float(Float)> reparam = Linear);
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

///** Morph
//***************************************************************************************************************************************************************/
//template<ActionType type>
//requires Morph<type>
//class Action<type> final : public ActionBase
//{
// public:
//   Action(Model& model, const Model& _other_actor, Float start_time, Float end_time, std::function<Float(Float)> reparam = Linear);
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
//   Action(Model& model, Float _new_colour, Float start_time, Float end_time, std::function<Float(Float)> reparam = Linear);
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
//   Action(Model& model, Float _new_colour, Float start_time, Float end_time, std::function<Float(Float)> reparam = Linear);
//
//   void
//   Do() override;
//
// private:
//   const SVectorF4 OldColour;
//   const SVectorF4 NewColour;
//};

}

#include "Action.tpp"
