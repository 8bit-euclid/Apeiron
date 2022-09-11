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

#include "Visualiser/include/Visualiser.h"
#include "Visualiser/include/Model.h"
#include "Visualiser/include/ModelFactory.h"
#include "Visualiser/include/Scene.h"

using namespace aprn::vis;

namespace aprn::test {

inline void RotatingCube()
{
   Visualiser visualiser;
   Scene scene;
   Model model;
   TeXBox tex_box;

   tex_box.Add(R"(This is a test: $e = mc^2$.)")
          .SetPixelDensity(2000)
          .OffsetPosition({0.0f, 2.0f, 2.0f});
   scene.Add(tex_box);

   // Cube
   model = ModelFactory::Cube(1.0);
   model.SetColour({1.0, 1.0, 1.0, 1.0})
        .RotateAt({0.0f, 0.0f, 1.0f}, 2.0)
        .Trace([](Float t){ return SVectorF3{ Three*std::sin(TwoThird * t), Zero, Zero }; }, 2.0);
   scene.Add(model, "Cube");

   // Floor
   const Float height_scale = 0.08;
   model = ModelFactory::Square(10.0);
   model.SetMaterial("Brick", 0.8, 256.0)
        .SetTexture("Brick", "Wall", 1, 2, height_scale)
        .OffsetPosition({0.0f, -2.0f, 0.0f})
        .OffsetOrientation(-HalfPi, {1.0f, 0.0f, 0.0f});
   scene.Add(model);

   // Wall 0
   model = ModelFactory::Square(5.0);
   model.SetMaterial("Brick", 0.8, 256.0)
        .SetTexture("Brick", "Wall", 1, 2, height_scale)
        .OffsetPosition({2.5f, 0.5f, -5.0f});
   scene.Add(model);

   // Wall 1
   model = ModelFactory::Square(5.0);
   model.SetMaterial("Brick", 0.8, 256.0)
        .SetTexture("Brick", "Wall", 1, 2, height_scale)
        .OffsetPosition({-2.5f, 0.5f, -5.0f});
   scene.Add(model)
        .Add(PointLight(glm::vec3(0.0, -0.5, -2.0), glm::vec4(1.0, 1.0, 1.0, 1.0), 20.0, 2.5, {0.01, 0.5, 0.5}), "Sun");

   visualiser.Add(scene);
   visualiser.Animate();
}

inline void EuclidsElementsEp1()
{
   Visualiser visualiser;
   Scene scene;
   Model model;
   TeXBox tex_box;

   tex_box.Add(R"(This is a test: $e = mc^2$.)")
          .SetPixelDensity(2000)
          .OffsetPosition({0.0, 0.0, 0.05});
   scene.Add(tex_box);

   // Paper sheet
   model = ModelFactory::Square(10.0);
   model.SetMaterial("Paper", 0.1, 64.0)
        .SetTexture("Paper", 2, 4, 0.02);
   scene.Add(model);

   // Lighting
   scene.Add(PointLight(glm::vec3(0.0, 0.0, 2.6), glm::vec4(1.0, 197.0/255.0, 143.0/255.0, 1.0), 5.0, 1.0, {0.1, 0.5, 0.5}), "Lamp");

   visualiser.Add(scene);
   visualiser.Animate();
}

}
