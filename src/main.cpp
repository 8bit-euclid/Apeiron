#include "../include/Global.h"
#include "../libs/Visualiser/include/Visualiser.h"
#include "../libs/Visualiser/test/VisualiserTest.h"

using namespace Apeiron;

//#include "../libs/Visualiser/resources/external/stb_image/stb_image.h"
//
//#include <string>
//#include <iostream>
//#include <fstream>
//#include <sstream>
//#include <tuple>
//
//#include <GL/glew.h>
//#include <GLFW/glfw3.h>
//#include <glm/glm.hpp>
//#include <glm/gtc/type_ptr.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//
//using namespace Apeiron;

//// OpenGL function wrapper for error checking in debug mode.
//#define GLCall(_function) GLClearErrors(); _function; GLLogCall(#_function, __FILE__, __LINE__)
//
//// Clear all errors OpenGL errors.
//void GLClearErrors() { while(glGetError() != GL_NO_ERROR) {} }
//
//// Throw current OpenGL error and exit program.
//void GLLogCall(const char* _function, const char* _file, const int _line)
//{
//  if(GLenum error = glGetError())
//  {
//    std::cout << "OpenGL error (0x0" << std::hex << error << std::dec << ") from call:\n\t" << _function
//              << "\n\tFile: " << _file << "\n\tLine: " << _line;
//    exit(-1);
//  }
//}

void OpenWindow(GLFWwindow*& _p_window, GLint _width, GLint _height)
{
  if(!glfwInit()) { glfwTerminate(); exit(-1); }

  // Window properties
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Enforces backward incompatibility
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  // Create a window and its OpenGL context.
  _p_window = glfwCreateWindow(_width, _height, "Visualiser", nullptr, nullptr);
  if(!_p_window) { glfwTerminate(); exit(-1); }

  // Set context for GLEW to use
  glfwMakeContextCurrent(_p_window);
  glfwSwapInterval(1);

  // Allow modern extension features
  glewExperimental = GL_TRUE;

  // Initialise GLEW
  if(glewInit() != GLEW_OK) { glfwDestroyWindow(_p_window); glfwTerminate(); exit(-1); }

  GLCall(glEnable(GL_DEPTH_TEST));
  GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
}

