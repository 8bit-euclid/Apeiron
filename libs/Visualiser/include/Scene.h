#pragma once

#include "../../../include/Global.h"
#include "DataContainer/include/Array.h"
#include "Glyph.h"
#include "Light.h"
#include "Model.h"
#include "Shader.h"
#include "TeXBox.h"
#include "Transition.h"

#include <map>
#include <unordered_map>

namespace aprn::vis {

class Scene
{
 public:
   Scene();

   Scene(Float duration, bool adjust_duration = true);

   Scene(Scene& prev_scene, Float duration = -One, bool adjust_duration = true);

   Scene(const Scene& other) = default;

   Scene(Scene&& other) noexcept = default;

   Scene& Add(Model& model, const std::string& name = "");

   Scene& Add(TeXBox& tex_box, const std::string& name = "");

   Scene& Add(DirectionalLight&& light, const std::string& name = "");

   Scene& Add(PointLight&& light, const std::string& name = "");

   Scene& Add(SpotLight&& light, const std::string& name = "");

   inline bool isCurrent(const Float current_time) const { return _StartTime <= current_time && current_time < _EndTime; }

 private:
   friend class Visualiser;
   friend class Transition;

   void Init(const Float start_time);

   void UpdateModels(const Float current_time);

   void RenderScene(Shader& shader, Camera& camera);

   void RenderDirectionalShadows(Shader& shader);

   void RenderPointShadows(Shader& shader);

   void RenderModels(Shader& shader);

   template<class type> using UMap = std::unordered_map<std::string, type>;
   std::string            _Title;
   UMap<SPtr<Model>>      _Models;
   UMap<SPtr<TeXBox>>     _TeXBoxes;
   UMap<DirectionalLight> _DLights;
   UMap<PointLight>       _PLights;
   UMap<SpotLight>        _SLights;
   UMap<UMap<Texture&>>   _Textures;
   Transition             _Transition{};
   Scene*                 _PrevScene{nullptr};
   Scene*                 _NextScene{nullptr};
   Float                  _Duration;
   Float                  _StartTime;
   Float                  _EndTime;
   bool                   _AdjustDuration{false};
   static bool            _isSingleScene;
};

}