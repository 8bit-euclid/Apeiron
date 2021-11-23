#include "../include/Visualiser.h"

namespace Apeiron {

void Visualiser::BeginFrame()
{
  // Compute delta time for previous frame
  OpenGLWindow.ComputeDeltaTime();

  // Get and handle user inputs
  glfwPollEvents();

  // Check if the viewport has been modified
  isViewPortModified = OpenGLWindow.isViewPortModified();

  // Clear window
  GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
  GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Visualiser::EndFrame()
{
  OpenGLWindow.SwapBuffers();
}

}