//GLuint CompileShader(GLenum _type, const std::string& _source)
//{
//  GLCall(GLuint shader_ID = glCreateShader(_type));
//  const GLchar* src = _source.c_str();
//
//  GLCall(glShaderSource(shader_ID, 1, &src, nullptr));
//  GLCall(glCompileShader(shader_ID));
//
//  // Check that shader compiled
//  GLint result = 0;
//  GLchar error_log[1024] = { 0 };
//  GLCall(glGetShaderiv(shader_ID, GL_COMPILE_STATUS, &result));
//  if(!result) { GLCall(glGetShaderInfoLog(shader_ID, sizeof(error_log), nullptr, error_log)); exit(-1); }
//
//  return shader_ID;
//}
//
//void AttachShader(GLuint _program, GLuint _shader) { GLCall(glAttachShader(_program, _shader)); }
//
//GLuint CreateShaderProgram()
//{
//  GLCall(GLuint shader = glCreateProgram());
//  if(!shader) { glfwTerminate(); exit(-1); }
//
//  std::ifstream stream("libs/Visualiser/resources/shaders/Minimal.glsl");
//  std::string line;
//  std::stringstream string_stream[2];
//  unsigned index;
//  while(getline(stream, line))
//  {
//    if(line.find("#shader") != std::string::npos)
//    {
//      if(line.find("vertex") != std::string::npos) index = 0;
//      else if(line.find("fragment") != std::string::npos) index = 1;
//    }
//    else string_stream[index] << line << '\n';
//  }
//
//  std::string vs_source(string_stream[0].str()), fs_source(string_stream[1].str());
//
//  // Create and attach vertex shader.
//  GLuint vs_id = CompileShader(GL_VERTEX_SHADER, vs_source);
//  AttachShader(shader, vs_id);
//
//  // Create and attach fragment shader.
//  GLuint fs_id = CompileShader(GL_FRAGMENT_SHADER, fs_source);
//  AttachShader(shader, fs_id);
//
//  // Link shader program
//  GLint result = 0;
//  GLchar error_log[1024] = {0};
//  GLCall(glLinkProgram(shader));
//  GLCall(glGetProgramiv(shader, GL_LINK_STATUS, &result));
//  if(!result) { GLCall(glGetProgramInfoLog(shader, sizeof(error_log), nullptr, error_log)); exit(-1); }
//
//  // Validate shader program
//  GLCall(glValidateProgram(shader));
//  GLCall(glGetProgramiv(shader, GL_VALIDATE_STATUS, &result));
//  if(!result) { GLCall(glGetProgramInfoLog(shader, sizeof(error_log), nullptr, error_log)); exit(-1); }
//
//  // Delete shaders
//  GLCall(glDeleteShader(vs_id));
//  GLCall(glDeleteShader(fs_id));
//
//  return shader;
//}
//
//std::pair<GLuint, GLuint> CreateSquare()
//{
//  GLfloat vertices[] = { -0.5f, -0.5f,  0.0f,       0.0f, 0.0f,
//                          0.5f, -0.5f,  0.0f,       1.0f, 0.0f,
//                          0.5f,  0.5f,  0.0f,       1.0f, 1.0f,
//                         -0.5f,  0.5f,  0.0f,       0.0f, 1.0f };
//
//  GLuint indices[] = { 0, 1, 3,
//                       1, 2, 3 };
//
//  GLuint VAO, VBO, EBO;
//
//  // Vertex array
//  GLCall(glGenVertexArrays(1, &VAO));
//  GLCall(glBindVertexArray(VAO));
//
//  // Vertex buffer
//  GLCall(glGenBuffers(1, &VBO));
//  GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
//  GLCall(glBufferData(GL_ARRAY_BUFFER, 4 * 5 * sizeof(GLfloat), vertices, GL_STATIC_DRAW));
//  GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, false, 5 * sizeof(GLfloat), reinterpret_cast<void*>(0)));
//  GLCall(glEnableVertexAttribArray(0));
//  GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, false, 5 * sizeof(GLfloat), reinterpret_cast<void*>(3 * sizeof(GLfloat))));
//  GLCall(glEnableVertexAttribArray(1));
//  GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
//
//  // Index buffer
//  GLCall(glGenBuffers(1, &EBO));
//  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
//  GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), indices, GL_STATIC_DRAW));
//  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
//
//  GLCall(glBindVertexArray(0));
//
//  return {VAO, EBO};
//}
//
//GLuint CreateTexture()
//{
//  GLuint texture;
//  GLCall(glGenTextures(1, &texture));
//
//  std::string file_path = "libs/Visualiser/resources/textures/ChernoLogo.png";
//  UChar* local_buffer;
//  int bits_per_pixel;
//
//  stbi_set_flip_vertically_on_load(1);
//  int width, height;
//  local_buffer = stbi_load(file_path.c_str(), &width, &height, &bits_per_pixel, 0);
//
//  ASSERT(local_buffer, "Could not load file \"", file_path, "\" to texture.")
//
//  GLCall(glBindTexture(GL_TEXTURE_2D, texture));
//
//  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
//  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
//  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
//  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
//
//  GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, local_buffer));
//
//  GLCall(glGenerateMipmap(GL_TEXTURE_2D));
//
//  GLCall(glBindTexture(GL_TEXTURE_2D, 0));
//
//  stbi_image_free(local_buffer);
//
//  return texture;
//}
//
//int main(void)
//{
//  GLFWwindow* p_window;
//  GLint width(1920), height(1080);
//  OpenWindow(p_window, width, height);
//  GLfloat aspect_ratio = (GLfloat)width / (GLfloat)height;
//
//  auto [VAO, EBO] = CreateSquare();
//
//  GLuint texture = CreateTexture();
//
//  GLuint shader = CreateShaderProgram();
//
//  GLCall(glUseProgram(shader));
//  GLint u_colour = glGetUniformLocation(shader, "u_colour");
//  GLCall(glUniform4f(u_colour, 0.0f, 0.0f, 1.0f, 1.0f));
//
//  GLCall(glEnable(GL_BLEND));
//  GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
//
//  while(!glfwWindowShouldClose(p_window))
//  {
//    GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
//    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
//
//    glm::mat4 proj_matrix = glm::ortho(-aspect_ratio, aspect_ratio, -1.0f, 1.0f, -10.0f, 10.0f);
////    glm::mat4 view_matrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
////    glm::mat4 model_matrix = glm::translate(glm::mat4(1.0f), {0.0f, 0.0f, -20.0f});
////
//    GLCall(glUniformMatrix4fv(glGetUniformLocation(shader, "u_projection_matrix"), 1, GL_FALSE, glm::value_ptr(proj_matrix)));
////    GLCall(glUniformMatrix4fv(glGetUniformLocation(shader, "u_view_matrix"), 1, GL_FALSE, glm::value_ptr(view_matrix)));
////    GLCall(glUniformMatrix4fv(glGetUniformLocation(shader, "u_model_matrix"), 1, GL_FALSE, glm::value_ptr(model_matrix)));
//
//    GLint u_texture = glGetUniformLocation(shader, "u_texture");
////    glEnable(GL_TEXTURE_2D);
//    GLCall(glUniform1i(u_texture, 0));
//    GLCall(glActiveTexture(GL_TEXTURE0));
//    GLCall(glBindTexture(GL_TEXTURE_2D, texture));
//
//    // Draw square
//    GLCall(glBindVertexArray(VAO));
//    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
//    GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
//    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
//    GLCall(glBindVertexArray(0));
//
//    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
//
//    glfwSwapBuffers(p_window);
//    glfwPollEvents();
//  }
//}






