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

   Scene& Add(DirectionalLight& light, const std::string& name = "");

   Scene& Add(PointLight& light, const std::string& name = "");

   Scene& Add(SpotLight& light, const std::string& name = "");

   Scene& Add(Model&& model, const std::string& name = "");

   Scene& Add(TeXBox&& tex_box, const std::string& name = "");

   Scene& Add(DirectionalLight&& light, const std::string& name = "");

   Scene& Add(PointLight&& light, const std::string& name = "");

   Scene& Add(SpotLight&& light, const std::string& name = "");

   inline bool isCurrent(Float current_time) const { return _StartTime <= current_time && current_time < _EndTime; }

 private:
   friend class Visualiser;
   friend class Transition;

   void Init(Float start_time);

   void UpdateModels(Float current_time);

   void RenderDirecShadows(Shader& shader);

   void RenderPointShadows(Shader& shader);

   void RenderScene(Shader& shader, Camera& camera);

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
   inline static bool     _isSingleScene {true};
};

}