#include "../include/Global.h"
#include "../libs/Visualiser/include/Visualiser.h"

using namespace Apeiron;

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




int main(void)
{
  Visualiser visualiser;
  visualiser.OpenWindow(800, 600);

  visualiser.Shaders.emplace_back("libs/Visualiser/resources/shaders/Line.glsl");
  visualiser.Shaders[0].Bind();
  visualiser.Shaders[0].SetUniform1f("u_thickness", 20.0);

  DynamicArray<glm::vec4> varray;
  varray.emplace_back(glm::vec4(-0.6f, -0.6f, 0.0f, 1.0f));
  varray.emplace_back(glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f));
  varray.emplace_back(glm::vec4(0.5f, 0.5f, 0.0f, 1.0f));
  varray.emplace_back(glm::vec4(0.6f, 0.6f, 0.0f, 1.0f));
  ShaderStorageBuffer shader_storage_buffer(varray);

  VertexArray vertex_array;
  vertex_array.Bind();

//  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  float x_incr(0.005);
  float x_offs(0.0);
  float x_sign(1.0);

  float angl_incr(0.1);
  float angl_offs(0.0);

  visualiser.Cameras.resize(1);
  visualiser.Cameras[0].SetOrientation(glm::vec3(0.0f, 0.0f, 1.0f), 0.0, 90.0);
  visualiser.Cameras[0].SetViewFrustum(visualiser.ViewportAspectRatio(), 45.0, 1.0, -100.0);

  auto [width, height] = visualiser.ViewportDimensions();
  visualiser.Shaders[0].SetUniform2f("u_resolution", (float)width, (float)height);

  while(visualiser.isWindowOpen())
  {
    visualiser.BeginFrame();

    visualiser.Cameras[0].KeyControl(visualiser.GetKeys());

    // If the viewport has been modified, update the view frustum
    if(visualiser.isViewPortModified)
    {
      auto [width, height] = visualiser.ViewportDimensions();
      visualiser.Cameras[0].SetViewFrustum(visualiser.ViewportAspectRatio());
      visualiser.Shaders[0].SetUniform2f("u_resolution", (float)width, (float)height);
    }

    x_offs += x_sign * x_incr;
    if(Abs(x_offs) > 1.0) x_sign *= -1.0;

    angl_offs += angl_incr;
    if(angl_offs > 360.0) angl_offs -= 360.0;

    glm::mat4 _model_matrix(1.0);
    _model_matrix = glm::translate(_model_matrix, glm::vec3(x_offs, 0.0f, 0.0f));
    _model_matrix = glm::rotate(_model_matrix, (float)ToRadians(angl_offs), glm::vec3(0.0f, 0.0f, 1.0f));
//    _model_matrix = glm::scale(_model_matrix, glm::vec3(2.0f, 2.0f, 1.0f));

    glm::mat4 _view_matrix(1.0), _projection_matrix(1.0);
    visualiser.Cameras[0].ComputeViewMatrix(_view_matrix);
    visualiser.Cameras[0].ComputeProjectionMatrix(_projection_matrix);

    visualiser.Shaders[0].SetUniformMatrix4f("u_model_matrix", _model_matrix);
    visualiser.Shaders[0].SetUniformMatrix4f("u_view_matrix", _view_matrix);
    visualiser.Shaders[0].SetUniformMatrix4f("u_projection_matrix", _projection_matrix);
    shader_storage_buffer.BindBase();
    GLsizei N2 = (GLsizei)varray.size() - 2;
    glDrawArrays(GL_TRIANGLES, 0, 6*(N2 - 1));

    visualiser.EndFrame();
  }

  return 0;
}






