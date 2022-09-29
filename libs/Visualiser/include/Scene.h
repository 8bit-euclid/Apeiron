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
#include "TeXGlyph.h"
#include "Light.h"
#include "ModelObject.h"
#include "Model.h"
#include "ModelGroup.h"
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

   Scene(Real duration, bool adjust_duration = true);

   Scene(Scene& prev_scene, Real duration = -One, bool adjust_duration = true);

   Scene(const Scene& other) = default;

   Scene(Scene&& other) noexcept = default;

   Scene& Add(Model& model, const std::string& name = "");

   Scene& Add(ModelGroup& model_group, const std::string& name = "");

   Scene& Add(TeXGlyph& tex_glyph, const std::string& name = "");

   Scene& Add(TeXBox& tex_box, const std::string& name = "");

   Scene& Add(DirectionalLight& light, const std::string& name = "");

   Scene& Add(PointLight& light, const std::string& name = "");

   Scene& Add(SpotLight& light, const std::string& name = "");

   Scene& Add(Model&& model, const std::string& name = "");

   Scene& Add(ModelGroup&& model, const std::string& name = "");

   Scene& Add(TeXGlyph&& tex_glyph, const std::string& name = "");

   Scene& Add(TeXBox&& tex_box, const std::string& name = "");

   Scene& Add(DirectionalLight&& light, const std::string& name = "");

   Scene& Add(PointLight&& light, const std::string& name = "");

   Scene& Add(SpotLight&& light, const std::string& name = "");

   inline bool isCurrent(const Real current_time) const { return StartTime_ <= current_time && current_time < EndTime_; }

 private:
   friend class Visualiser;
   friend class Transition;

   void Init(Real start_time);

   void UpdateModels(Real current_time);

   void RenderDirecShadows(Shader& shader);

   void RenderPointShadows(Shader& shader);

   void RenderScene(Shader& shader, Camera& camera);

   void RenderModels(Shader& shader);

   void RenderModel(SPtr<ModelGroup>& model, Shader& shader);

   template<class type> using UMap = std::unordered_map<std::string, type>;
   std::string             Title_;
   UMap<SPtr<ModelObject>> Actors_;
   UMap<SPtr<TeXBox>>      TeXBoxes_;
   UMap<DirectionalLight>  DLights_;
   UMap<PointLight>        PLights_;
   UMap<SpotLight>         SLights_;
   UMap<UMap<Texture&>>    Textures_;
   Transition              Transition_{};
   Scene*                  PrevScene_{};
   Scene*                  NextScene_{};
   Real                    Duration_;
   Real                    StartTime_;
   Real                    EndTime_;
   bool                    AdjustDuration_{false};
   inline static bool      SingleScene_{true};
};

}