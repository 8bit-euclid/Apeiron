#include "../include/Visualiser.h"

namespace Apeiron {

void Visualiser::BeginFrame()
{
  // Compute delta time for previous frame
  OpenGLWindow.ComputeDeltaTime();

  // Check if the viewport has been modified
  isViewPortModified = OpenGLWindow.isViewPortModified();

  // Clear window
  GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
  GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Visualiser::RenderScene()
{
  OpenGLWindow.ResetViewPort();

  // Clear window
  GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
  GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

  Shaders[0].Bind();

  Shaders[0].UseCamera(Cameras[0]);

//  Shaders[0].UseLight(MainLight);
  Shaders[0].SetDirectionalLightSpaceMatrix(MainLight.GetLightSpaceMatrix());
  MainLight.GetShadowMap().ReadFrom(1);
  Shaders[0].SetDirectionalShadowMap(1);

  FOR(i, PointLights.size())
  {
    Shaders[0].UseLight(PointLights[i]);
    PointLights[i].GetShadowMap().ReadFrom(i + 2);
    Shaders[0].SetPointShadowMap(i, i + 2);
  }
  Shaders[0].SetPointFarPlane(PointLight::GetFarPlane());

//  Shaders[0].UseLight(PointLights[1]);
//  Shaders[0].UseLight(SpotLights[0]);
//  Shaders[0].UseLight(SpotLights[1]);

  RenderModels(0);

  MainLight.GetShadowMap().Finalise();

  Shaders[0].Unbind();
}

void Visualiser::RenderDirectionalShadows()
{
  Shaders[1].Bind();
  Shaders[1].SetDirectionalLightSpaceMatrix(MainLight.GetLightSpaceMatrix());

  const Shadow& shadow_map = MainLight.GetShadowMap();

  GLCall(glViewport(0, 0, shadow_map.GetDepthMap().GetWidth(), shadow_map.GetDepthMap().GetHeight()));

//  GLCall(glCullFace(GL_FRONT)); // Prevents peter-panning

  shadow_map.WriteTo();

  RenderModels(1);

  shadow_map.Finalise();

//  GLCall(glCullFace(GL_BACK));

  Shaders[1].Unbind();
}

void Visualiser::RenderPointShadows()
{
  if(PointLights.empty()) return;

  Shaders[2].Bind();

  FOR_EACH(point_light, PointLights)
  {
    Shaders[2].SetPointLightSpaceMatrices(point_light.GetLightSpaceMatrices());
    Shaders[2].SetPointPosition(point_light.GetPosition());
    Shaders[2].SetPointFarPlane(PointLight::GetFarPlane());

    const Shadow& shadow_map = point_light.GetShadowMap();

    GLCall(glViewport(0, 0, shadow_map.GetDepthMap().GetWidth(), shadow_map.GetDepthMap().GetHeight()));

    shadow_map.WriteTo();

    RenderModels(2);

    shadow_map.Finalise();
  }

  Shaders[2].Unbind();
}

void Visualiser::RenderModels(UInt _shader_index)
{
  static float x_incr(0.008);
  static float x_offs(0.0);
  static float x_sign(1.0);

  static float angl_incr(0.1);
  static float angl_offs(0.0);

  if(_shader_index == 1)
  {
    x_offs += x_sign * x_incr;
    if(Abs(x_offs) > 2.0) x_sign *= -1.0;

    angl_offs += angl_incr;
    if(angl_offs > 360.0) angl_offs -= 360.0;
  }

  // Cube model
  Models[0].Reset();
  Models[0].Translate({x_offs, 0.0f, 0.0f});
  Models[0].Rotate(angl_offs, {0.0f, 0.0f, 1.0f});

  Shaders[_shader_index].UseModel(Models[0]);
  Shaders[_shader_index].UseMaterial(Materials[0]);
//    shader_storage_buffer.BindBase();
//    GLsizei N2 = (GLsizei)varray.size() - 2;
//    glDrawArrays(GL_TRIANGLES, 0, 6*(N2 - 1));
  Models[0].Draw();

  // Floor model
  Models[1].Reset();
  Models[1].Translate({0.0f, -2.0f, 0.0f});
  Models[1].Rotate(90.0, {1.0f, 0.0f, 0.0f});
  Shaders[_shader_index].UseModel(Models[1]);
  Shaders[_shader_index].UseMaterial(Materials[0]);
//  Shaders[0].UseTexture(Textures[0], 0);
  Models[1].Draw();
//  Textures[0].Unbind();
}

void Visualiser::EndFrame()
{
  OpenGLWindow.SwapBuffers();
  glfwPollEvents();
}

}

