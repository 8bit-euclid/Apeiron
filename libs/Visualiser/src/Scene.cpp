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

#include "../include/Scene.h"

namespace aprn::vis {

/***************************************************************************************************************************************************************
* Public Interface
***************************************************************************************************************************************************************/
Scene::Scene()
   : Scene(1000.0, true) {}

Scene::Scene(Real duration, bool adjust_duration)
   : Duration_(duration), AdjustDuration_(adjust_duration)
{
   ASSERT(isPositive(duration) || adjust_duration, "Cannot have a negative duration for a scene unless the final duration is to be computed.")
   ASSERT(SingleScene_, "This constructor should only be called for the first scene.")

   SingleScene_ = false;
}

Scene::Scene(Scene& prev_scene, Real duration, bool adjust_duration)
   : Duration_(duration), AdjustDuration_(adjust_duration)
{
   ASSERT(Zero < duration      , "Cannot have a negative duration for a scene.")
   ASSERT(!SingleScene_      , "This constructor should not be called for the first scene.")
   ASSERT(PrevScene_           , "The current scene has already been assigned a previous scene.")
   ASSERT(prev_scene.NextScene_, "The previous scene has already been assigned a next scene.")

   PrevScene_ = &prev_scene;
   prev_scene.NextScene_ = this;
}

Scene&
Scene::Add(Model& model, const std::string& name) { return Add(std::move(model), name); }

Scene&
Scene::Add(ModelGroup& model_group, const std::string& name) { return Add(std::move(model_group), name); }

Scene&
Scene::Add(TeXGlyph& tex_glyph, const std::string& name) { return Add(std::move(tex_glyph), name); }

Scene&
Scene::Add(TeXBox& tex_box, const std::string& name) { return Add(std::move(tex_box), name); }

Scene&
Scene::Add(DirectionalLight& light, const std::string& name) { return Add(std::move(light), name); }

Scene&
Scene::Add(PointLight& light, const std::string& name) { return Add(std::move(light), name); }

Scene&
Scene::Add(SpotLight& light, const std::string& name) { return Add(std::move(light), name); }

Scene&
Scene::Add(Model&& model, const std::string& name)
{
   const std::string& id = name.empty() ? "Model_" + ToString(Actors_.size()) : name;
   Actors_.emplace(id, std::make_shared<Model>(std::move(model)));
   return *this;
}

Scene&
Scene::Add(ModelGroup&& model, const std::string& name)
{
   const std::string& id = name.empty() ? "Model_" + ToString(Actors_.size()) : name;
   Actors_.emplace(id, std::make_shared<ModelGroup>(std::move(model)));
   return *this;
}

Scene&
Scene::Add(TeXGlyph&& tex_glyph, const std::string& name)
{
   return *this;
}

Scene&
Scene::Add(TeXBox&& tex_box, const std::string& name)
{
   const std::string& id = name.empty() ? "TeXBox_" + ToString(TeXBoxes_.size()) : name;
   auto ptex_box = std::make_shared<TeXBox>(std::move(tex_box));
   Actors_.emplace(id, ptex_box);
   TeXBoxes_.emplace(id, ptex_box);
   return *this;
}

Scene&
Scene::Add(DirectionalLight&& light, const std::string& name)
{
   const std::string& id = name.empty() ? "D-light_" + ToString(DLights_.size()) : name;
   DLights_.emplace(id, std::move(light));
   return *this;
}

Scene&
Scene::Add(PointLight&& light, const std::string& name)
{
   const std::string& id = name.empty() ? "P-light_" + ToString(PLights_.size()) : name;
   PLights_.emplace(id, std::move(light));
   return *this;
}

Scene&
Scene::Add(SpotLight&& light, const std::string& name)
{
   const std::string& id = name.empty() ? "S-light_" + ToString(SLights_.size()) : name;
   SLights_.emplace(id, std::move(light));
   return *this;
}

void
Scene::Init(const Real start_time)
{
   // Initialise all models and lights.
   FOR_EACH(_, model , Actors_)  model->Init();
   FOR_EACH(_, dlight, DLights_) dlight.Init();
   FOR_EACH(_, plight, PLights_) plight.Init();
   FOR_EACH(_, slight, SLights_) slight.Init();

   // Compute the start and end times of the scene.
   const auto max_duration(1.0e5);
   if(AdjustDuration_)
   {
      Real duration = -One;
      FOR_EACH_CONST(_, model, Actors_) if(model->ExitTime() < max_duration) Maximise(duration, model->ExitTime());
      Duration_ = isPositive(duration) ? duration : Duration_;
      ASSERT(isPositive(Duration_), "Could not adjust the scene duration based on model lifetimes. Please specify the duration for scene: ", Title_)
   }
   else
   {
      FOR_EACH_CONST(_, model, Actors_)
         if(model->ExitTime() < max_duration)
            ASSERT(model->ExitTime() < Duration_, "This model's lifespan exceeds that of scene: ", Title_)
   }

   StartTime_ = start_time;
   EndTime_   = StartTime_ + Duration_;
}

/***************************************************************************************************************************************************************
* Private Interface
***************************************************************************************************************************************************************/
void
Scene::UpdateModels(const Real current_time) { FOR_EACH(_, model, Actors_) model->Update(current_time); }

void
Scene::RenderDirecShadows(Shader& shader)
{
   if(DLights_.empty()) return;
   else ASSERT(DLights_.size() == 1, "Can currently only handle one directional light.")

   shader.Bind();
   shader.SetDirectionalLightSpaceMatrix(DLights_["Sun"].LightSpaceMatrix());

   auto& shadow_map = DLights_["Sun"].ShadowMap();
   GLCall(glViewport(0, 0, shadow_map.DepthMap().Width(), shadow_map.DepthMap().Height()))

//  GLCall(glCullFace(GL_FRONT)); // Prevents peter-panning

   shadow_map.StartWrite();
   RenderModels(shader);
   shadow_map.StopWrite();

//   GLCall(glCullFace(GL_BACK));

   shader.Unbind();
}

void
Scene::RenderPointShadows(Shader& shader)
{
   if(PLights_.empty()) return;
   else ASSERT(PLights_.size() < 5, "Can currently handle at most four point lights.")

   shader.Bind();

   FOR_EACH(_, point_light, PLights_)
   {
      shader.SetPointLightSpaceMatrices(point_light.LightSpaceMatrices());
      shader.SetPointPosition(point_light.Position());
      shader.SetPointFarPlane(PointLight::FarPlane());

      auto& shadow_map = point_light.ShadowMap();
      GLCall(glViewport(0, 0, shadow_map.DepthMap().Width(), shadow_map.DepthMap().Height()));

      shadow_map.StartWrite();
      RenderModels(shader);
      shadow_map.StopWrite();
   }

   shader.Unbind();
}

void
Scene::RenderScene(Shader& shader, Camera& camera)
{
   ASSERT(DLights_.size() <= 1, "Can currently only handle at most one directional light.")

   shader.Bind();
   shader.UseCamera(camera);

   if(!DLights_.empty())
   {
      shader.UseLight(DLights_["Sun"]);
      shader.SetDirectionalLightSpaceMatrix(DLights_["Sun"].LightSpaceMatrix());
      DLights_["Sun"].ShadowMap().StartRead(1);
      shader.SetDirectionalShadowMap(1);
   }

   size_t i = 0;
   FOR_EACH(_, point_light, PLights_)
   {
      shader.UseLight(point_light);
      point_light.ShadowMap().StartRead(i + 2);
      shader.SetPointShadowMap(i, i + 2);
      i++;
   }
   shader.SetPointFarPlane(PointLight::FarPlane());

   RenderModels(shader);

   shader.Unbind();
}

void
Scene::RenderModels(Shader& shader)
{
   // Line segment
//    shader_storage_buffer.BindBase();
//    GLsizei N2 = (GLsizei)varray.size() - 2;
//    glDrawArrays(GL_TRIANGLES, 0, 6*(N2 - 1));

   shader.SetUniform1i("u_use_diffuse_map"     , 0);
   shader.SetUniform1i("u_use_normal_map"      , 0);
   shader.SetUniform1i("u_use_displacement_map", 0);

   // Render model and its sub-models.
   FOR_EACH(_, actor, Actors_) RenderModel(actor, shader);

   // Unbind all textures
   FOR_EACH(_, sub_textures, Textures_) FOR_EACH(_, texture, sub_textures) texture.Unbind();
}

void
Scene::RenderModel(SPtr<ModelGroup>& model, Shader& shader)
{
   constexpr int slot_offset(3); // TODO - currently hard-coded.

   if(model->isInitialised_)
   {
      if(model->_Material.has_value()) shader.UseMaterial(model->_Material.value());
      if(model->_TextureInfo.has_value())
      {
         size_t texture_index = 0;
         FOR_EACH(type_string, texture, Textures_[model->_TextureInfo.value().first])
         {
            // Configure respective texture uniform.
            const auto& uniform_name = TextureUniformString(type_string);
            shader.UseTexture(texture, "u_" + uniform_name, slot_offset + texture_index++);
            shader.SetUniform1i("u_use_" + uniform_name, 1);

            // Set scale if this is a displacement map.
            if(GetTextureType(type_string) == TextureType::Displacement)
            {
               const auto& scale = texture.MapScale();
               ASSERT(scale.has_value(), "The displacement map scale has not been set.")
               shader.SetUniform1f("u_" + uniform_name + "_scale", scale.value());
            }
         }
      }

      shader.UseModel(*model);
      model->Render();

      // Switch off texture maps
      if(model->_TextureInfo.has_value())
         FOR_EACH(type_string, _, Textures_[model->_TextureInfo.value().first])
            shader.SetUniform1i("u_use_" + TextureUniformString(type_string), 0);
   }

   // Render sub-models recursively.
   FOR_EACH(_, sub_model, model->SubModels_) RenderModel(sub_model, shader);
}

}