int main(void)
{
  Visualiser visualiser;

  // Floor model
  visualiser.Models.resize(1);
  CreateSquare(visualiser.Models[0], 1.0f);

  visualiser.Models[0].Reset();

  visualiser.Shaders.emplace_back("libs/Visualiser/resources/shaders/Minimal.glsl");
  visualiser.Shaders[0].Bind();
  visualiser.Shaders[0].SetUniform4f("u_colour", 1.0f, 0.0f, 0.0f, 1.0f);

//  visualiser.Textures.emplace_back("libs/Visualiser/resources/textures/Cloth.jpg");
  visualiser.Textures.emplace_back("libs/Visualiser/resources/textures/Papyrus.png");
//  visualiser.Textures.emplace_back("libs/Visualiser/resources/textures/YouTubeLogo.png");
//  visualiser.Textures.emplace_back("libs/Visualiser/resources/textures/ChernoLogo.png");

  while(visualiser.isWindowOpen())
  {
    visualiser.BeginFrame();

    visualiser.Shaders[0].SetUniformMatrix4f("u_projection_matrix",
                                             glm::ortho(-visualiser.ViewportAspectRatio(), visualiser.ViewportAspectRatio(), -1.0f, 1.0f, -10.0f, 10.0f));
    visualiser.Shaders[0].UseModel(visualiser.Models[0]);

    visualiser.Shaders[0].UseTexture(visualiser.Textures[0], 0);
    visualiser.Models[0].Draw();
    visualiser.Textures[0].Unbind();

    visualiser.EndFrame();
  }

  return 0;
}





