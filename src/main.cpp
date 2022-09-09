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

#include "../include/Global.h"
#include "../include/Testing.h"
#include "Visualiser/include/Visualiser.h"
#include "Visualiser/include/Scene.h"

#include "FileManager/include/File.h"
#include "FileManager/include/FileSystem.h"

#include <string>
#include <string_view>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

using namespace aprn::test;

int main(void)
{
//   RotatingCube();
   EuclidsElementsEp1();

   return 0;
}

//  visualiser.Shaders.emplace_back("libs/Visualiser/resources/shaders/Line.glsl");
//  visualiser.Shaders[0].Bind();
//  visualiser.Shaders[0].SetUniform1f("u_thickness", 20.0);
//  visualiser.Shaders[0].Unbind();
//  DynamicArray<glm::vec4> varray;
//  varray.emplace_back(glm::vec4(-0.6f, -0.6f, 0.0f, 1.0f));
//  varray.emplace_back(glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f));
//  varray.emplace_back(glm::vec4(0.5f, 0.5f, 0.0f, 1.0f));
//  varray.emplace_back(glm::vec4(0.6f, 0.6f, 0.0f, 1.0f));
//  ShaderStorageBuffer shader_storage_buffer(varray);
//  VertexArray vertex_array;
//  vertex_array.Bind();

//  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);