#include "../include/Window.h"

namespace Apeiron {

void Window::Open(GLint _width, GLint _height)
{
  if(!glfwInit())
  {
    glfwTerminate();
    EXIT("Failed to Initialise GLFW.")
  }

  // Window properties
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Enforces backward incompatibility
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  // Anti-aliasing properties
  glfwWindowHint(GLFW_SAMPLES, 16);
  glEnable(GL_MULTISAMPLE);

  // Create a window and its OpenGL context.
  WindowDimensions = {_width, _height};
  pWindow = glfwCreateWindow(_width, _height, "Apeiron Visualiser", nullptr, nullptr);
  if(!pWindow)
  {
    glfwTerminate();
    EXIT("Could not create an OpenGL window.")
  }

  // Set context for GLEW to use
  glfwMakeContextCurrent(pWindow);
  glfwSwapInterval(1);

  // Set viewport dimensions
  auto [width, height] = GetFrameBufferSize();
  ViewportDimensions[0] = width;
  ViewportDimensions[1] = height;

  // Handle key mouse inputs
  CreateCallBacks();
//  glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // Allow modern extension features
  glewExperimental = GL_TRUE;

  // Initialise GLEW
  if(glewInit() != GLEW_OK)
  {
    glfwDestroyWindow(pWindow);
    glfwTerminate();
    EXIT("Failed to Initialise GLEW.")
  }
  else Print("\nRunning OpenGL Version:", glGetString(GL_VERSION));

  GLCall(glEnable(GL_DEPTH_TEST));
  GLCall(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));

  glfwSetWindowUserPointer(pWindow, this);
}

StaticArray<GLdouble, 2> Window::GetMouseDisplacement()
{
  GLdouble x_displacement = MouseDisplacement[0];
  GLdouble y_displacement = MouseDisplacement[1];
  MouseDisplacement[0] = 0.0;
  MouseDisplacement[1] = 0.0;

  return {x_displacement, y_displacement};
}

bool Window::isViewPortModified()
{
  DEBUG_ASSERT(isOpen(), "The OpenGL window is not open.")

  // Adjust viewport, if necessary
  auto [width, height] = GetFrameBufferSize();
  if(width != ViewportDimensions[0] || height != ViewportDimensions[1])
  {
    ViewportDimensions[0] = width;
    ViewportDimensions[1] = height;
    GLCall(glViewport(0, 0, width, height));
    return true;
  }
  else return false;
}

void Window::SwapBuffers()
{
  glfwSwapBuffers(pWindow);
}

void Window::ComputeDeltaTime()
{
  const GLfloat current_time = glfwGetTime();
  DeltaTime = current_time - LastTime;
  LastTime = current_time;
}

void Window::CreateCallBacks()
{
  glfwSetKeyCallback(pWindow, HandleKeys);
  glfwSetCursorPosCallback(pWindow, HandleMouse);
}

void Window::HandleKeys(GLFWwindow* _p_window, const GLint _key, const GLint _code, const GLint _action, const GLint _mode)
{
  // Get pointer to the Window object which contains _p_window.
  Window* p_window = static_cast<Window*>(glfwGetWindowUserPointer(_p_window));

  // Check for pressing of the ESC key.
  if(_key == GLFW_KEY_ESCAPE && _action == GLFW_PRESS) glfwSetWindowShouldClose(_p_window, GL_TRUE);

  // Check for press/release of all other keys.
  if(isBounded(_key, static_cast<GLint>(0), mKeys))
  {
    if(_action == GLFW_PRESS) p_window->Keys[_key] = true;
    else if(_action == GLFW_RELEASE) p_window->Keys[_key] = false;
  }
}

void Window::HandleMouse(GLFWwindow* _p_window, const GLdouble _x_coord, const GLdouble _y_coord)
{
  // Get pointer to the Window object which contains _p_window.
  Window* p_window = static_cast<Window*>(glfwGetWindowUserPointer(_p_window));


  if(p_window->isFirstMouseMovement)
  {
    p_window->PreviousMousePosition = {_x_coord, _y_coord};
    p_window->isFirstMouseMovement = false;
  }

  p_window->MouseDisplacement = {_x_coord - p_window->PreviousMousePosition[0], _y_coord - p_window->PreviousMousePosition[1]};
  p_window->PreviousMousePosition = {_x_coord, _y_coord};
}

std::pair<GLint, GLint> Window::GetFrameBufferSize() const
{
  GLint width, height;
  glfwGetFramebufferSize(pWindow, &width, &height);
  return { width, height };
}

}

