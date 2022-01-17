#include "VisualiserTest.h"

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

namespace Apeiron {

VisualiserTest::VisualiserTest()
  : TextureTest(GL_TEXTURE_2D)
{

}

TestMenu::TestMenu()
//  : pCurrentTest(shared_from_this())
{PING
  pCurrentTest = shared_from_this();
//  pCurrentTest = shared_from_base<TestMenu>();
  PING
  Tests.push_back({"Clear Colour", std::make_shared<TestClearColour>()});
  Tests.push_back({"Texture", std::make_shared<TestTexture2D>()});
}

TestMenu::~TestMenu()
{

}

void TestMenu::OnImGuiRender()
{
  for(auto [name, test] : Tests)
    if(ImGui::Button(name.c_str())) pCurrentTest = test;
}

/***************************************************************************************************************************************************************
* Clear Colour Test Class
***************************************************************************************************************************************************************/
void TestClearColour::OnUpdate(const Float _delta_time)
{

}

void TestClearColour::OnRender()
{
  GLCall(glClearColor(ClearColour[0], ClearColour[1], ClearColour[2], ClearColour[3]));
  GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void TestClearColour::OnImGuiRender()
{
  ImGui::ColorEdit4("Clear Color", ClearColour.data());
}

/***************************************************************************************************************************************************************
* Texture Test Class
***************************************************************************************************************************************************************/
TestTexture2D::TestTexture2D()
{
  CreateSquare(ModelTest, 100.0f);

  ModelTest.Geometry.Vertices[0].TextureCoordinates = glm::vec2(0.0f, 0.0f);
  ModelTest.Geometry.Vertices[1].TextureCoordinates = glm::vec2(1.0f, 0.0f);
  ModelTest.Geometry.Vertices[2].TextureCoordinates = glm::vec2(1.0f, 1.0f);
  ModelTest.Geometry.Vertices[3].TextureCoordinates = glm::vec2(0.0f, 1.0f);

  GLCall(glEnable(GL_BLEND));
  GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

  glm::mat4 proj_matrix = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f);
  glm::mat4 view_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

  ShaderTest.ReadFromFile("libs/Visualiser/resources/shaders/Basic.glsl");
  ShaderTest.Bind();

  ShaderTest.SetUniform4f("u_color", 0.8f, 0.3f, 0.8f, 1.0f);

  TextureTest.ReadFromFile("libs/Visualiser/resources/textures/ChernoLogo.png", GL_CLAMP_TO_EDGE);
  TextureTest.Bind();
  ShaderTest.SetUniform1i("u_diffuse_map", 0);

  ShaderTest.Unbind();

  ProjectionMatrix = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f);
  ViewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
}

void TestTexture2D::OnUpdate(const Float _delta_time)
{

}

void TestTexture2D::OnRender()
{
  GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
  GLCall(glClear(GL_COLOR_BUFFER_BIT));

  ShaderTest.Bind();

  ShaderTest.SetUniformMatrix4f("u_view_matrix", ViewMatrix);
  ShaderTest.SetUniformMatrix4f("u_proj_matrix", ProjectionMatrix);

  ModelTest.Reset();
  ModelTest.Translate(TranslationA);
  ShaderTest.UseModel(ModelTest);
  ModelTest.Draw();

  ModelTest.Reset();
  ModelTest.Translate(TranslationB);
  ShaderTest.UseModel(ModelTest);
  ModelTest.Draw();

  ShaderTest.Unbind();
}

void TestTexture2D::OnImGuiRender()
{
  ImGui::SliderFloat3("Translation A", &TranslationA.x, 0.0f, 960.0f);
  ImGui::SliderFloat3("Translation B", &TranslationB.x, 0.0f, 960.0f);
  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}

}
