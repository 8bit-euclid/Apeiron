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

#include "../include/Visualiser.h"

#include <execution>
#include <optional>
#include <string>

namespace aprn::vis {

/***************************************************************************************************************************************************************
* Public Interface
***************************************************************************************************************************************************************/
Visualiser::Visualiser()
   : Visualiser(1920, 1080) {}

Visualiser::Visualiser(GLint window_width, GLint window_height)
   : _Window(window_width, window_height), _Cameras{{"Main", Camera()}}, _ActiveCamera(&_Cameras["Main"]) {}

void
Visualiser::Add(Scene& scene, const std::string& name)
{
   const std::string& id = name.empty() ? "Scene_" + ToString(_Scenes.size()) : name;
   _Scenes.emplace(id, std::move(scene));
}

void
Visualiser::Add(Camera& camera, const std::string& name) { Add(std::move(camera), name); }

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
   while(_Window.isOpen())
   {
      BeginFrame();
      UpdateScene();
      HandleUserInputs();
      RenderScene();
      PostProcess();
      EndFrame();
   }
}

/***************************************************************************************************************************************************************
* Private Interface
***************************************************************************************************************************************************************/
void
Visualiser::Init()
{
   // Open a window and set its title.
   _Window.Open();
   _Window.SetTitle("Apeiron");

   // Initialise all scenes (and their models and lights), tex-boxes, textures, cameras, shaders, and the post-processor.
   InitOpenGL();
   InitScenes();
   InitTeXBoxes();
   InitTextures();
   InitCameras();
   InitShaders();
   InitPostProcessor();

   // Zero the clock time.
   _Window.InitTime();
}

void
Visualiser::InitOpenGL()
{
   // Initialise OpenGL debug output
#ifdef DEBUG_MODE
   int flags;
   GLCall(glGetIntegerv(GL_CONTEXT_FLAGS, &flags))
   if(flags & GL_CONTEXT_FLAG_DEBUG_BIT)
   {
      GLCall(glEnable(GL_DEBUG_OUTPUT))
      GLCall(glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS))
      GLCall(glDebugMessageCallback(GLDebugOutput, nullptr))
      GLCall(glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE))
   }
#endif

   GLCall(glEnable(GL_DEPTH_TEST))
   GLCall(glEnable(GL_MULTISAMPLE))
//   GLCall(glEnable(GL_CULL_FACE))
//   GLCall(glCullFace(GL_FRONT))
//   GLCall(glFrontFace(GL_CCW))
   GLCall(glEnable(GL_BLEND))
   GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA))
//   GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE))
}

void
Visualiser::InitScenes()
{
   auto first_scene_it = std::find_if(_Scenes.begin(), _Scenes.end(), [](const auto& entry){ return entry.second._PrevScene == nullptr; });
   ASSERT(first_scene_it != _Scenes.end(), "Failed to locate the first scene.")

   _CurrentScene = &first_scene_it->second;
   Scene* current_scene(_CurrentScene);
   size_t scene_count{};
   Float  start_time{};

   // Loop through scenes
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
         current_scene = current_scene->_NextScene; // Move over to the next scene
      }

      // Initialise current scene and update count
      current_scene->Init(start_time);
      ++scene_count;
   }
   while(current_scene->_NextScene);

   ASSERT(scene_count == _Scenes.size(), "There was a mismatch in the total number of scenes.")
}

void
Visualiser::InitTeXBoxes()
{
   // Linearise pointers to all TeX-boxes to allow for parallel initialisation.
   DArray<std::pair<size_t, TeXBox*>> tex_boxes;
   tex_boxes.reserve(10 * _Scenes.size());
   FOR_EACH(_, scene, _Scenes) FOR_EACH(_, tex_box, scene._TeXBoxes) tex_boxes.push_back({ tex_boxes.size(), tex_box.get() });

   // Initialise LaTeX compilation directory, compile all LaTeX source code, generate glyph sheets, and initialise underlying tex-box Model.
   TeXBox::InitTeXDirectory();
   FOR(i, tex_boxes.size()) tex_boxes[i].second->Init(i);

   // Load tex-box model textures. Note: only diffuse texture required.
   FOR_EACH(_, scene, _Scenes)
      FOR_EACH_CONST(_, tex_box, scene._TeXBoxes)
      {
         const auto texture_name = tex_box->_Label + "_texture"; // TODO - what if the label is empty??!
         const auto texture_type = TextureType::Diffuse;
         const auto type_string  = TextureTypeString(texture_type);

         // Load compiled tex-box image as a texture.
         UMap<Texture> texture_files;
         texture_files.emplace(type_string, Texture(texture_type, tex_box->ImagePath()));
         _Textures.emplace(texture_name, std::move(texture_files));

         // Point to the textures from the scene.
         UMap<Texture&> texture_file_map;
         FOR_EACH(sub_texture_name, sub_texture, _Textures[texture_name]) texture_file_map.emplace(sub_texture_name, sub_texture);
         scene._Textures.emplace(texture_name, texture_file_map);

         // Point to the texture from the tex-box model.
         tex_box->_TextureInfo = { texture_name, Zero };
      }
}

