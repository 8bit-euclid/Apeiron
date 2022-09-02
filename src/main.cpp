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
#include "Visualiser/include/Visualiser.h"
#include "Visualiser/include/Scene.h"

#include "FileManager/include/File.h"
#include "FileManager/include/FileSystem.h"

#include <string>
#include <string_view>

using namespace aprn;
using namespace aprn::vis;

int main(void)
{
   Visualiser visualiser;
   Scene scene(1000.0);
   Model model;

//   scene.Add(TeXBox(R"(This is a test $e = mc^2$.)", ""));
//   scene.Add(TeXBox(R"(This is another test: $p_4(x) = a_0 + a_1x + a_2x^2 + a_3x^3 + a_4x^4$.)", ""));
//   scene.Add(TeXBox(R"(This is yet \textbf{another} test with a \textit{slightly} more complicated mathematical equation: $s = u\tau + \frac{1}{2}a\tau^2$.)", ""));

//   scene.Add(TeXBox(R"($\sum$)", ""));
//   scene.Add(TeXBox(R"(Test $\tau$ and $\sum$,)", ""));
//   scene.Add(TeXBox(R"(This is \textbf{Bold}, \textit{italic} polynomial: $p_2(x) = a_0 + a_1x + a_2x^2$. Equation with commands and a fraction: $s = u\tau + \frac{1}{2}a\tau^2$.)", ""));

   TeXBox tex_box;
   tex_box.Add(R"(Test $\tau$ and $\sum$,)");
//          .OffsetPosition({0.0f, -2.0f, 0.0f})
//          .OffsetOrientation(-HalfPi, {1.0f, 0.0f, 0.0f});
   scene.Add(tex_box);

   // Cube
   model = ModelFactory::Cube(1.0);
   model.SetColour({1.0, 1.0, 1.0})
        .RotateAt({0.0f, 0.0f, 1.0f}, 2.0)
        .Trace([](Float t){ return SVectorF3{ Three*std::sin(TwoThird * t), Zero, Zero }; }, 2.0);
   scene.Add(model, "Cube");

   // Floor
   model = ModelFactory::Square(10.0);
   model.SetMaterial("Brick", 0.8, 256.0)
        .SetTexture("Brick", "Wall", 1, 2)
        .OffsetPosition({0.0f, -2.0f, 0.0f})
        .OffsetOrientation(-HalfPi, {1.0f, 0.0f, 0.0f});
   scene.Add(model);

   // Wall 0
   model = ModelFactory::Square(5.0);
   model.SetMaterial("Brick", 0.8, 256.0)
        .SetTexture("Brick", "Wall", 1, 2)
        .OffsetPosition({2.5f, 0.5f, -5.0f});
   scene.Add(model);

   // Wall 1
   model = ModelFactory::Square(5.0);
   model.SetMaterial("Brick", 0.8, 256.0)
        .SetTexture("Brick", "Wall", 1, 2)
        .OffsetPosition({-2.5f, 0.5f, -5.0f});
   scene.Add(model)
        .Add(PointLight(glm::vec3(0.0, -0.5, -2.0), glm::vec4(1.0, 1.0, 1.0, 1.0), 20.0, 2.5, {0.01, 0.5, 0.5}), "Sun");
//        .Add(DirectionalLight(glm::vec3(0.0, -1.0, -1.0), glm::vec4(1.0, 1.0, 1.0, 1.0), 0.3, 0.6), "Sun");

//   // Floor
//   model = ModelFactory::Square(10.0);
//   model.SetMaterial("Paper", 0.1, 16.0)
//        .SetColour({1, 1, 1});
////        .SetTexture("Paper", "", 2, 4)
//        .OffsetPosition({0.0f, -2.0f, 0.0f})
//        .OffsetOrientation(-HalfPi, {1.0f, 0.0f, 0.0f});
//   scene.Add(model);

   visualiser.Add(scene);
   visualiser.Render();

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