//int main(void)
//{
//  if(!glfwInit()) return -1;
//
//  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
//  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
//  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//  glfwWindowHint(GLFW_SAMPLES, 16);
//  glEnable(GL_MULTISAMPLE);
//
//  GLFWwindow *window = glfwCreateWindow(800, 600, "GLFW OGL window", nullptr, nullptr);
//  if(!window)
//  {
//    glfwTerminate();
//    return 0;
//  }
//  glfwMakeContextCurrent(window);
//  ASSERT(glewInit() == GLEW_OK, "Failed to Initialise OpenGL.")
//
//  Shader shader("libs/Visualiser/resources/shaders/Line.glsl");
//  shader.Bind();
//  shader.SetUniform1f("u_thickness", 1.0);
//
//  GLushort pattern = 0x18ff;
//  GLfloat factor = 2.0f;
//
//  glm::vec4 p0(-1.0f, -1.0f, 0.0f, 1.0f);
//  glm::vec4 p1(1.0f, -1.0f, 0.0f, 1.0f);
//  glm::vec4 p2(1.0f, 1.0f, 0.0f, 1.0f);
//  glm::vec4 p3(-1.0f, 1.0f, 0.0f, 1.0f);
//  std::vector<glm::vec4> varray1{p3, p0, p1, p2, p3, p0, p1};
//  GLuint ssbo1 = CreateSSBO(varray1);
//
////  std::vector<glm::vec4> varray2;
////  varray2.emplace_back(glm::vec4(-1.0f, -1.0f, 0.0f, 1.0f));
////  varray2.emplace_back(glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f));
////  varray2.emplace_back(glm::vec4(0.5f, 0.5f, 0.0f, 1.0f));
////  varray2.emplace_back(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
//  std::vector<glm::vec4> varray2;
//  for(int u=-2; u <= 362; u += 2)
//  {
//    double a = u*Pi/180.0;
//    double c = cos(a);
//    double s = sin(a);
//    varray2.emplace_back(glm::vec4((float)c, (float)s, 0.0f, 1.0f));
//  }
//  GLuint ssbo2 = CreateSSBO(varray2);
//
//  VertexArray vertex_array;
//  vertex_array.Bind();
//
//  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
//  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//
//  glm::mat4 project;
//  int vpSize[2]{0, 0};
//  while(!glfwWindowShouldClose(window))
//  {
//    int w, h;
//    glfwGetFramebufferSize(window, &w, &h);
//    if(w != vpSize[0] ||  h != vpSize[1])
//    {
//      vpSize[0] = w;
//      vpSize[1] = h;
//      glViewport(0, 0, vpSize[0], vpSize[1]);
//      float aspect = (float)w/(float)h;
//      project = glm::ortho(-aspect, aspect, -1.0f, 1.0f, -10.0f, 10.0f);
//      shader.SetUniform2f("u_resolution", (float)w, (float)h);
//    }
//
//    glClear(GL_COLOR_BUFFER_BIT);
//
//    glm::mat4 modelview1(1.0f);
//    modelview1 = glm::translate(modelview1, glm::vec3(-0.6f, 0.0f, 0.0f));
//    modelview1 = glm::scale(modelview1, glm::vec3(0.5f, 0.5f, 1.0f));
//    glm::mat4 mvp1 = project * modelview1;
//
//    shader.SetUniformMatrix4f("u_mvp", mvp1);
//    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo1);
//    GLsizei N1 = (GLsizei)varray1.size() - 2;
//    glDrawArrays(GL_TRIANGLES, 0, 6*(N1 - 1));
//
//    glm::mat4 modelview2(1.0f);
//    modelview2 = glm::translate(modelview2, glm::vec3(0.6f, 0.0f, 0.0f));
//    modelview2 = glm::scale(modelview2, glm::vec3(0.5f, 0.5f, 1.0f));
//    glm::mat4 mvp2 = project * modelview2;
//
//    shader.SetUniformMatrix4f("u_mvp", mvp2);
//    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo2);
//    GLsizei N2 = (GLsizei)varray2.size() - 2;
//    glDrawArrays(GL_TRIANGLES, 0, 6*(N2 - 1));
//
//    glfwSwapBuffers(window);
//    glfwPollEvents();
//  }
//  glfwTerminate();
//
//  return 0;
//}






//int main(void)
//{
//  GLFWwindow* window;
//
//  /* Initialize the library */
//  if (!glfwInit())
//    return -1;
//
//  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
//  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
//  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//  /* Create a windowed mode window and its OpenGL context */
//  window = glfwCreateWindow(640, 480, "Hello World", nullptr, nullptr);
//  if(!window)
//  {
//    glfwTerminate();
//    return -1;
//  }
//
//  /* Make the window's context current */
//  glfwMakeContextCurrent(window);
//  glfwSwapInterval(1);
//
//  ASSERT(glewInit() == GLEW_OK, "Failed to Initialise OpenGL.")
//  Print("\nOpenGL Version:", glGetString(GL_VERSION));
//
//  float positions[] = {
//         -0.5f,  -0.5f,
//          0.5f,   0.5f,
//          0.49f,  0.51f,
//         -0.51f, -0.49f
//  };
//
//  unsigned int indices[] = {
//          0, 1, 2,
//          2, 3, 0
//  };
//
//  VertexArray vertex_array;
//  VertexBuffer vertex_buffer(positions, 4 * 2 * sizeof(float));
//
//  VertexBufferLayout layout;
//  layout.AddEntity<float>(2);
//  vertex_array.AddBuffer(vertex_buffer, layout);
//
//  IndexBuffer index_buffer(indices, 6);
//
//  Shader shader("libs/Visualiser/resources/shaders/Basic.glsl");
//  shader.Bind();
//  shader.SetUniform4f("u_color", 0.8f, 0.3f, 0.8f, 1.0f);
//
//  vertex_array.Unbind();
//  shader.Unbind();
//  vertex_buffer.Unbind();
//  index_buffer.Unbind();
//
//  // Loop until the user closes the window
//  Renderer renderer;
//  while(!glfwWindowShouldClose(window))
//  {
//    renderer.Clear();
//
//    // Bind shader and setup uniforms.
//    shader.Bind();
//    shader.SetUniform4f("u_color", 1.0f, 1.0f, 1.0f, 1.0f);
//
//    renderer.Draw(vertex_array, index_buffer, shader);
//
//
//
//    // Swap front and back buffers
//    glfwSwapBuffers(window);
//
//    // Poll for and process events
//    glfwPollEvents();
//  }
//
//  glfwTerminate();
//  return 0;
//}