#include "../include/Scene.h"

namespace aprn::vis {

/***************************************************************************************************************************************************************
* Public Interface
***************************************************************************************************************************************************************/
Scene::Scene()
   : Scene(-One, true) {}

Scene::Scene(Float duration, bool adjust_duration)
   : _Duration(duration), _AdjustDuration(adjust_duration)
{
   ASSERT(isPositive(duration) || _AdjustDuration, "Cannot have a negative duration for a scene unless the final duration is to be computed.")
   ASSERT(_isSingleScene, "This constructor should only be called for the first scene.")

   _isSingleScene = false;
}

Scene::Scene(Scene& prev_scene, Float duration, bool adjust_duration)
   : _Duration(duration), _AdjustDuration(adjust_duration)
{
   ASSERT(Zero < duration, "Cannot have a negative duration for a scene.")
   ASSERT(!_isSingleScene, "This constructor should not be called for the first scene.")
   ASSERT(_PrevScene, "The current scene has already been assigned a previous scene.")
   ASSERT(prev_scene._NextScene, "The previous scene has already been assigned a next scene.")

   _PrevScene = &prev_scene;
   prev_scene._NextScene = this;
}

Scene&
Scene::Add(Model& model, const std::string& name)
{
   const std::string& id = name.empty() ? "Model_" + ToString(_Models.size()) : name;
   auto pmodel = std::make_shared<Model>(std::move(model));
   pmodel->Init();
   _Models[id] = pmodel;
   return *this;
}

Scene&
Scene::Add(TeXBox& tex_box, const std::string& name)
{
   const std::string& id = name.empty() ? "TeXBox_" + ToString(_TeXBoxes.size()) : name;
   auto ptex_box = std::make_shared<TeXBox>(std::move(tex_box));
   ptex_box->Init();
   _Models[id] = ptex_box;
   return *this;
}

Scene&
Scene::Add(DirectionalLight&& light, const std::string& name)
{
   const std::string& id = name.empty() ? "D-light_" + ToString(_DLights.size()) : name;
   _DLights.emplace(id, std::move(light));
   return *this;
}

Scene&
Scene::Add(PointLight&& light, const std::string& name)
{
   const std::string& id = name.empty() ? "P-light_" + ToString(_PLights.size()) : name;
   _PLights.emplace(id, std::move(light));
   return *this;
}

Scene&
Scene::Add(SpotLight&& light, const std::string& name)
{
   const std::string& id = name.empty() ? "S-light_" + ToString(_SLights.size()) : name;
   _SLights.emplace(id, std::move(light));
   return *this;
}

void
Scene::Init(const Float start_time)
{
   const auto max_duration(1.0e5);

   if(_AdjustDuration)
   {
      Float duration = -One;
      FOR_EACH_CONST(_, model, _Models) if(model->_ExitTime < max_duration) Maximise(duration, model->_ExitTime);
      _Duration = isPositive(duration) ? duration : _Duration;
      ASSERT(isPositive(_Duration), "Could not adjust the scene duration based on model lifetimes. Please specify the duration for scene: ", _Title)
   }
   else
   {
      FOR_EACH_CONST(_, model, _Models)
         if(model->_ExitTime < max_duration) ASSERT(model->_ExitTime < _Duration, "This model's lifespan exceeds that of scene: ", _Title)
   }

   _StartTime = start_time;
   _EndTime   = _StartTime + _Duration;
}

/***************************************************************************************************************************************************************
* Private Interface
***************************************************************************************************************************************************************/
bool Scene::_isSingleScene = true;

void
Scene::UpdateModels(const Float current_time) { FOR_EACH(_, model, _Models) model->Update(current_time); }

void
Scene::RenderScene(Shader& shader, Camera& camera)
{
   ASSERT(_DLights.size() <= 1, "Can currently only handle at most one directional light.")

   // Clear window
   GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
   GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

   shader.Bind();

   shader.UseCamera(camera);

   if(!_DLights.empty())
   {
      shader.UseLight(_DLights["Sun"]);
      shader.SetDirectionalLightSpaceMatrix(_DLights["Sun"].GetLightSpaceMatrix());
      _DLights["Sun"].GetShadowMap().ReadFrom(1);
      shader.SetDirectionalShadowMap(1);
   }

   size_t i = 0;
   FOR_EACH(_, point_light, _PLights)
   {
      shader.UseLight(point_light);
      point_light.GetShadowMap().ReadFrom(i + 2);
      shader.SetPointShadowMap(i, i + 2);
      i++;
   }
   shader.SetPointFarPlane(PointLight::GetFarPlane());

   RenderModels(shader);

   if(!_DLights.empty()) _DLights["Sun"].GetShadowMap().Finalise();

   shader.Unbind();
}

void
Scene::RenderDirectionalShadows(Shader& shader)
{
   if(_DLights.empty()) return;
   else ASSERT(_DLights.size() == 1, "Can currently only handle one directional light.")

   shader.Bind();
   shader.SetDirectionalLightSpaceMatrix(_DLights["Sun"].GetLightSpaceMatrix());

   auto& shadow_map = _DLights["Sun"].GetShadowMap();
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
   if(_PLights.empty()) return;

   shader.Bind();

   FOR_EACH(_, point_light, _PLights)
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

   FOR_EACH(_, model, _Models)
   {
      if(model->_Material.has_value()) shader.UseMaterial(model->_Material.value());
      if(model->_Texture.has_value())
      {
         size_t texture_index = 0;
         FOR_EACH(type_string, texture, _Textures[model->_Texture.value()])
         {
            const auto& uniform_name = GetTextureUniformString(type_string);
            shader.UseTexture(texture, "u_" + uniform_name, slot_offset + texture_index++);
            shader.SetUniform1i("u_use_" + uniform_name, 1);
            if(GetTextureType(type_string) == TextureType::Displacement)
            {
               const auto& scale = texture.GetMapScale();
               ASSERT(scale.has_value(), "The displacement map scale has not been set.")
               shader.SetUniform1f("u_" + uniform_name + "_scale", scale.value());
            }
         }
      }

      shader.UseModel(*model);
      model->Render();

      // Switch off texture maps
      if(model->_Texture.has_value())
         FOR_EACH(type_string, _, _Textures[model->_Texture.value()])
            shader.SetUniform1i("u_use_" + GetTextureUniformString(type_string), 0);
   }

   // Unbind all textures
   FOR_EACH(_, sub_textures, _Textures) FOR_EACH(_, texture, sub_textures) texture.Unbind();
}

}