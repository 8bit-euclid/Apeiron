#include "../include/Global.h"
#include "../libs/Visualiser/include/Visualiser.h"

using namespace Apeiron;

int main(void)
{
  Visualiser visualiser;
  visualiser.OpenWindow(1920, 1080);

  // Tetrahedron model
  visualiser.Models.resize(1);
  visualiser.Models[0].Geometry.Shading = ShadingType::Phong;
  DynamicArray<Vertex>& vertices0 = visualiser.Models[0].Geometry.Vertices;
  DynamicArray<GLuint>& indices0 = visualiser.Models[0].Geometry.Indices;

  vertices0.resize(4);
  vertices0[0].Position = glm::vec3(0.0f, 1.0f, 1.0f);
  vertices0[1].Position = glm::vec3(1.0f, 1.0f, 0.0f);
  vertices0[2].Position = glm::vec3(0.0f, 1.0f, 0.0f);
  vertices0[3].Position = glm::vec3(0.0f, 2.0f, 0.0f);

  indices0.resize(12);
  indices0[0] = 0;
  indices0[1] = 1;
  indices0[2] = 2;

  indices0[3] = 0;
  indices0[4] = 1;
  indices0[5] = 3;

  indices0[6] = 0;
  indices0[7] = 2;
  indices0[8] = 3;

  indices0[9] = 1;
  indices0[10] = 2;
  indices0[11] = 3;

  visualiser.Models[0].Load();

//  // Floor model
  visualiser.Models.resize(2);
  visualiser.Models[1].Geometry.Shading = ShadingType::Flat;
  DynamicArray<Vertex>& vertices1 = visualiser.Models[1].Geometry.Vertices;
  DynamicArray<GLuint>& indices1 = visualiser.Models[1].Geometry.Indices;

  vertices1.resize(4);
  vertices1[0].Position = glm::vec3(-10.0f, 0.0f, -10.0f);
  vertices1[1].Position = glm::vec3(10.0f, 0.0f, -10.0f);
  vertices1[2].Position = glm::vec3(-10.0f, 0.0f, 10.0f);
  vertices1[3].Position = glm::vec3(10.0f, 0.0f, 10.0f);

  vertices1[0].Normal = glm::vec3(0.0f, -1.0f, 0.0f);
  vertices1[1].Normal = glm::vec3(0.0f, -1.0f, 0.0f);
  vertices1[2].Normal = glm::vec3(0.0f, -1.0f, 0.0f);
  vertices1[3].Normal = glm::vec3(0.0f, -1.0f, 0.0f);

  vertices1[0].TextureCoordinates = glm::vec2(0.0f, 0.0f);
  vertices1[1].TextureCoordinates = glm::vec2(1.0f, 0.0f);
  vertices1[2].TextureCoordinates = glm::vec2(0.0f, 1.0f);
  vertices1[3].TextureCoordinates = glm::vec2(1.0f, 1.0f);

  indices1.resize(6);
  indices1[0] = 0;
  indices1[1] = 2;
  indices1[2] = 1;

  indices1[3] = 1;
  indices1[4] = 2;
  indices1[5] = 3;

  visualiser.Models[1].Load();

  // Shaders
  visualiser.Shaders.emplace_back("libs/Visualiser/resources/shaders/Basic.glsl");
//  visualiser.Shaders.emplace_back("libs/Visualiser/resources/shaders/Line.glsl");
  visualiser.Shaders[0].Bind();
  visualiser.Shaders[0].SetUniform1f("u_thickness", 20.0);

  visualiser.Shaders[0].SetUniform4f("u_colour", 1.0f, 1.0f, 1.0f, 1.0f);

//  DynamicArray<glm::vec4> varray;
//  varray.emplace_back(glm::vec4(-0.6f, -0.6f, 0.0f, 1.0f));
//  varray.emplace_back(glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f));
//  varray.emplace_back(glm::vec4(0.5f, 0.5f, 0.0f, 1.0f));
//  varray.emplace_back(glm::vec4(0.6f, 0.6f, 0.0f, 1.0f));
//  ShaderStorageBuffer shader_storage_buffer(varray);
//
//  VertexArray vertex_array;
//  vertex_array.Bind();

  float x_incr(0.005);
  float x_offs(0.0);
  float x_sign(1.0);

  float angl_incr(0.1);
  float angl_offs(0.0);

  visualiser.Textures.emplace_back("libs/Visualiser/resources/textures/Papyrus.png");
  visualiser.Materials.emplace_back(0.7, 128.0);
//  visualiser.Materials.emplace_back(1.0, 32.0);

  visualiser.DirectionalLights.emplace_back(glm::vec3(0.0, 0.0, -1.0), glm::vec4(1.0, 1.0, 1.0, 1.0), 0.0, 0.0);

  visualiser.PointLights.emplace_back(glm::vec3(0.0, 0.0, 0.0), glm::vec4(0.0, 0.0, 1.0, 1.0), 0.0, 1.0, StaticArray<GLfloat, 3>{0.3, 0.2, 0.1});
  visualiser.PointLights.emplace_back(glm::vec3(-4.0, 2.0, 0.0), glm::vec4(0.0, 1.0, 0.0, 1.0), 0.0, 1.0, StaticArray<GLfloat, 3>{0.3, 0.1, 0.1});

  visualiser.Cameras.resize(1);
  visualiser.Cameras[0].SetOrientation(glm::vec3(0.0f, 0.0f, 1.0f), 0.0, 90.0);
  visualiser.Cameras[0].SetViewFrustum(visualiser.ViewportAspectRatio(), 45.0, 1.0, -100.0);

//  auto [width, height] = visualiser.ViewportDimensions();
//  visualiser.Shaders[0].SetUniform2f("u_resolution", (float)width, (float)height);

  while(visualiser.isWindowOpen())
  {
    visualiser.BeginFrame();

    visualiser.Cameras[0].KeyControl(visualiser.GetKeys(), visualiser.GetDeltaTime());
    visualiser.Cameras[0].MouseControl(visualiser.GetMouseDisplacement());

    // If the viewport has been modified, update the view frustum
    if(visualiser.isViewPortModified)
    {
      auto [width, height] = visualiser.ViewportDimensions();
      visualiser.Cameras[0].SetViewFrustum(visualiser.ViewportAspectRatio());
      visualiser.Shaders[0].SetUniform2f("u_resolution", (float)width, (float)height);
    }

    glm::vec3 camera_position = visualiser.Cameras[0].GetPosition();
    visualiser.Shaders[0].SetUniform3f("u_camera_position", camera_position.x, camera_position.y, camera_position.z);

    x_offs += x_sign * x_incr;
    if(Abs(x_offs) > 1.0) x_sign *= -1.0;

    angl_offs += angl_incr;
    if(angl_offs > 360.0) angl_offs -= 360.0;

    // Tetrahedron model
    glm::mat4 _model_matrix(1.0);
    _model_matrix = glm::translate(_model_matrix, glm::vec3(x_offs, 0.0f, 0.0f));
    _model_matrix = glm::rotate(_model_matrix, (float)ToRadians(angl_offs), glm::vec3(0.0f, 0.0f, 1.0f));
//    _model_matrix = glm::scale(_model_matrix, glm::vec3(2.0f, 2.0f, 1.0f));

    visualiser.Shaders[0].UseMaterial(visualiser.Materials[0]);
    visualiser.Shaders[0].UseLight(visualiser.DirectionalLights[0]);
    visualiser.Shaders[0].UseLight(visualiser.PointLights[0]);
    visualiser.Shaders[0].UseLight(visualiser.PointLights[1]);

    glm::mat4 _view_matrix(1.0), _projection_matrix(1.0);
    visualiser.Cameras[0].ComputeViewMatrix(_view_matrix);
    visualiser.Cameras[0].ComputeProjectionMatrix(_projection_matrix);

    visualiser.Shaders[0].SetUniformMatrix4f("u_model_matrix", _model_matrix);
    visualiser.Shaders[0].SetUniformMatrix4f("u_view_matrix", _view_matrix);
    visualiser.Shaders[0].SetUniformMatrix4f("u_projection_matrix", _projection_matrix);
//    shader_storage_buffer.BindBase();
//    GLsizei N2 = (GLsizei)varray.size() - 2;
//    glDrawArrays(GL_TRIANGLES, 0, 6*(N2 - 1));

    visualiser.Models[0].Draw();

    // Floor model
    _model_matrix = glm::mat4(1.0);
    _model_matrix = glm::translate(_model_matrix, glm::vec3(0.0f, -2.0f, 0.0f));
    visualiser.Shaders[0].SetUniformMatrix4f("u_model_matrix", _model_matrix);
    visualiser.Textures[0].Bind();
    visualiser.Shaders[0].UseMaterial(visualiser.Materials[0]);
    visualiser.Models[1].Draw();




    visualiser.EndFrame();
  }

  return 0;
}


