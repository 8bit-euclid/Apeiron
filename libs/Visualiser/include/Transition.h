#pragma once

#include "../../../include/Global.h"

namespace aprn::vis {

enum class TransitionType
{
   None
};

class Scene;
class Transition
{
 public:
   Transition(TransitionType type, Float duration)
      : Type(type), Duration(duration) {}

 private:
   friend class Visualiser;

   void Init(const Float start_time);

   TransitionType Type{TransitionType::None};
   SPtr<Scene>    FromScene;
   SPtr<Scene>    ToScene;
   Float          Duration{};
   Float          StartTime;
   Float          EndTime;
};

}