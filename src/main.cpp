#include "../include/Global.h"
#include "../libs/Visualiser/include/Visualiser.h"
#include "../libs/Polytope/include/Polygon.h"
#include "../libs/Visualiser/test/VisualiserTest.h"

#include "../libs/DataContainer/include/MultiArray.h"
#include "../libs/Tensor/include/Tensor.h"
#include "../libs/LinearAlgebra/include/Vector.h"
#include "../libs/LinearAlgebra/include/VectorOperations.h"
#include "../libs/Manifold/include/Curve.h"

using namespace aprn;
using namespace aprn::vis;

int main(void)
{
   Visualiser vis;

   Model model;

   // Cube
//   model = ModelFactory::Cube(1.0);
//   model.Rotate(DegToRad(angl_offs), {0.0f, 0.0f, 1.0f});
//        .MoveAlong({x_offs, 0.0f, 0.0f});
//   visualiser.AddModel(model, "Cube");

   // Floor
   model = ModelFactory::Square(10.0);
   model.SetMaterial("Brick", 0.8, 256.0)
        .SetTexture("Brick", "Wall", 1, 2)
        .SetColour({1.0, 0.0, 0.0})
        .OffsetPosition({0.0f, -2.0f, 0.0f})
        .OffsetOrientation(-HalfPi, {1.0f, 0.0f, 0.0f});
   vis.Add(model, "Floor");

//   // Wall 0
//   model = ModelFactory::Square(5.0);
//   model.SetMaterial("Brick", 0.8, 256.0)
//        .SetTexture("Brick", "Wall", 1, 2)
//        .OffsetPosition({2.5f, 0.5f, -5.0f});
//   vis.Add(model, "Wall 0");

//   // Wall 1
//   model = ModelFactory::Square(5.0);
//   model.SetMaterial("Brick", 0.8, 256.0)
//        .SetTexture("Brick", "Wall", 1, 2)
//        .OffsetPosition({-2.5f, 0.5f, -5.0f});
//   vis.Add(model, "Wall 0");

//   vis.Add(PointLight(glm::vec3(0.0, 2.0, -2.0), glm::vec4(1.0, 1.0, 1.0, 1.0), 0.4, 0.8, SVector3<GLfloat>{0.3, 0.1, 0.0}));

   vis.Render();

   return 0;
}



//   static float x_incr(0.008);
//   static float x_offs(0.0);
//   static float x_sign(1.0);
//
//   static float angl_incr(0.1);
//   static float angl_offs(0.0);

//   if(_shader_index == 1)
//   {
//     x_offs += x_sign * x_incr;
//     if(Abs(x_offs) > 2.0) x_sign *= -1.0;
//
//     angl_offs += angl_incr;
//     if(angl_offs > 360.0) angl_offs -= 360.0;
//   }


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





//int main(void)
//{
//  Visualiser visualiser;
//  std::shared_ptr<TestMenu> p_test = std::make_shared<TestMenu>();
////  std::shared_ptr<VisualiserTest> p_test = std::make_shared<TestMenu>();
//
//  GLCall(glEnable(GL_BLEND));
//  GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
//
//  ImGui::CreateContext();
//  ImGui_ImplOpenGL3_Init("#version 460");
//  ImGui_ImplGlfw_InitForOpenGL(visualiser.GetGLFWWindow(), true);
//  ImGui::StyleColorsDark();
//
//  // Loop until the user closes the window
//  while(visualiser.isWindowOpen())
//  {
//    visualiser.BeginFrame();
//
//    ImGui_ImplOpenGL3_NewFrame();
//    ImGui_ImplGlfw_NewFrame();
//    ImGui::NewFrame();
//
//
//    ImGui::Begin("Test");
//
//    p_test->OnImGuiRender();
//
//    ImGui::End();
//
//    ImGui::Render();
//
//    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//
//    visualiser.EndFrame();
//  }
//
//  ImGui_ImplOpenGL3_Shutdown();
//  ImGui_ImplGlfw_Shutdown();
//  ImGui::DestroyContext();
//
//  glfwTerminate();
//  return 0;
//}