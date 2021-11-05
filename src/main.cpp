#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../include/Global.h"
#include "../libs/Visualiser/include/Buffers.h"
#include "../libs/Visualiser/include/Shader.h"
#include "../libs/Visualiser/include/Renderer.h"
#include "../libs/Visualiser/include/Texture.h"

using namespace Apeiron;

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
          -0.5f, -0.5f, 0.0f, 0.0f,
           0.5f, -0.5f, 1.0f, 0.0f,
           0.5f,  0.5f, 1.0f, 1.0f,
          -0.5f,  0.5f, 0.0f, 1.0f
  };

  unsigned int indices[] = {
          0, 1, 2,
          2, 3, 0
  };

  GLCall(glEnable(GL_BLEND));
  GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

  VertexArray vertex_array;
  VertexBuffer vertex_buffer(positions, 4 * 4 * sizeof(float));

  VertexBufferLayout layout;
  layout.Push<float>(2);
  layout.Push<float>(2);
  vertex_array.AddBuffer(vertex_buffer, layout);

  IndexBuffer index_buffer(indices, 6);

  Shader shader("libs/Visualiser/resources/shaders/Basic.glsl");
  shader.Bind();
  shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

//  Texture texture("libs/Visualiser/resources/textures/Papyrus.png");
  Texture texture("libs/Visualiser/resources/textures/ChernoLogo.png");
  const UInt _slot = 0;
  texture.Bind(_slot);
  shader.SetUniform1i("u_Texture", _slot);

  vertex_array.Unbind();
  shader.Unbind();
  vertex_buffer.Unbind();
  index_buffer.Unbind();

  Renderer renderer;

  float red = 0.0f;
  float increment = 0.05f;

  // Loop until the user closes the window
  while(!glfwWindowShouldClose(window))
  {
    renderer.Clear();

    // Bind shader and setup uniforms.
    shader.Bind();
    shader.SetUniform4f("u_Color", red, 0.3, 0.8f, 1.0f);

    renderer.Draw(vertex_array, index_buffer, shader);

    if(red > 1.0f) increment = -0.05f;
    else if(red < 0.0f) increment = 0.05f;

    red += increment;

    // Swap front and back buffers
    glfwSwapBuffers(window);

    // Poll for and process events
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}