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
Scene::Add(Model& model, const std::string& name) { return Add(std::move(model), name); }

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
   const std::string& id = name.empty() ? "Model_" + ToStr(_Models.size()) : name;
   auto pmodel = std::make_shared<Model>(std::move(model));

   pmodel->Init();
   _Models[id] = pmodel;

   return *this;
}

Scene&
Scene::Add(TeXBox&& tex_box, const std::string& name)
{
   const std::string& id = name.empty() ? "TeXBox_" + ToStr(_TeXBoxes.size()) : name;
   auto ptex_box = std::make_shared<TeXBox>(std::move(tex_box));

   // Note: must not intialise the tex-box (or its underlying model) here. This is done for all tex-boxes collectively in InitTeXBoxes().
   _Models[id]   = ptex_box;
   _TeXBoxes[id] = ptex_box;

   return *this;
}

Scene&
Scene::Add(DirectionalLight&& light, const std::string& name)
{
   const std::string& id = name.empty() ? "D-light_" + ToStr(_DLights.size()) : name;
   _DLights.emplace(id, std::move(light));
   return *this;
}

Scene&
Scene::Add(PointLight&& light, const std::string& name)
{
   const std::string& id = name.empty() ? "P-light_" + ToStr(_PLights.size()) : name;
   _PLights.emplace(id, std::move(light));
   return *this;
}

Scene&
Scene::Add(SpotLight&& light, const std::string& name)
{
   const std::string& id = name.empty() ? "S-light_" + ToStr(_SLights.size()) : name;
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
void
Scene::UpdateModels(const Float current_time) { FOR_EACH(_, model, _Models) model->Update(current_time); }

void
Scene::RenderScene(Shader& shader, Camera& camera)
{
   ASSERT(_DLights.size() <= 1, "Can currently only handle at most one directional light.")

   shader.Bind();
   shader.UseCamera(camera);

   if(!_DLights.empty())
   {
      shader.UseLight(_DLights["Sun"]);
      shader.SetDirectionalLightSpaceMatrix(_DLights["Sun"].LightSpaceMatrix());
      _DLights["Sun"].ShadowMap().StartRead(1);
      shader.SetDirectionalShadowMap(1);
   }

   size_t i = 0;
   FOR_EACH(_, point_light, _PLights)
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
Scene::RenderDirecShadows(Shader& shader)
{
   if(_DLights.empty()) return;
   else ASSERT(_DLights.size() == 1, "Can currently only handle one directional light.")

   shader.Bind();
   shader.SetDirectionalLightSpaceMatrix(_DLights["Sun"].LightSpaceMatrix());

   auto& shadow_map = _DLights["Sun"].ShadowMap();
   GLCall(glViewport(0, 0, shadow_map.DepthMap().Width(), shadow_map.DepthMap().Height()));

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
   if(_PLights.empty()) return;

   shader.Bind();

   FOR_EACH(_, point_light, _PLights)
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
      if(model->_Texture.has_value())
         FOR_EACH(type_string, _, _Textures[model->_Texture.value()])
            shader.SetUniform1i("u_use_" + TextureUniformString(type_string), 0);
   }

   // Unbind all textures
   FOR_EACH(_, sub_textures, _Textures) FOR_EACH(_, texture, sub_textures) texture.Unbind();
}

}