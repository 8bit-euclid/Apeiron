
#include "../include/Visualiser.h"

#include <optional>
#include <string>

namespace aprn::vis {

/***************************************************************************************************************************************************************
* Public Interface
***************************************************************************************************************************************************************/
Visualiser::Visualiser()
   : Visualiser(1920, 1080) {}

Visualiser::Visualiser(GLint window_width, GLint window_height)
   : OpenGLWindow(window_width, window_height), Cameras{{"Main", Camera()}}, ActiveCamera(Cameras["Main"])
{
//   Add(DirectionalLight(glm::vec3(0.0, -1.0, -1.0), glm::vec4(1.0, 1.0, 1.0, 1.0), 0.3, 0.6), "Sun");
}

void
Visualiser::Add(Model& model, const std::string& name)
{
   const std::string& id = name.empty() ? "model_" + ToString(Models.size()) : name;
   auto mod = std::make_shared<Model>(std::move(model));
   mod->Init();
   Add(mod, id);
}

void
Visualiser::Add(Camera&& camera, const std::string& name)
{
   const std::string& id = name.empty() ? "camera_" + ToString(Cameras.size()) : name;
   Cameras.emplace(id, std::move(camera));
}

void
Visualiser::Add(DirectionalLight&& light, const std::string& name)
{
   const std::string& id = name.empty() ? "d-light_" + ToString(DirectionalLights.size()) : name;
   DirectionalLights.emplace(id, std::move(light));
}

void
Visualiser::Add(PointLight&& light, const std::string& name)
{
   const std::string& id = name.empty() ? "p-light_" + ToString(PointLights.size()) : name;
   PointLights.emplace(id, std::move(light));
}

void
Visualiser::Add(SpotLight&& light, const std::string& name)
{
   const std::string& id = name.empty() ? "s-light_" + ToString(SpotLights.size()) : name;
   SpotLights.emplace(id, std::move(light));
}

void
Visualiser::Render()
{
   Init();
   OpenGLWindow.ResetTime();

   while(OpenGLWindow.isOpen())
   {
      BeginFrame();
      ManageUserInputs();
      UpdateViewFrustum();
      RenderDirectionalShadows();
      RenderPointShadows();
      RenderFullScene();
      EndFrame();
   }
}

/***************************************************************************************************************************************************************
* Private Interface
***************************************************************************************************************************************************************/
void
Visualiser::Init()
{
   // Set default settings of the main camera
   auto& camera = ActiveCamera.get();
   camera.SetOrientation(glm::vec3(0.0f, 0.0f, 1.0f), 0.0, 90.0);
   camera.SetViewFrustum(OpenGLWindow.ComputeViewportAspectRatio(), 45.0, 1.0, -100.0);

   // Load essential shaders
   Shaders.emplace("General",           "libs/Visualiser/resources/shaders/General.glsl");
   Shaders.emplace("Line",              "libs/Visualiser/resources/shaders/Line.glsl");
   Shaders.emplace("DirectionalShadow", "libs/Visualiser/resources/shaders/DirectionalShadow.glsl");
   Shaders.emplace("PointShadow",       "libs/Visualiser/resources/shaders/PointShadow.glsl");

   // Load textures from models
   AddTextures();
}

void
Visualiser::Add(std::shared_ptr<Model> model, const std::string& name) { Models[name] = model; }

void
Visualiser::AddTextures()
{
   for(const auto& [_, model] : Models)
      if(model->TextureSpec.has_value() && !Textures.contains(model->TextureSpec.value()))
      {
         // Add all files associated to the given texture
         const auto& texture_name = model->TextureSpec.value();
         const auto texture_list  = {TextureType::Diffuse,
                                     TextureType::Normal,
                                     TextureType::Displacement}; // Add appropriate enums if more textures are to be read
         Map<Texture> texture_files;
         for(auto texture_type : texture_list)
         {
            auto path = GetTextureFilePath(GetTextureFileDirectory(texture_name), texture_type);
            if(path.has_value())
            {
               texture_files.emplace(GetTextureTypeString(texture_type), std::move(Texture{texture_type, path.value()}));
               if(texture_type == TextureType::Displacement)
               {
//                  Print<'\0'>("Please enter the height map scale for texture ", texture_name, ": ");
                  Float displacement_map_scale;
//                  std::cin >> displacement_map_scale;
                  displacement_map_scale = 0.08;
                  texture_files.at(GetTextureTypeString(texture_type)).SetMapScale(displacement_map_scale);
               }
            }
            else EXIT("Could not locate the texture files of texture ", texture_name)
         }

         // Add texture files to the list of textures
         Textures.emplace(texture_name, std::move(texture_files));
      }
}

void
Visualiser::BeginFrame()
{
   // Clear window
   GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
   GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

   // Update the current and previous times, compute delta time, and check if the viewport was modified.
   OpenGLWindow.ComputeDeltaTime();
   isViewPortModified = OpenGLWindow.isViewPortModified();

   // Update model actions at the current time-stamp
   UpdateModels();
}

void
Visualiser::EndFrame()
{
   OpenGLWindow.SwapBuffers();
   glfwPollEvents();
}

void
Visualiser::UpdateModels() { for(auto& [_, model] : Models) model->Update(OpenGLWindow.GetCurrentTime()); }

void
Visualiser::ManageUserInputs()
{
   auto& camera = ActiveCamera.get();
   camera.KeyControl(OpenGLWindow.Keys, OpenGLWindow.GetDeltaTime());
   camera.MousePositionControl(OpenGLWindow.GetMouseDisplacement());
   camera.MouseWheelControl(OpenGLWindow.GetMouseWheelDisplacement());
}

void
Visualiser::UpdateViewFrustum()
{
   // If the viewport has been modified, update the view frustum
   if(isViewPortModified)
   {
      ActiveCamera.get().SetViewFrustum(OpenGLWindow.ComputeViewportAspectRatio());
      Shaders["Line"].SetUniform2f("u_resolution", OpenGLWindow.ViewportDimensions[0], OpenGLWindow.ViewportDimensions[1]);
   }
}

void
Visualiser::RenderModels(const std::string& _shader_name)
{
   // Line segment
//    shader_storage_buffer.BindBase();
//    GLsizei N2 = (GLsizei)varray.size() - 2;
//    glDrawArrays(GL_TRIANGLES, 0, 6*(N2 - 1));

   auto& shader = Shaders[_shader_name];

   constexpr int slot_offset(3);
   shader.SetUniform1i("u_use_diffuse_map", 0);
   shader.SetUniform1i("u_use_normal_map", 0);
   shader.SetUniform1i("u_use_displacement_map", 0);

   for(auto& [_, model] : Models)
   {
      if(model->MaterialSpec.has_value()) shader.UseMaterial(model->MaterialSpec.value());
      if(model->TextureSpec.has_value())
      {
         size_t texture_index = 0;
         for(auto& [type_string, texture] : Textures[model->TextureSpec.value()])
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
      if(model->TextureSpec.has_value())
         for(auto& [type_string, _] : Textures[model->TextureSpec.value()])
            shader.SetUniform1i("u_use_" + GetTextureUniformString(type_string), 0);
   }

   // Unbind all textures
   for(auto& [_, sub_textures] : Textures) for(auto& [_, texture] : sub_textures) texture.Unbind();
}

void
Visualiser::RenderDirectionalShadows()
{
   if(DirectionalLights.empty()) return;
   else ASSERT(DirectionalLights.size() == 1, "Can currently only handle one directional light.")

   auto& shader_name = "DirectionalShadow";
   auto& shader      = Shaders[shader_name];
   shader.Bind();
   shader.SetDirectionalLightSpaceMatrix(DirectionalLights["Sun"].GetLightSpaceMatrix());

   Shadow& shadow_map = DirectionalLights["Sun"].GetShadowMap();

   GLCall(glViewport(0, 0, shadow_map.GetDepthMap().GetWidth(), shadow_map.GetDepthMap().GetHeight()));

//  GLCall(glCullFace(GL_FRONT)); // Prevents peter-panning

   shadow_map.WriteTo();

   RenderModels(shader_name);

   shadow_map.Finalise();

//   GLCall(glCullFace(GL_BACK));

   shader.Unbind();
}

void
Visualiser::RenderPointShadows()
{
   if(PointLights.empty()) return;

   auto& shader_name = "PointShadow";
   auto& shader      = Shaders[shader_name];
   shader.Bind();

   for(auto& [_, point_light] : PointLights)
   {
      shader.SetPointLightSpaceMatrices(point_light.GetLightSpaceMatrices());
      shader.SetPointPosition(point_light.GetPosition());
      shader.SetPointFarPlane(PointLight::GetFarPlane());

      const Shadow& shadow_map = point_light.GetShadowMap();

      GLCall(glViewport(0, 0, shadow_map.GetDepthMap().GetWidth(), shadow_map.GetDepthMap().GetHeight()));

      shadow_map.WriteTo();

      RenderModels(shader_name);

      shadow_map.Finalise();
   }

   shader.Unbind();
}

void
Visualiser::RenderFullScene()
{
   ASSERT(DirectionalLights.size() <= 1, "Can currently only handle at most one directional light.")

   OpenGLWindow.ResetViewPort();

   // Clear window
   GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
   GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

   auto& shader_name = "General";
   auto& shader      = Shaders[shader_name];
   shader.Bind();

   shader.UseCamera(ActiveCamera.get());

   if(!DirectionalLights.empty())
   {
      shader.UseLight(DirectionalLights["Sun"]);
      shader.SetDirectionalLightSpaceMatrix(DirectionalLights["Sun"].GetLightSpaceMatrix());
      DirectionalLights["Sun"].GetShadowMap().ReadFrom(1);
      shader.SetDirectionalShadowMap(1);
   }

   size_t i = 0;
   for(auto& [_, point_light] : PointLights)
   {
      shader.UseLight(point_light);
      point_light.GetShadowMap().ReadFrom(i + 2);
      shader.SetPointShadowMap(i, i + 2);
      i++;
   }
   shader.SetPointFarPlane(PointLight::GetFarPlane());

   RenderModels(shader_name);

   if(!DirectionalLights.empty()) DirectionalLights["Sun"].GetShadowMap().Finalise();

   shader.Unbind();
}

}

