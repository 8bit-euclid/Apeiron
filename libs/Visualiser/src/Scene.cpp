#include "../include/Scene.h"

namespace aprn::vis {

/***************************************************************************************************************************************************************
* Public Interface
***************************************************************************************************************************************************************/
Scene::Scene()
   : Scene(-One, true) {}

Scene::Scene(Float duration, bool adjust_duration)
   : Duration(duration), AdjustDuration(adjust_duration)
{
   ASSERT(isPositive(duration) || AdjustDuration, "Cannot have a negative duration for a scene unless the final duration is to be computed.")
   ASSERT(isSingleScene, "This constructor should only be called for the first scene.")

   isSingleScene = false;
}

Scene::Scene(Scene& prev_scene, Float duration, bool adjust_duration)
   : Duration(duration), AdjustDuration(adjust_duration)
{
   ASSERT(Zero < duration, "Cannot have a negative duration for a scene.")
   ASSERT(!isSingleScene, "This constructor should not be called for the first scene.")
   ASSERT(PrevScene, "The current scene has already been assigned a previous scene.")
   ASSERT(prev_scene.NextScene, "The previous scene has already been assigned a next scene.")

   PrevScene.reset(&prev_scene);
   prev_scene.NextScene.reset(this);
}

void
Scene::Add(Model& model, const std::string& name)
{
   const std::string& id = name.empty() ? "Model_" + ToString(Models.size()) : name;
   auto pmodel = std::make_shared<Model>(std::move(model));
   pmodel->Init();
   Add(pmodel, id);
}

void
Scene::Add(DirectionalLight&& light, const std::string& name)
{
   const std::string& id = name.empty() ? "D-light_" + ToString(DirectionalLights.size()) : name;
   DirectionalLights.emplace(id, std::move(light));
}

void
Scene::Add(PointLight&& light, const std::string& name)
{
   const std::string& id = name.empty() ? "P-light_" + ToString(PointLights.size()) : name;
   PointLights.emplace(id, std::move(light));
}

void
Scene::Add(SpotLight&& light, const std::string& name)
{
   const std::string& id = name.empty() ? "S-light_" + ToString(SpotLights.size()) : name;
   SpotLights.emplace(id, std::move(light));
}

void
Scene::Init(const Float start_time)
{
   const auto max_duration(1.0e5);

   if(AdjustDuration)
   {
      Float duration = -One;
      FOR_EACH_CONST(_, model, Models) if(model->ExitTime < max_duration) Maximise(duration, model->ExitTime);
      Duration = isPositive(duration) ? duration : Duration;

      ASSERT(isPositive(Duration), "Could not adjust the scene duration based on model lifetimes. Please specify the duration for scene: ", Name)
   }
   else
   {
      FOR_EACH_CONST(_, model, Models)
         if(model->ExitTime < max_duration) ASSERT(model->ExitTime < Duration, "This model's lifespan exceeds that of scene: ", Name)
   }

   StartTime = start_time;
   EndTime   = StartTime + Duration;
}

/***************************************************************************************************************************************************************
* Private Interface
***************************************************************************************************************************************************************/
bool Scene::isSingleScene = true;

void
Scene::Add(std::shared_ptr<Model> model, const std::string& name) { Models[name] = model; }

void
Scene::UpdateModels(const Float current_time) { FOR_EACH(_, model, Models) model->Update(current_time); }

void
Scene::RenderScene(Shader& shader, Camera& camera)
{
   ASSERT(DirectionalLights.size() <= 1, "Can currently only handle at most one directional light.")

   // Clear window
   GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
   GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

   shader.Bind();

   shader.UseCamera(camera);

   if(!DirectionalLights.empty())
   {
      shader.UseLight(DirectionalLights["Sun"]);
      shader.SetDirectionalLightSpaceMatrix(DirectionalLights["Sun"].GetLightSpaceMatrix());
      DirectionalLights["Sun"].GetShadowMap().ReadFrom(1);
      shader.SetDirectionalShadowMap(1);
   }

   size_t i = 0;
   FOR_EACH(_, point_light, PointLights)
   {
      shader.UseLight(point_light);
      point_light.GetShadowMap().ReadFrom(i + 2);
      shader.SetPointShadowMap(i, i + 2);
      i++;
   }
   shader.SetPointFarPlane(PointLight::GetFarPlane());

   RenderModels(shader);

   if(!DirectionalLights.empty()) DirectionalLights["Sun"].GetShadowMap().Finalise();

   shader.Unbind();
}

void
Scene::RenderDirectionalShadows(Shader& shader)
{
   if(DirectionalLights.empty()) return;
   else ASSERT(DirectionalLights.size() == 1, "Can currently only handle one directional light.")

   shader.Bind();
   shader.SetDirectionalLightSpaceMatrix(DirectionalLights["Sun"].GetLightSpaceMatrix());

   auto& shadow_map = DirectionalLights["Sun"].GetShadowMap();
   GLCall(glViewport(0, 0, shadow_map.GetDepthMap().GetWidth(), shadow_map.GetDepthMap().GetHeight()));

//  GLCall(glCullFace(GL_FRONT)); // Prevents peter-panning

   shadow_map.WriteTo();
   RenderModels(shader);
   shadow_map.Finalise();

//   GLCall(glCullFace(GL_BACK));

   shader.Unbind();
}

void
Scene::RenderPointShadows(Shader& shader)
{
   if(PointLights.empty()) return;

   shader.Bind();

   FOR_EACH(_, point_light, PointLights)
   {
      shader.SetPointLightSpaceMatrices(point_light.GetLightSpaceMatrices());
      shader.SetPointPosition(point_light.GetPosition());
      shader.SetPointFarPlane(PointLight::GetFarPlane());

      auto& shadow_map = point_light.GetShadowMap();
      GLCall(glViewport(0, 0, shadow_map.GetDepthMap().GetWidth(), shadow_map.GetDepthMap().GetHeight()));

      shadow_map.WriteTo();
      RenderModels(shader);
      shadow_map.Finalise();
   }

   shader.Unbind();
}

void
Scene::RenderModels(Shader& shader)
{
   // Line segment
//    shader_storage_buffer.BindBase();
//    GLsizei N2 = (GLsizei)varray.size() - 2;
//    glDrawArrays(GL_TRIANGLES, 0, 6*(N2 - 1));

   constexpr int slot_offset(3);
   shader.SetUniform1i("u_use_diffuse_map", 0);
   shader.SetUniform1i("u_use_normal_map", 0);
   shader.SetUniform1i("u_use_displacement_map", 0);

   FOR_EACH(_, model, Models)
   {
      if(model->MaterialSpec.has_value()) shader.UseMaterial(model->MaterialSpec.value());
      if(model->TextureSpec.has_value())
      {
         size_t texture_index = 0;
         FOR_EACH(type_string, texture, Textures[model->TextureSpec.value()])
         {
            const auto& uniform_name = GetTextureUniformString(type_string);
            shader.UseTexture(*texture, "u_" + uniform_name, slot_offset + texture_index++);
            shader.SetUniform1i("u_use_" + uniform_name, 1);
            if(GetTextureType(type_string) == TextureType::Displacement)
            {
               const auto& scale = texture->GetMapScale();
               ASSERT(scale.has_value(), "The displacement map scale has not been set.")
               shader.SetUniform1f("u_" + uniform_name + "_scale", scale.value());
            }
         }
      }

      shader.UseModel(*model);
      model->Render();

      // Switch off texture maps
      if(model->TextureSpec.has_value())
         FOR_EACH(type_string, _, Textures[model->TextureSpec.value()])
            shader.SetUniform1i("u_use_" + GetTextureUniformString(type_string), 0);
   }

   // Unbind all textures
   FOR_EACH(_, sub_textures, Textures) FOR_EACH(_, texture, sub_textures) texture->Unbind();
}

}