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

   void Add(Model& model, const std::string& name = "");

   void Add(DirectionalLight&& light, const std::string& name = "");

   void Add(PointLight&& light, const std::string& name = "");

   void Add(SpotLight&& light, const std::string& name = "");

   inline bool isCurrent(const Float current_time) { return StartTime <= current_time && current_time < EndTime; }

 private:
   friend class Visualiser;
   friend class Transition;

   void Add(std::shared_ptr<Model> model, const std::string& name);

   void Init(const Float start_time);

   void UpdateModels(const Float current_time);

   void RenderScene(Shader& shader, Camera& camera);

   void RenderDirectionalShadows(Shader& shader);

   void RenderPointShadows(Shader& shader);

   void RenderModels(Shader& shader);

   template<class type> using Map = std::unordered_map<std::string, type>;
   std::string               Name;
   Map<SPtr<Model>>          Models;
   Map<DirectionalLight>     DirectionalLights;
   Map<PointLight>           PointLights;
   Map<SpotLight>            SpotLights;
   Map<TeXBox>               TeXBoxes;
   Map<Map<SPtr<Texture>>>   Textures;
   SPtr<Scene>               PrevScene{nullptr};
   SPtr<Scene>               NextScene{nullptr};
   std::optional<Transition> TransitionToNext;
   Float                     Duration;
   Float                     StartTime;
   Float                     EndTime;
   bool                      AdjustDuration{false};
   static bool               isSingleScene;
};

}