#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../include/Global.h"
#include "../libs/Visualiser/include/Buffers.h"
#include "../libs/Visualiser/include/Renderer.h"

using namespace Apeiron;

struct ShaderProgramSource
{
  std::string VertexSource;
  std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& _file_path)
{
  std::ifstream stream(_file_path);

  enum class ShaderType { None = -1, Vertex = 0, Fragment = 1 };

  std::string line;
  std::stringstream string_stream[2];
  ShaderType type = ShaderType::None;
  while(getline(stream, line))
  {
    if(line.find("#shader") != std::string::npos)
    {
      if(line.find("vertex") != std::string::npos) type = ShaderType::Vertex;
      else if(line.find("fragment") != std::string::npos) type = ShaderType::Fragment;
    }
    else string_stream[static_cast<int>(type)] << line << '\n';
  }

  return { string_stream[0].str(), string_stream[1].str() };
}

static unsigned int CompileShader(unsigned int _type, const std::string& _source)
{
  unsigned int id = glCreateShader(_type);
  const char* src = _source.c_str();

  glShaderSource(id, 1, &src, nullptr);
  glCompileShader(id);

  int result;
  glGetShaderiv(id, GL_COMPILE_STATUS, &result);
  if(result == GL_FALSE)
  {
    int length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    char* message = (char*)alloca(length * sizeof(char));
    glGetShaderInfoLog(id, length, &length, message);
    EXIT("Failed to compile ", (_type == GL_VERTEX_SHADER ? "vertex" : "fragment"), " shader:\n", message);
  }

  return id;
}

static int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
  unsigned int program = glCreateProgram();
  unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
  unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  glValidateProgram(program);

  glDeleteShader(vs);
  glDeleteShader(fs);

  return program;
}

int main(void)
{
  GLFWwindow* window;

  /* Initialize the library */
  if (!glfwInit())
    return -1;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
  if(!window)
  {
    glfwTerminate();
    return -1;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  ASSERT(glewInit() == GLEW_OK, "Failed to Initialise OpenGL.")
  Print("\nOpenGL Version:", glGetString(GL_VERSION));

  float positions[] = {
          -0.5f, -0.5f,
           0.5f, -0.5f,
           0.5f,  0.5f,
          -0.5f,  0.5f,
  };

  unsigned int indices[] = {
          0, 1, 2,
          2, 3, 0
  };

  unsigned int vertex_array;
  GLCall(glGenVertexArrays(1, &vertex_array));
  GLCall(glBindVertexArray(vertex_array));

  VertexBuffer vertex_buffer(positions, 6 * 2 * sizeof(float));

  GLCall(glEnableVertexAttribArray(0));
  GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));

  IndexBuffer index_buffer(indices, 6);

  ShaderProgramSource source = ParseShader("./libs/Visualiser/resources/shaders/Basic.shader");
  unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
  GLCall(glUseProgram(shader));

  GLCall(int location = glGetUniformLocation(shader, "u_Color"));
  ASSERT(location != -1, "Could not find uniform location.")
  GLCall(glUniform4f(location, 0.8f, 0.3, 0.8f, 1.0f));

  GLCall(glBindVertexArray(0));
  GLCall(glUseProgram(0));
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

  float red = 0.0f;
  float increment = 0.05f;

  // Loop until the user closes the window
  while(!glfwWindowShouldClose(window))
  {
    // Render here
    GLCall(glClear(GL_COLOR_BUFFER_BIT));

    // Bind shader and setup uniforms.
    GLCall(glUseProgram(shader));
    GLCall(glUniform4f(location, red, 0.3, 0.8f, 1.0f));

    // Bind index buffer.
    GLCall(glBindVertexArray(vertex_array));
    index_buffer.Bind();

    GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

    if(red > 1.0f) increment = -0.05f;
    else if(red < 0.0f) increment = 0.05f;

    red += increment;

    // Swap front and back buffers
    GLCall(glfwSwapBuffers(window));

    // Poll for and process events
    GLCall(glfwPollEvents());
  }

  GLCall(glDeleteProgram(shader));

  GLCall(glfwTerminate());
  return 0;
}