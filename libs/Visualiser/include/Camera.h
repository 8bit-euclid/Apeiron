#pragma once

#include "../../../include/Global.h"
#include "../../DataContainers/include/Array.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

namespace Apeiron {

class Camera
{
  constexpr static GLint mKeys = 1024;

public:
  Camera();

  Camera(const glm::vec3& _position, const GLfloat& _pitch, const GLfloat& _yaw);

  void SetOrientation(const glm::vec3& _position, const GLfloat& _pitch, const GLfloat& _yaw);

  void SetViewFrustum(const GLfloat& _aspect_ratio, const GLfloat& _field_of_view = -1.0, const GLfloat& _near_plane = 0.0, const GLfloat& _far_plane = 0.0);

  void KeyControl(const StaticArray<Bool, mKeys>& _keys, const GLfloat& _delta_time);

  // TODO - need to replace argument type with StaticVector, once it is implemented
  void MousePositionControl(const StaticArray<GLdouble, 2>& _cursor_displacement);

  // TODO - need to replace argument type with StaticVector, once it is implemented
  void MouseWheelControl(const StaticArray<GLdouble, 2>& _wheel_displacement);

  void UpdateViewMatrix();

  void UpdateProjectionMatrix();

  void BoundPitch() { Pitch = Bound(Pitch, -89.0f, 89.0f); }

  inline const glm::vec3& GetPosition() const { return Position; }

  inline const glm::mat4& GetViewMatrix() const { return ViewMatrix; }

  inline const glm::mat4& GetProjectionMatrix() const { return ProjectionMatrix; }

private:
  glm::vec3 Position;
  glm::vec3 Front;
  glm::vec3 Up;
  glm::vec3 Right;
  constexpr static glm::vec3 WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

  constexpr static GLfloat MoveSpeed{10.0f};
  constexpr static GLfloat MousePositionSensitivity{0.4f};
  constexpr static GLfloat MouseWheelSensitivity{1.0f};

  GLfloat Pitch;
  GLfloat Yaw;
  GLfloat AspectRatio;
  GLfloat FieldOfView;
  GLfloat NearPlane;
  GLfloat FarPlane;

  glm::mat4 ViewMatrix{1.0};
  glm::mat4 ProjectionMatrix{1.0};
};

}
