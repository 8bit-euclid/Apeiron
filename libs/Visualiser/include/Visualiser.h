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
#include "FileManager/include/FileSystem.h"

#include "Camera.h"
#include "GlyphSheet.h"
#include "GUI.h"
#include "ModelFactory.h"
#include "Light.h"
#include "ModelGroup.h"
#include "Material.h"
#include "Scene.h"
#include "PostProcessor.h"
#include "Shader.h"
#include "Window.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <unordered_map>

namespace aprn::vis {

class Visualiser
{
 public:
   Visualiser();

   Visualiser(GLint window_width, GLint window_height);

   void Add(Scene& scene, const std::string& name = "");

   void Add(Camera& camera, const std::string& name = "");

   void Add(Camera&& camera, const std::string& name = "");

   void Animate();

 private:
   void Init();

   void InitWindow();

   void InitGUI();

   void InitScenes();

   void InitTeXBoxes();

   void InitTextures();

   void InitCameras();

   void InitShaders();

   void InitPostProcessor();

   void BeginFrame();

   void UpdateScene();

   void HandleUserInputs();

   void RenderScene();

   void PostProcess();

   void RenderGUIWindow();

   void AddGUIElements();

   void EndFrame();

   void Terminate();

   template<class type> using UMap = std::unordered_map<std::string, type>;

   Window              _Window;
   UMap<Scene>         _Scenes;
   UMap<Camera>        _Cameras;
   UMap<Shader>        _Shaders;
   UMap<UMap<Texture>> _Textures;
   PostProcessor       _PostProcessor;
   Camera*             _ActiveCamera;
   Scene*              _CurrentScene;
   GUI                 _GUI;
   bool                _wasViewPortModified{};
   bool                _HideCursor{false};
};

}