//int main(void)
//{
//  float positions[] = {
//    -50.0f, -50.0f, 0.0f, 0.0f,
//    50.0f, -50.0f, 1.0f, 0.0f,
//    50.0f,  50.0f, 1.0f, 1.0f,
//    -50.0f,  50.0f, 0.0f, 1.0f
//  };
//
//  unsigned int indices[] = {
//    0, 1, 2,
//    2, 3, 0
//  };
//
//  GLCall(glEnable(GL_BLEND));
//  GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
//
//  VertexArray vertex_array;
//  VertexBuffer vertex_buffer(positions, 4 * 4 * sizeof(float));
//
//  VertexBufferLayout layout;
//  layout.Push<float>(2);
//  layout.Push<float>(2);
//  vertex_array.AddBuffer(vertex_buffer, layout);
//
//  IndexBuffer index_buffer(indices, 6);
//
//  glm::mat4 proj_matrix = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f);
//  glm::mat4 view_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
//
//  Shader shader("libs/Visualiser/resources/shaders/Basic.glsl");
//  shader.Bind();
//  shader.SetUniform4f("u_color", 0.8f, 0.3f, 0.8f, 1.0f);
//
//  Texture texture("libs/Visualiser/resources/textures/ChernoLogo.png");
//  const UInt _slot = 0;
//  texture.Bind(_slot);
//  shader.SetUniform1i("u_texture", _slot);
//
//  vertex_array.Unbind();
//  shader.Unbind();
//  vertex_buffer.Unbind();
//  index_buffer.Unbind();
//
//  Renderer renderer;
//
//  ImGui::CreateContext();
//  ImGuiIO& io = ImGui::GetIO();
//  (void)io;
//  ImGui::StyleColorsDark();
//  ImGui_ImplGlfw_InitForOpenGL(window, true);
//  ImGui_ImplOpenGL3_Init("#version 460");
//
//  glm::vec3 translationA(200, 200, 0);
//  glm::vec3 translationB(400, 200, 0);
//
//  float red = 0.0f;
//  float increment = 0.05f;
//
//  // Loop until the user closes the window
//  while(!glfwWindowShouldClose(window))
//  {
//    renderer.Clear();
//
//    ImGui_ImplOpenGL3_NewFrame();
//    ImGui_ImplGlfw_NewFrame();
//    ImGui::NewFrame();
//
//    // Bind shader and setup uniforms.
//    shader.Bind();
////    shader.SetUniform4f("u_color", red, 0.3, 0.8f, 1.0f);
//
//    {
//      glm::mat4 model_matrix = glm::translate(glm::mat4(1.0f), translationA);
//      glm::mat4 mvp_matrix = proj_matrix * view_matrix * model_matrix;
//      shader.SetUniformMatrix4f("u_mvp_matrix", mvp_matrix);
//      renderer.Draw(vertex_array, index_buffer, shader);
//    }
//
//    {
//      glm::mat4 model_matrix = glm::translate(glm::mat4(1.0f), translationB);
//      glm::mat4 mvp_matrix = proj_matrix * view_matrix * model_matrix;
//      shader.SetUniformMatrix4f("u_mvp_matrix", mvp_matrix);
//      renderer.Draw(vertex_array, index_buffer, shader);
//    }
//
//    ImGui::Begin("ImGui Window.");
//    ImGui::SliderFloat3("Translation A", &translationA.x, 0.0f, 960.0f);
//    ImGui::SliderFloat3("Translation B", &translationB.x, 0.0f, 960.0f);
//    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
//    ImGui::End();
//
//    ImGui::Render();
//    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//
//    if(red > 1.0f) increment = -0.05f;
//    else if(red < 0.0f) increment = 0.05f;
//
//    red += increment;
//
//    // Swap front and back buffers
//    glfwSwapBuffers(window);
//
//    // Poll for and process events
//    glfwPollEvents();
//  }
//
//  ImGui_ImplOpenGL3_Shutdown();
//  ImGui_ImplGlfw_Shutdown();
//  ImGui::DestroyContext();
//
//  glfwTerminate();
//  return 0;
//}




