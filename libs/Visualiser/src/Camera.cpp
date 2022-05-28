#include "../include/Camera.h"

namespace aprn::vis {

Camera::Camera()
  : Camera(glm::vec3(0.0f, 0.0f, 0.0), 0.0f, 0.0f) {}

Camera::Camera(const glm::vec3& _position, GLfloat _pitch, GLfloat _yaw)
  : Position(_position), Front(glm::vec3(0.0f, 0.0f, -1.0f)), Pitch(_pitch), Yaw(_yaw), AspectRatio(0.0), FieldOfView(45.0), NearPlane(0.0),
    FarPlane(0.0)
{
  SetOrientation(Position, Pitch, Yaw);
}

void Camera::SetOrientation(const glm::vec3& _position, GLfloat _pitch, GLfloat _yaw)
{
  Position = _position;

  auto pitch = glm::radians(_pitch);
  auto yaw = glm::radians(_yaw);

  Front.x = glm::cos(yaw) * glm::cos(pitch);
  Front.y = glm::sin(pitch);
  Front.z = glm::sin(yaw) * glm::cos(pitch);
  Front = glm::normalize(Front);

  Right = glm::normalize(glm::cross(Front, WorldUp));
  Up = glm::normalize(glm::cross(Right, Front));
}

void Camera::SetViewFrustum(const GLfloat& _aspect_ratio, const GLfloat& _field_of_view, const GLfloat& _near_plane, const GLfloat& _far_plane)
{
  AspectRatio = _aspect_ratio;
  if(_field_of_view > 0.0)
  {
    FieldOfView = _field_of_view;
    NearPlane = _near_plane;
    FarPlane = _far_plane;
  }
}

void Camera::KeyControl(const StaticArray<Bool, mKeys>& _keys, const GLfloat& _delta_time)
{
   GLfloat displacement = MoveSpeed * _delta_time;

   // Position control
   if(_keys[GLFW_KEY_W]) Position += displacement * Front;
   if(_keys[GLFW_KEY_S]) Position -= displacement * Front;
   if(_keys[GLFW_KEY_A]) Position -= displacement * Right;
   if(_keys[GLFW_KEY_D]) Position += displacement * Right;
   if(_keys[GLFW_KEY_Q]) Position += displacement * WorldUp;
   if(_keys[GLFW_KEY_E]) Position -= displacement * WorldUp;

   // Pitch control
   if(_keys[GLFW_KEY_UP])
   {
     Pitch += 8.0 * displacement;
     BoundPitch();
   }
   if(_keys[GLFW_KEY_DOWN])
   {
     Pitch -= 8.0 * displacement;
     BoundPitch();
   }

   // Yaw control
   if(_keys[GLFW_KEY_LEFT]) Yaw -= 8.0 * displacement;
   if(_keys[GLFW_KEY_RIGHT]) Yaw += 8.0 * displacement;
}

// TODO - need to replace argument type with StaticVector, once it is implemented
void Camera::MousePositionControl(const SVectorF2& _cursor_displacement)
{
  // Update yaw and pitch. Ensure that the pitch is in the range [-90, 90]
  Yaw += MousePositionSensitivity * _cursor_displacement[0];
  Pitch -= MousePositionSensitivity * _cursor_displacement[1];
  BoundPitch();

  // Update camera orientation
  SetOrientation(Position, Pitch, Yaw);
}

void Camera::MouseWheelControl(const SVectorF2& _wheel_displacement)
{
  GLfloat displacement = MouseWheelSensitivity * _wheel_displacement[1];
  Position += displacement * Front;
}

void Camera::UpdateViewMatrix()
{
//  ViewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), WorldUp);
  ViewMatrix = glm::lookAt(Position, Position + Front, WorldUp);
}

void Camera::UpdateProjectionMatrix()
{
  ProjectionMatrix = glm::perspective(glm::radians(FieldOfView), AspectRatio, NearPlane, FarPlane);
//  ProjectionMatrix = glm::ortho(-AspectRatio, AspectRatio, -1.0f, 1.0f, -10.0f, 10.0f);
}

}

