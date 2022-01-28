#include "../include/Window.h"
#include "LinearAlgebra/include/Vector.h"

#define GL_DEBUG_MODE

namespace Apeiron {

Window::Window(GLint _width, GLint _height)
  : WindowDimensions{_width, _height}
{
  Open(_width, _height);
}

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
#ifdef GL_DEBUG_MODE
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif

  // Framebuffer properties
//  glfwWindowHint(GLFW_RED_BITS, 8);
//  glfwWindowHint(GLFW_GREEN_BITS, 8);
//  glfwWindowHint(GLFW_BLUE_BITS, 8);
//  glfwWindowHint(GLFW_ALPHA_BITS, 8);

  // Anti-aliasing properties
  glfwWindowHint(GLFW_SAMPLES, 24);
  GLCall(glEnable(GL_MULTISAMPLE));

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
//  glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

  // Initialise OpenGL debug output
#ifdef GL_DEBUG_MODE
  int flags;
  GLCall(glGetIntegerv(GL_CONTEXT_FLAGS, &flags));
  if(flags & GL_CONTEXT_FLAG_DEBUG_BIT)
  {
    GLCall(glEnable(GL_DEBUG_OUTPUT));
    GLCall(glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS));
    GLCall(glDebugMessageCallback(glDebugOutput, nullptr));
    GLCall(glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE));
  }
#endif

  GLCall(glEnable(GL_DEPTH_TEST));
  GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));

//  GLCall(glEnable(GL_BLEND));
//  GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

  glfwSetWindowUserPointer(pWindow, this);
}

SVectorF2 Window::GetMouseDisplacement()
{
  GLdouble x_displacement = MouseDisplacement[0];
  GLdouble y_displacement = MouseDisplacement[1];
  MouseDisplacement[0] = 0.0;
  MouseDisplacement[1] = 0.0;

  return {x_displacement, y_displacement};
}

SVectorF2 Window::GetMouseWheelDisplacement()
{
  GLdouble x_displacement = MouseWheelDisplacement[0];
  GLdouble y_displacement = MouseWheelDisplacement[1];
  MouseWheelDisplacement[0] = 0.0;
  MouseWheelDisplacement[1] = 0.0;

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

void Window::ResetViewPort()
{
  GLCall(glViewport(0, 0, ViewportDimensions[0], ViewportDimensions[1]));
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

std::pair<GLint, GLint> Window::GetFrameBufferSize() const
{
  GLint width, height;
  glfwGetFramebufferSize(pWindow, &width, &height);
  return { width, height };
}

void Window::CreateCallBacks()
{
  glfwSetKeyCallback(pWindow, HandleKeys);
  glfwSetCursorPosCallback(pWindow, HandleMousePosition);
  glfwSetScrollCallback(pWindow, HandleMouseWheel);
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

void Window::HandleMousePosition(GLFWwindow* _p_window, const GLdouble _x_coord, const GLdouble _y_coord)
{
  Window* p_window = static_cast<Window*>(glfwGetWindowUserPointer(_p_window));

  if(p_window->isFirstMouseMovement)
  {
    p_window->PreviousMousePosition = {_x_coord, _y_coord};
    p_window->isFirstMouseMovement = false;
  }

  p_window->MouseDisplacement = {_x_coord - p_window->PreviousMousePosition[0], _y_coord - p_window->PreviousMousePosition[1]};
  p_window->PreviousMousePosition = {_x_coord, _y_coord};
}

void Window::HandleMouseWheel(GLFWwindow* _p_window, const GLdouble _x_offset, const GLdouble _y_offset)
{
  Window* p_window = static_cast<Window*>(glfwGetWindowUserPointer(_p_window));
  p_window->MouseWheelDisplacement = {0.0, _y_offset};
}

void Window::glDebugOutput(GLenum _source, GLenum _type, unsigned int id, GLenum _severity, GLsizei length, const char* message, const void* userParam)
{
  // Ignore insignificant error/warning codes
  if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

  std::cout << "---------------" << std::endl;
  std::cout << "Debug message (" << id << "): " << message << std::endl;

  // Write error source
  switch (_source)
  {
    case GL_DEBUG_SOURCE_API:
      std::cout << "Source: API"; break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
      std::cout << "Source: Window System"; break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
      std::cout << "Source: Shader Compiler"; break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:
      std::cout << "Source: Third Party"; break;
    case GL_DEBUG_SOURCE_APPLICATION:
      std::cout << "Source: Application"; break;
    case GL_DEBUG_SOURCE_OTHER:
      std::cout << "Source: Other"; break;
  } std::cout << std::endl;

  // Write error type
  switch (_type)
  {
    case GL_DEBUG_TYPE_ERROR:
      std::cout << "Type: Error"; break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
      std::cout << "Type: Deprecated Behaviour"; break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
      std::cout << "Type: Undefined Behaviour"; break;
    case GL_DEBUG_TYPE_PORTABILITY:
      std::cout << "Type: Portability"; break;
    case GL_DEBUG_TYPE_PERFORMANCE:
      std::cout << "Type: Performance"; break;
    case GL_DEBUG_TYPE_MARKER:
      std::cout << "Type: Marker"; break;
    case GL_DEBUG_TYPE_PUSH_GROUP:
      std::cout << "Type: Push Group"; break;
    case GL_DEBUG_TYPE_POP_GROUP:
      std::cout << "Type: Pop Group"; break;
    case GL_DEBUG_TYPE_OTHER:
      std::cout << "Type: Other"; break;
  } std::cout << std::endl;

  // Write error severity
  switch(_severity)
  {
    case GL_DEBUG_SEVERITY_HIGH:
      std::cout << "Severity: high"; break;
    case GL_DEBUG_SEVERITY_MEDIUM:
      std::cout << "Severity: medium"; break;
    case GL_DEBUG_SEVERITY_LOW:
      std::cout << "Severity: low"; break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
      std::cout << "Severity: notification"; break;
  } std::cout << std::endl;
  std::cout << std::endl;
}

}

