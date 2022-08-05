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
   : _OpenGLWindow(window_width, window_height), _Cameras{{"Main", Camera()}}, _ActiveCamera(&_Cameras["Main"]) {}

void
Visualiser::Add(Scene& scene, const std::string& name)
{
   const std::string& id = name.empty() ? "Scene_" + ToString(_Scenes.size()) : name;
   _Scenes.emplace(id, std::move(scene));
}

void
Visualiser::Add(Camera&& camera, const std::string& name)
{
   const std::string& id = name.empty() ? "Camera_" + ToString(_Cameras.size()) : name;
   _Cameras.emplace(id, std::move(camera));
}

void
Visualiser::Render()
{
   Init();

   while(_OpenGLWindow.isOpen())
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
   ASSERT(_ActiveCamera, "The active camera pointer has not yet been set.")
   _ActiveCamera->SetOrientation(glm::vec3(0.0f, 0.0f, 1.0f), 0.0, 90.0);
   _ActiveCamera->SetViewFrustum(_OpenGLWindow.ComputeViewportAspectRatio(), 45.0, 1.0, -100.0);

   // Load all shaders
   _Shaders.emplace("General", "libs/Visualiser/resources/shaders/General.glsl");
   _Shaders.emplace("Line", "libs/Visualiser/resources/shaders/Line.glsl");
   _Shaders.emplace("DirectionalShadow", "libs/Visualiser/resources/shaders/DirectionalShadow.glsl");
   _Shaders.emplace("PointShadow", "libs/Visualiser/resources/shaders/PointShadow.glsl");

   // Load requires textures and initialise scenes
   LoadTextures();
   InitScenes();

   // Set clock time to zero
   _OpenGLWindow.ResetTime();
}

void
Visualiser::LoadTextures()
{
   // Load model textures
   FOR_EACH(_, scene, _Scenes)
      FOR_EACH_CONST(_, model, scene._Models)
         if(model->_Texture.has_value() && !_Textures.contains(model->_Texture.value()))
         {
            // Add all files associated to the given texture
            const auto& texture_name = model->_Texture.value();
            const auto  texture_list = {TextureType::Diffuse,
                                        TextureType::Normal,
                                        TextureType::Displacement}; // Add appropriate enums if more textures are to be read
            UMap<Texture> texture_files;
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
            _Textures.emplace(texture_name, std::move(texture_files));

            // Point to the textures from the scene
            UMap<Texture&> texture_file_map;
            FOR_EACH(sub_texture_name, sub_texture, _Textures[texture_name]) texture_file_map.emplace(sub_texture_name, sub_texture);
            scene._Textures.emplace(texture_name, texture_file_map);
         }

   // Load font/glyph sheets as textures
   FOR_EACH(_, scene, _Scenes)
      FOR_EACH_CONST(_, tex_box, scene._TeXBoxes)
      {

      }
}

void
Visualiser::InitScenes()
{
   auto first_scene_it = std::find_if(_Scenes.begin(), _Scenes.end(), [](const auto& entry){return entry.second._PrevScene == nullptr;});
   ASSERT(first_scene_it != _Scenes.end(), "Could not locate the first scene.")

   _CurrentScene = &first_scene_it->second;
   Scene* current_scene(_CurrentScene);
   size_t scene_count{};
   Float  start_time{};
   do
   {
      // Update current scene if it isn't the first iteration
      if(scene_count)
      {
         // Sync the start-time of the next scene (or the transition to it) to the end-time of the current scene.
         start_time = current_scene->_EndTime;

         // Initialise the scene transition if there is one, and re-update the start time of the next scene.
         if(current_scene->_Transition._Type != TransitionType::None)
         {
            auto& transition = current_scene->_Transition;
            transition.Init(start_time);
            start_time = transition._EndTime;
         }

         // Move to next scene
         current_scene = current_scene->_NextScene;
      }

      // Initialise current scene and update count
      current_scene->Init(start_time);
      ++scene_count;
   }
   while(current_scene->_NextScene);

   ASSERT(scene_count == _Scenes.size(), "There was a mismatch in the total number of scenes.")
}

void
Visualiser::StartFrame()
{
   // Clear window
   GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
   GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

   // Update the current and previous times, compute delta time, and check if the viewport was modified.
   _OpenGLWindow.ComputeDeltaTime();
   _isViewPortModified = _OpenGLWindow.isViewPortModified();
}

void
Visualiser::EndFrame()
{
   _OpenGLWindow.SwapBuffers();
   glfwPollEvents();
}

void
Visualiser::UpdateScene()
{
   DEBUG_ASSERT(_CurrentScene, "The current scene pointer has not yet been set.")

   // Determine if the current scene needs to be updated
   const auto current_time = _OpenGLWindow.GetCurrentTime();
   if(!_CurrentScene->isCurrent(current_time)) _CurrentScene = _CurrentScene->_NextScene;

   // Update models in current scene
   _CurrentScene->UpdateModels(current_time);
}

void
Visualiser::ManageUserInputs()
{
   _ActiveCamera->KeyControl(_OpenGLWindow.Keys, _OpenGLWindow.GetDeltaTime());
   _ActiveCamera->MousePositionControl(_OpenGLWindow.GetMouseDisplacement());
   _ActiveCamera->MouseWheelControl(_OpenGLWindow.GetMouseWheelDisplacement());
}

void
Visualiser::UpdateViewFrustum()
{
   // If the viewport was modified, update the view frustum and adjust line shader resolution
   if(_isViewPortModified)
   {
      _ActiveCamera->SetViewFrustum(_OpenGLWindow.ComputeViewportAspectRatio());
      _Shaders["Line"].SetUniform2f("u_resolution", _OpenGLWindow.ViewportDimensions[0], _OpenGLWindow.ViewportDimensions[1]);
   }
}

void
Visualiser::RenderScene()
{
   _CurrentScene->RenderDirectionalShadows(_Shaders["DirectionalShadow"]);
   _CurrentScene->RenderPointShadows(_Shaders["PointShadow"]);
   _OpenGLWindow.ResetViewPort();
   _CurrentScene->RenderScene(_Shaders["General"], *_ActiveCamera);
}

}

