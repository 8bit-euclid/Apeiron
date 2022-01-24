#include "../include/Global.h"
#include "../libs/Visualiser/include/Visualiser.h"
#include "../libs/Polytope/include/Polygon.h"
#include "../libs/Visualiser/test/VisualiserTest.h"

#include "../libs/DataContainer/include/MultiArray.h"
#include "../libs/Tensor/include/Tensor.h"
#include "../libs/LinearAlgebra/include/Vector.h"

using namespace Apeiron;

template<typename T, class D>
void TestFunc(const Vector<T, D>& _vector)
{
  Print("Success!");
}

int main(void)
{
  StaticTensor<int, 2, 3, 5> tensor1;
  tensor1(0, 2, 4) = 1;

  DynamicTensor<int> tensor2(3, 4, 5);
  tensor2(0, 2, 4) = 1;

  StaticArray<Float, 3> t;

  FOR_EACH(item, t) item = 1;

  StaticMultiArray<int, 2, 4, 5> marray1;
  marray1(0, 2, 4) = 1;

  DynamicMultiArray<int> marray2(3, 4, 5);
  marray2(0, 2, 4) = 1;

  unsigned count(1);
//  FOR_EACH(item, marray2) Print("Count: ", count++);
  FOR_EACH(item, tensor2) Print("Count: ", count++);

  StaticVector<Float, 3> vec1(True);
  DynamicVector<Float> vec2(3, Five);

  TestFunc(vec1);
  TestFunc(vec2);

//  StaticMultiArray<int, 3, 4, 5> tensor1;
//  tensor1(0, 2, 4) = 1;
//
//  DynamicMultiArray<int> tensor2(3, 4, 5);
//  tensor2(0, 2, 4) = 1;

//  Geometry::Polygon p(8, 5.0);

  Visualiser visualiser;

  // Tetrahedron model
  visualiser.Models.resize(1);
//  CreateTetrahedron(visualiser.Models[0], 1.0f);
  CreateCube(visualiser.Models[0], 1.0f);

  // Floor model
  visualiser.Models.resize(2);
  CreateSquare(visualiser.Models[1], 10.0f);

  // Wall model
  visualiser.Models.resize(3);
  CreateSquare(visualiser.Models[2], 5.0f);

  // Shaders
//  visualiser.Shaders.emplace_back("libs/Visualiser/resources/shaders/Minimal.glsl");
//  visualiser.Shaders.emplace_back("libs/Visualiser/resources/shaders/Basic.glsl");
//  visualiser.Shaders.emplace_back("libs/Visualiser/resources/shaders/Intermediate.glsl");
  visualiser.Shaders.emplace_back("libs/Visualiser/resources/shaders/Advanced.glsl");
  visualiser.Shaders.emplace_back("libs/Visualiser/resources/shaders/DirectionalShadow.glsl");
  visualiser.Shaders.emplace_back("libs/Visualiser/resources/shaders/PointShadow.glsl");
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
//
//  VertexArray vertex_array;
//  vertex_array.Bind();


//  visualiser.Materials.emplace_back(0.3, 1024.0);
//  visualiser.Materials.emplace_back(0.7, 1024.0);
  visualiser.Materials.emplace_back(0.8, 256.0);

//  visualiser.Textures.emplace_back("libs/Visualiser/resources/textures/BrickWall_003_1K/BrickWall_003_1K.jpg");
//  visualiser.Textures.emplace_back("libs/Visualiser/resources/textures/BrickWall_003_1K/BrickWall_003_1K_Normal.jpg");

  visualiser.Textures.emplace_back("libs/Visualiser/resources/textures/BrickWall_001_2K/Diffuse.png");
  visualiser.Textures.emplace_back("libs/Visualiser/resources/textures/BrickWall_001_2K/Normal.png");
  visualiser.Textures.emplace_back("libs/Visualiser/resources/textures/BrickWall_001_2K/Displacement.png");

//  visualiser.Textures.emplace_back("libs/Visualiser/resources/textures/WoodToyBox_1K/Diffuse.png");
//  visualiser.Textures.emplace_back("libs/Visualiser/resources/textures/WoodToyBox_1K/Normal.png");
//  visualiser.Textures.emplace_back("libs/Visualiser/resources/textures/WoodToyBox_1K/Displacement.png");

//  visualiser.Textures.emplace_back("libs/Visualiser/resources/textures/WoodFloor_001_4K/Diffuse.png");
//  visualiser.Textures.emplace_back("libs/Visualiser/resources/textures/WoodFloor_001_4K/Normal.png");
//  visualiser.Textures.emplace_back("libs/Visualiser/resources/textures/Paper_001_4K/Paper_001_4K.png");
//  visualiser.Textures.emplace_back("libs/Visualiser/resources/textures/Paper_001_4K/Paper_001_4K_Normal.png");
//  visualiser.Textures.emplace_back("libs/Visualiser/resources/textures/ChernoLogo.png");
//  visualiser.Textures.emplace_back("libs/Visualiser/resources/textures/Papyrus.png");
//  visualiser.Textures.emplace_back("libs/Visualiser/resources/textures/Cloth.jpg");

  visualiser.PointLights.emplace_back(glm::vec3(0.0, 2.0, -2.0), glm::vec4(1.0, 1.0, 1.0, 1.0), 0.4, 0.8, SVector3<GLfloat>{0.3, 0.1, 0.0});
//  visualiser.PointLights.emplace_back(glm::vec3(-4.0, 2.0, -1.0), glm::vec4(1.0, 1.0, 1.0, 1.0), 0.7, 0.6, SVectorT3<GLfloat>{0.3, 0.1, 0.05});
//  visualiser.PointLights.emplace_back(glm::vec3(4.0, 2.0, -1.0), glm::vec4(1.0, 1.0, 1.0, 1.0), 0.7, 0.6, SVectorT3<GLfloat>{0.3, 0.1, 0.05});
//  visualiser.PointLights.emplace_back(glm::vec3(-4.0, 2.0, 0.0), glm::vec4(0.0, 1.0, 0.0, 1.0), 0.5, 0.3, SVectorT3<GLfloat>{0.3, 0.1, 0.1});

//  visualiser.SpotLights.emplace_back(glm::vec3(0.0, 3.0, 0.0), glm::vec3(0.0, -1.0, 0.0), glm::vec4(1.0, 1.0, 1.0, 1.0), 20.0, 1.0, 0.7, SVectorT3<GLfloat>{1.0, 0.0, 0.0});
//  visualiser.SpotLights.emplace_back(glm::vec3(3.0, 1.0, 0.0), glm::vec3(-5.0, -1.0, 0.0), glm::vec4(1.0, 1.0, 1.0, 1.0), 20.0, 0.5, 0.3, SVectorT3<GLfloat>{1.0, 0.0, 0.0});

  visualiser.Cameras.resize(1);
  visualiser.Cameras[0].SetOrientation(glm::vec3(0.0f, 0.0f, 1.0f), 0.0, 90.0);
  visualiser.Cameras[0].SetViewFrustum(visualiser.ViewportAspectRatio(), 45.0, 1.0, -100.0);

//  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  while(visualiser.isWindowOpen())
  {
    visualiser.BeginFrame();

    visualiser.Cameras[0].KeyControl(visualiser.GetKeys(), visualiser.GetDeltaTime());
    visualiser.Cameras[0].MousePositionControl(visualiser.GetMouseDisplacement());
    visualiser.Cameras[0].MouseWheelControl(visualiser.GetMouseWheelDisplacement());

    // If the viewport has been modified, update the view frustum
    if(visualiser.isViewPortModified)
    {
      auto [width, height] = visualiser.ViewportDimensions();
      visualiser.Cameras[0].SetViewFrustum(visualiser.ViewportAspectRatio());
      visualiser.Shaders[0].SetUniform2f("u_resolution", width, height);
    }

    visualiser.RenderDirectionalShadows();
    visualiser.RenderPointShadows();
    visualiser.RenderScene();

    visualiser.EndFrame();
  }

  return 0;
}





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