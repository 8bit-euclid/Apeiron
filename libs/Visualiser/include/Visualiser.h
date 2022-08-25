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
#include "ModelFactory.h"
#include "Light.h"
#include "Model.h"
#include "Material.h"
#include "Scene.h"
#include "FrameTexture.h"
#include "Shader.h"
#include "Window.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <map>

namespace aprn::vis {

class Visualiser
{
 public:
   Visualiser();

   Visualiser(GLint window_width, GLint window_height);

   void Add(Scene& scene, const std::string& name = "");

   void Add(Camera&& camera, const std::string& name = "");

   void Render();

 private:
   void Init();

   void InitScenes();

   void InitTeXBoxes();

   void InitTextures();

   void InitScreenTexture();

   void BeginFrame();

   void UpdateScene();

   void HandleUserInputs();

   void UpdateViewFrustum();

   void RenderScene();

   void RenderFrameTexture();

   void EndFrame();

   template<class type> using UMap = std::unordered_map<std::string, type>;
   Window              _OpenGLWindow;
   UMap<Scene>         _Scenes;
   UMap<Camera>        _Cameras;
   UMap<Shader>        _Shaders;
   UMap<UMap<Texture>> _Textures;
   FrameTexture        _FrameTexture;
   Camera*             _ActiveCamera;
   Scene*              _CurrentScene;
   bool                _ViewPortModified{};
   bool                _PostProcess{true};
   bool                _HDR{};
};

}