void
Visualiser::InitTextures()
{
   // Load model textures
   FOR_EACH(_, scene, _Scenes)
      FOR_EACH_CONST(_, model, scene._Models)
         if(model->_TextureInfo.has_value())
         {
            const auto& texture_info = model->_TextureInfo;
            const auto& texture_name = texture_info.value().first;

            if(!_Textures.contains(texture_name))
            {
               // Add all files associated to the given texture
               const auto  texture_list = { TextureType::Diffuse,
                                            TextureType::Normal,
                                            TextureType::Displacement }; // Add appropriate enums if more textures are to be read
               UMap<Texture> texture_files;
               FOR_EACH_CONST(texture_type, texture_list)
               {
                  const auto path = TexturePath(TextureDirectory(texture_name), texture_type);
                  if(path.has_value())
                  {
                     texture_files.emplace(TextureTypeString(texture_type), Texture(texture_type, path.value()));
                     if(texture_type == TextureType::Displacement)
                     {
                        const auto& displacement_map_scale = texture_info.value().second;
                        texture_files.at(TextureTypeString(texture_type)).SetMapScale(displacement_map_scale);
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

         }
}

void
Visualiser::InitCameras()
{
   // Set default settings of the main camera.
   ASSERT(_ActiveCamera, "The active camera pointer has not yet been set.")
   _ActiveCamera->SetOrientation(glm::vec3(0.0f, 0.0f, 1.0f), 0.0, 90.0);
   _ActiveCamera->SetViewFrustum(_Window.ViewportAspectRatio(), 45.0, 1.0, -100.0);
}

void
Visualiser::InitShaders()
{
   for(std::string shader : {"Default", "DirecShadow", "PointShadow", "Line"})
      _Shaders.emplace(shader, Shader::Directory + shader + ".glsl");
}

void
Visualiser::InitPostProcessor()
{
   const auto [width, height] = _Window.ViewportDimensions(); // Note: must not use the window dimensions here.
   _PostProcessor.Init(width, height);
}

void
Visualiser::BeginFrame()
{
   // Clear currently bound frame buffer.
   ClearFrameBuffer();

   // Update the current and previous times, compute delta time, compute and display frame-rate, and check if the viewport was modified.
   _Window.ComputeDeltaTime();
   _Window.ComputeFrameRate();
   _ViewPortModified = _Window.isViewportModified();
}

void
Visualiser::UpdateScene()
{
   DEBUG_ASSERT(_CurrentScene, "The current scene pointer has not yet been set.")

   // Determine if the current scene needs to be updated
   const auto current_time = _Window.CurrentTime();
   if(!_CurrentScene->isCurrent(current_time))
   {
      ASSERT(_CurrentScene->_NextScene, "TODO - need to properly handle final scene.")
      _CurrentScene = _CurrentScene->_NextScene;
   }

   // Update models in current scene
   _CurrentScene->UpdateModels(current_time);
}

void
Visualiser::HandleUserInputs()
{
   // Handle key, cursor, and mouse wheel inputs.
   _ActiveCamera->KeyControl(_Window._Keys, _Window.DeltaTime());
   _ActiveCamera->CursorControl(_Window.CursorDisplacement());
   _ActiveCamera->WheelControl(_Window.WheelDisplacement());

   // If the viewport was modified, update the view frustum and adjust line shader resolution.
   if(_ViewPortModified)
   {
      _ActiveCamera->SetViewFrustum(_Window.ViewportAspectRatio());
      _Shaders.at("Line").SetUniform2f("u_resolution", _Window._ViewportDimensions[0], _Window._ViewportDimensions[1]);
   }
}

void
Visualiser::RenderScene()
{
   // Render shadows from all directional and point light sources.
   _CurrentScene->RenderDirecShadows(_Shaders.at("DirecShadow"));
   _CurrentScene->RenderPointShadows(_Shaders.at("PointShadow"));

   // Point shadow rendering modifies the viewport, so need to reset it.
   _Window.ResetViewport();

   // Write to off-screen frame buffer.
   _PostProcessor.StartWrite();

   // Render all elements of the current scene.
   _CurrentScene->RenderScene(_Shaders.at("Default"), *_ActiveCamera);

   // Finalise off-screen render.
   _PostProcessor.StopWrite();
}

void
Visualiser::PostProcess() { _PostProcessor.Render(); }

void
Visualiser::EndFrame()
{
   _Window.SwapBuffers();
   glfwPollEvents();
}

}

