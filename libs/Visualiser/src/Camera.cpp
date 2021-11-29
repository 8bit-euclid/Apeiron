#include "../include/Camera.h"

namespace Apeiron {

Camera::Camera()
  : Position(glm::vec3(0.0f, 0.0f, 0.0)), Front(glm::vec3(0.0f, 0.0f, -1.0)), Pitch(0.0), Yaw(0.0), AspectRatio(0.0), FieldOfView(45.0), NearPlane(0.0),
    FarPlane(0.0)
{
  SetOrientation(Position, Pitch, Yaw);
}

Camera::Camera(const glm::vec3& _position, const GLfloat& _pitch, const GLfloat& _yaw)
  : Position(_position), Front(glm::vec3(0.0f, 0.0f, -1.0)), Pitch(_pitch), Yaw(_yaw), AspectRatio(0.0), FieldOfView(45.0), NearPlane(0.0),
    FarPlane(0.0)
{
  SetOrientation(Position, Pitch, Yaw);
}

Camera::~Camera()
{

}

void Camera::SetOrientation(const glm::vec3& _position, const GLfloat& _pitch, const GLfloat& _yaw)
{
  Position = _position;

  GLfloat pitch = glm::radians(_pitch);
  GLfloat yaw = glm::radians(_yaw);

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

glm::vec3 Camera::GetPosition()
{
  return Position;
}

void Camera::KeyControl(const StaticArray<Bool, mKeys>& _keys, const GLfloat& _delta_time)
{
  GLfloat displacement = Speed * _delta_time;

  if(_keys[GLFW_KEY_UP]) Position += displacement * Front;
  if(_keys[GLFW_KEY_DOWN]) Position -= displacement * Front;
  if(_keys[GLFW_KEY_LEFT]) Position -= displacement * Right;
  if(_keys[GLFW_KEY_RIGHT]) Position += displacement * Right;
}

// TODO - need to replace argument type with StaticVector, once it is implemented
void Camera::MouseControl(const StaticArray<GLdouble, 2>& _cursor_displacement)
{
  // Update yaw and pitch. Ensure that the pitch is in the range [-90, 90]
  Yaw += Sensitivity * _cursor_displacement[0];
  Pitch += Sensitivity * _cursor_displacement[1];
  Pitch = Bound(Pitch, -89.0f, 89.0f);

  // Update camera orientation
  SetOrientation(Position, Pitch, Yaw);
}

void Camera::ComputeViewMatrix(glm::mat4& _view_matrix)
{
//  _view_matrix = glm::mat4(1.0);
  _view_matrix = glm::lookAt(Position, Position + Front, WorldUp);
}

void Camera::ComputeProjectionMatrix(glm::mat4& _projection_matrix)
{
  _projection_matrix = glm::perspective(glm::radians(FieldOfView), AspectRatio, NearPlane, FarPlane);
//  _projection_matrix = glm::ortho(-AspectRatio, AspectRatio, -1.0f, 1.0f, -10.0f, 10.0f);
}

}

