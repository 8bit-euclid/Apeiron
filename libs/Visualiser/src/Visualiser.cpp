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
   : OpenGLWindow(window_width, window_height), Cameras{{"Main", Camera()}}, ActiveCamera(&Cameras["Main"]) {}

void
Visualiser::Add(Scene& scene, const std::string& name)
{
   const std::string& id = name.empty() ? "Scene_" + ToString(Scenes.size()) : name;
   Scenes.emplace(id, std::move(scene));
}

void
Visualiser::Add(Camera&& camera, const std::string& name)
{
   const std::string& id = name.empty() ? "Camera_" + ToString(Cameras.size()) : name;
   Cameras.emplace(id, std::move(camera));
}

void
Visualiser::Render()
{
   Init();

   while(OpenGLWindow.isOpen())
   {
      StartFrame();
      UpdateScene();
      ManageUserInputs();
      UpdateViewFrustum();
      RenderScene();
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
   ASSERT(ActiveCamera, "The active camera pointer has not yet been set.")
   ActiveCamera->SetOrientation(glm::vec3(0.0f, 0.0f, 1.0f), 0.0, 90.0);
   ActiveCamera->SetViewFrustum(OpenGLWindow.ComputeViewportAspectRatio(), 45.0, 1.0, -100.0);

   // Load all shaders
   Shaders.emplace("General",           "libs/Visualiser/resources/shaders/General.glsl");
   Shaders.emplace("Line",              "libs/Visualiser/resources/shaders/Line.glsl");
   Shaders.emplace("DirectionalShadow", "libs/Visualiser/resources/shaders/DirectionalShadow.glsl");
   Shaders.emplace("PointShadow",       "libs/Visualiser/resources/shaders/PointShadow.glsl");

   // Load requires textures and initialise scenes
   LoadTextures();
   InitScenes();

   // Set clock time to zero
   OpenGLWindow.ResetTime();
}

void
Visualiser::LoadTextures()
{
   FOR_EACH(_, scene, Scenes)
      FOR_EACH_CONST(_, model, scene.Models)
         if(model->TextureSpec.has_value() && !Textures.contains(model->TextureSpec.value()))
         {
            // Add all files associated to the given texture
            const auto& texture_name = model->TextureSpec.value();
            const auto  texture_list = {TextureType::Diffuse,
                                        TextureType::Normal,
                                        TextureType::Displacement}; // Add appropriate enums if more textures are to be read
            Map<Texture> texture_files;
            FOR_EACH_CONST(texture_type, texture_list)
            {
               auto path = GetTextureFilePath(GetTextureFileDirectory(texture_name), texture_type);
               if(path.has_value())
               {
                  texture_files.emplace(GetTextureTypeString(texture_type), std::move(Texture{texture_type, path.value()}));
                  if(texture_type == TextureType::Displacement)
                  {
//                     Print<'\0'>("Please enter the height map scale for texture ", texture_name, ": ");
                     Float displacement_map_scale;
//                     std::cin >> displacement_map_scale;
                     displacement_map_scale = 0.08;
                     texture_files.at(GetTextureTypeString(texture_type)).SetMapScale(displacement_map_scale);
                  }
               }
               else EXIT("Could not locate the texture files of texture ", texture_name)
            }

            // Add texture files to the list of textures
            Textures.emplace(texture_name, std::move(texture_files));

            // Point to the textures from the scene
            Map<SPtr<Texture>> ptexture_files;
            FOR_EACH(sub_texture_name, sub_texture, Textures[texture_name]) ptexture_files.emplace(sub_texture_name, SPtr<Texture>(&sub_texture));
            scene.Textures.emplace(texture_name, ptexture_files);
         }
}

void
Visualiser::InitScenes()
{
   auto first_scene_it = std::find_if(Scenes.begin(), Scenes.end(), [](const auto& entry){return entry.second.PrevScene == nullptr;});
   ASSERT(first_scene_it != Scenes.end(), "Could not locate the first scene.")

   CurrentScene = &first_scene_it->second;
   Scene* current_scene(CurrentScene);
   size_t scene_count{};
   Float  start_time{};
   do
   {
      // Update current scene if it isn't the first iteration
      if(scene_count)
      {
         // Sync the start-time of the next scene (or the transition to it) to the end-time of the current scene.
         start_time = current_scene->EndTime;

         // Initialise the scene transition if there is one, and re-update the start time of the next scene.
         if(current_scene->TransitionToNext.has_value())
         {
            auto& transition = current_scene->TransitionToNext.value();
            transition.Init(start_time);
            start_time = transition.EndTime;
         }

         // Move to next scene
         current_scene = current_scene->NextScene.get();
      }

      // Initialise current scene and update count
      current_scene->Init(start_time);
      ++scene_count;
   }
   while(current_scene->NextScene);

   ASSERT(scene_count == Scenes.size(), "There was a mismatch in the total number of scenes.")
}

void
Visualiser::StartFrame()
{
   // Clear window
   GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
   GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

   // Update the current and previous times, compute delta time, and check if the viewport was modified.
   OpenGLWindow.ComputeDeltaTime();
   wasViewPortModified = OpenGLWindow.isViewPortModified();
}

void
Visualiser::EndFrame()
{
   OpenGLWindow.SwapBuffers();
   glfwPollEvents();
}

void
Visualiser::UpdateScene()
{
   DEBUG_ASSERT(CurrentScene, "The current scene pointer has not yet been set.")

   // Determine if the current scene needs to be updated
   const auto current_time = OpenGLWindow.GetCurrentTime();
   if(!CurrentScene->isCurrent(current_time)) CurrentScene = CurrentScene->NextScene.get();

   // Update models in current scene
   CurrentScene->UpdateModels(current_time);
}

void
Visualiser::ManageUserInputs()
{
   ActiveCamera->KeyControl(OpenGLWindow.Keys, OpenGLWindow.GetDeltaTime());
   ActiveCamera->MousePositionControl(OpenGLWindow.GetMouseDisplacement());
   ActiveCamera->MouseWheelControl(OpenGLWindow.GetMouseWheelDisplacement());
}

void
Visualiser::UpdateViewFrustum()
{
   // If the viewport was modified, update the view frustum and adjust line shader resolution
   if(wasViewPortModified)
   {
      ActiveCamera->SetViewFrustum(OpenGLWindow.ComputeViewportAspectRatio());
      Shaders["Line"].SetUniform2f("u_resolution", OpenGLWindow.ViewportDimensions[0], OpenGLWindow.ViewportDimensions[1]);
   }
}

void
Visualiser::RenderScene()
{
   CurrentScene->RenderDirectionalShadows(Shaders["DirectionalShadow"]);
   CurrentScene->RenderPointShadows(Shaders["PointShadow"]);
   OpenGLWindow.ResetViewPort();
   CurrentScene->RenderScene(Shaders["General"], *ActiveCamera);
}

}