//int main(void)
//{
//  Visualiser visualiser;
//  visualiser.OpenWindow(800, 600);
//
//  visualiser.Shaders.emplace_back("libs/Visualiser/resources/shaders/Line.glsl");
//  visualiser.Shaders[0].Bind();
//  visualiser.Shaders[0].SetUniform1f("u_thickness", 20.0);
//
//  DynamicArray<glm::vec4> varray;
//  varray.emplace_back(glm::vec4(-0.6f, -0.6f, 0.0f, 1.0f));
//  varray.emplace_back(glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f));
//  varray.emplace_back(glm::vec4(0.5f, 0.5f, 0.0f, 1.0f));
//  varray.emplace_back(glm::vec4(0.6f, 0.6f, 0.0f, 1.0f));
//  ShaderStorageBuffer shader_storage_buffer(varray);
//
//  VertexArray vertex_array;
//  vertex_array.Bind();
//
////  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//
//  float x_incr(0.005);
//  float x_offs(0.0);
//  float x_sign(1.0);
//
//  float angl_incr(0.1);
//  float angl_offs(0.0);
//
//  visualiser.Cameras.resize(1);
//  visualiser.Cameras[0].SetOrientation(glm::vec3(0.0f, 0.0f, 1.0f), 0.0, 90.0);
//  visualiser.Cameras[0].SetViewFrustum(visualiser.ViewportAspectRatio(), 45.0, 1.0, -100.0);
//
//  auto [width, height] = visualiser.ViewportDimensions();
//  visualiser.Shaders[0].SetUniform2f("u_resolution", (float)width, (float)height);
//
//  while(visualiser.isWindowOpen())
//  {
//    visualiser.BeginFrame();
//
//    visualiser.Cameras[0].KeyControl(visualiser.GetKeys(), visualiser.GetDeltaTime());
//    visualiser.Cameras[0].MouseControl(visualiser.GetMouseDisplacement());
//
//    // If the viewport has been modified, update the view frustum
//    if(visualiser.isViewPortModified)
//    {
//      auto [width, height] = visualiser.ViewportDimensions();
//      visualiser.Cameras[0].SetViewFrustum(visualiser.ViewportAspectRatio());
//      visualiser.Shaders[0].SetUniform2f("u_resolution", (float)width, (float)height);
//    }
//
//    x_offs += x_sign * x_incr;
//    if(Abs(x_offs) > 1.0) x_sign *= -1.0;
//
//    angl_offs += angl_incr;
//    if(angl_offs > 360.0) angl_offs -= 360.0;
//
//    glm::mat4 _model_matrix(1.0);
//    _model_matrix = glm::translate(_model_matrix, glm::vec3(x_offs, 0.0f, 0.0f));
//    _model_matrix = glm::rotate(_model_matrix, (float)ToRadians(angl_offs), glm::vec3(0.0f, 0.0f, 1.0f));
////    _model_matrix = glm::scale(_model_matrix, glm::vec3(2.0f, 2.0f, 1.0f));
//
//    glm::mat4 _view_matrix(1.0), _projection_matrix(1.0);
//    visualiser.Cameras[0].ComputeViewMatrix(_view_matrix);
//    visualiser.Cameras[0].ComputeProjectionMatrix(_projection_matrix);
//
//    visualiser.Shaders[0].SetUniformMatrix4f("u_model_matrix", _model_matrix);
//    visualiser.Shaders[0].SetUniformMatrix4f("u_view_matrix", _view_matrix);
//    visualiser.Shaders[0].SetUniformMatrix4f("u_projection_matrix", _projection_matrix);
//    shader_storage_buffer.BindBase();
//    GLsizei N2 = (GLsizei)varray.size() - 2;
//    glDrawArrays(GL_TRIANGLES, 0, 6*(N2 - 1));
//
//    visualiser.EndFrame();
//  }
//
//  return 0;
//}