#pragma once

#include "../include/Global.h"
#include "../../DataContainer/include/Array.h"
#include "../../LinearAlgebra/include/Vector.h"
#include "../include/GLDebug.h"
#include "../include/GLTypes.h"
#include "../include/Visualiser.h"

#include <memory>
#include <imgui.h>

namespace aprn::vis {

/***************************************************************************************************************************************************************
* Visuliser Test Base and Test Menu Classes
***************************************************************************************************************************************************************/
class VisualiserTest : public std::enable_shared_from_this<VisualiserTest>
{
public:
  VisualiserTest();

  virtual ~VisualiserTest() {}

  virtual void OnUpdate(const Float _delta_time) {}

  virtual void OnRender() {}

  virtual void OnImGuiRender() {}

protected:
  Model     ModelTest;
  Texture   TextureTest;
  Material  MaterialTest;
  Shader    ShaderTest;
  glm::mat4 ViewMatrix;
  glm::mat4 ProjectionMatrix;

//  template<typename t_derived>
//  std::shared_ptr<t_derived> shared_from_base()
//  {
//    return std::static_pointer_cast<t_derived>(shared_from_this());
//  }

//private:
//  using std::enable_shared_from_this<VisualiserTest>::shared_from_this;
};

class TestMenu : public VisualiserTest
{
public:
  TestMenu();

  ~TestMenu();

  void AddTest(std::shared_ptr<VisualiserTest> _new_test);

  void OnImGuiRender() override;

private:
  std::shared_ptr<VisualiserTest> pCurrentTest;
  DynamicArray<std::pair<std::string, std::shared_ptr<VisualiserTest>>> Tests;
};

/***************************************************************************************************************************************************************
* Clear Colour Test Class
***************************************************************************************************************************************************************/
class TestClearColour : public VisualiserTest
{
public:
  TestClearColour() = default;

  ~TestClearColour() = default;

  void OnUpdate(const Float _delta_time) override;

  void OnRender() override;

  void OnImGuiRender() override;

private:
  SVector4<GLfloat> ClearColour{0.2f, 0.3f, 0.8f, 1.0f};
};

/***************************************************************************************************************************************************************
* Texture Test Class
***************************************************************************************************************************************************************/
class TestTexture2D : public VisualiserTest
{
public:
  TestTexture2D();

  ~TestTexture2D() = default;

  void OnUpdate(const Float _delta_time) override;

  void OnRender() override;

  void OnImGuiRender() override;

private:
  glm::vec3 TranslationA{200, 200, 0};
  glm::vec3 TranslationB{400, 200, 0};
};

}
