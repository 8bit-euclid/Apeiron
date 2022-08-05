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
   Transition() = default;

   Transition(TransitionType type, Float duration)
      : _Type(type), _Duration(duration) {}

 private:
   friend class Visualiser;

   void Init(const Float start_time);

   TransitionType _Type{TransitionType::None};
   SPtr<Scene>    _FromScene;
   SPtr<Scene>    _ToScene;
   Float          _Duration{};
   Float          _StartTime;
   Float          _EndTime;
};

}