//int main(void)
//{
//  Visualiser visualiser;
//
//  // Tetrahedron model
//  visualiser.Models.resize(1);
////  CreateTetrahedron(visualiser.Models[0], 1.0f);
//  CreateCube(visualiser.Models[0], 1.0f);
//
//  // Floor model
//  visualiser.Models.resize(2);
//  CreateSquare(visualiser.Models[1], 10.0f);
//  visualiser.Models[1].Geometry.Vertices[0].TextureCoordinates = glm::vec2(0.0f, 0.0f);
//  visualiser.Models[1].Geometry.Vertices[1].TextureCoordinates = glm::vec2(1.0f, 0.0f);
//  visualiser.Models[1].Geometry.Vertices[2].TextureCoordinates = glm::vec2(1.0f, 1.0f);
//  visualiser.Models[1].Geometry.Vertices[3].TextureCoordinates = glm::vec2(0.0f, 1.0f);
//
//  // Shaders
//  visualiser.Shaders.emplace_back("libs/Visualiser/resources/shaders/Minimal.glsl");
////  visualiser.Shaders.emplace_back("libs/Visualiser/resources/shaders/Basic.glsl");
////  visualiser.Shaders.emplace_back("libs/Visualiser/resources/shaders/Intermediate.glsl");
////  visualiser.Shaders.emplace_back("libs/Visualiser/resources/shaders/Advanced.glsl");
////  visualiser.Shaders.emplace_back("libs/Visualiser/resources/shaders/Shadow.glsl");
////  visualiser.Shaders.emplace_back("libs/Visualiser/resources/shaders/Line.glsl");
//
//  visualiser.Shaders[0].Bind();
////  visualiser.Shaders[0].SetUniform1f("u_thickness", 20.0);
//  visualiser.Shaders[0].SetUniform4f("u_colour", 1.0f, 1.0f, 1.0f, 1.0f);
//
////  DynamicArray<glm::vec4> varray;
////  varray.emplace_back(glm::vec4(-0.6f, -0.6f, 0.0f, 1.0f));
////  varray.emplace_back(glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f));
////  varray.emplace_back(glm::vec4(0.5f, 0.5f, 0.0f, 1.0f));
////  varray.emplace_back(glm::vec4(0.6f, 0.6f, 0.0f, 1.0f));
////  ShaderStorageBuffer shader_storage_buffer(varray);
////
////  VertexArray vertex_array;
////  vertex_array.Bind();
//
//
//  visualiser.Materials.emplace_back(0.3, 256.0);
////  visualiser.Materials.emplace_back(0.2, 32.0);
//
//  visualiser.Textures.emplace_back("libs/Visualiser/resources/textures/ChernoLogo.png");
////  visualiser.Textures.emplace_back("libs/Visualiser/resources/textures/Papyrus.png");
////  visualiser.Textures.emplace_back("libs/Visualiser/resources/textures/Cloth.jpg");
//  visualiser.Textures[0].Bind();
//  visualiser.Shaders[0].UseTexture(visualiser.Textures[0], 0);
//  visualiser.Textures[0].Unbind();
//
//  GLCall(glEnable(GL_BLEND));
//  GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
//
////  visualiser.PointLights.emplace_back(glm::vec3(0.0, 2.0, 0.0), glm::vec4(1.0, 1.0, 1.0, 1.0), 0.7, 0.5, StaticArray<GLfloat, 3>{0.3, 0.2, 0.1});
////  visualiser.PointLights.emplace_back(glm::vec3(-4.0, 2.0, 0.0), glm::vec4(0.0, 1.0, 0.0, 1.0), 0.5, 0.3, StaticArray<GLfloat, 3>{0.3, 0.1, 0.1});
//
////  visualiser.SpotLights.emplace_back(glm::vec3(0.0, 3.0, 0.0), glm::vec3(0.0, -1.0, 0.0), glm::vec4(1.0, 1.0, 1.0, 1.0), 20.0, 1.0, 0.7, StaticArray<GLfloat, 3>{1.0, 0.0, 0.0});
////  visualiser.SpotLights.emplace_back(glm::vec3(3.0, 1.0, 0.0), glm::vec3(-5.0, -1.0, 0.0), glm::vec4(1.0, 1.0, 1.0, 1.0), 20.0, 0.5, 0.3, StaticArray<GLfloat, 3>{1.0, 0.0, 0.0});
//
//  visualiser.Cameras.resize(1);
//  visualiser.Cameras[0].SetOrientation(glm::vec3(0.0f, 0.0f, 1.0f), 0.0, 90.0);
//  visualiser.Cameras[0].SetViewFrustum(visualiser.ViewportAspectRatio(), 45.0, 1.0, -100.0);
//
////  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//
//  while(visualiser.isWindowOpen())
//  {
//    visualiser.BeginFrame();
//
//    visualiser.Cameras[0].KeyControl(visualiser.GetKeys(), visualiser.GetDeltaTime());
//    visualiser.Cameras[0].MousePositionControl(visualiser.GetMouseDisplacement());
//    visualiser.Cameras[0].MouseWheelControl(visualiser.GetMouseWheelDisplacement());
//
//    // If the viewport has been modified, update the view frustum
//    if(visualiser.isViewPortModified)
//    {
//      auto [width, height] = visualiser.ViewportDimensions();
//      visualiser.Cameras[0].SetViewFrustum(visualiser.ViewportAspectRatio());
//      visualiser.Shaders[0].SetUniform2f("u_resolution", (float)width, (float)height);
//    }
//
////    visualiser.RenderShadows();
//
//    visualiser.RenderScene();
//
//    visualiser.EndFrame();
//  }
//
//  return 0;
//}





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