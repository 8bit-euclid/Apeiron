#pragma once

#include "../../../include/Global.h"
#include "../../DataContainer/include/Array.h"
#include "../../LinearAlgebra/include/Vector.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

namespace aprn::vis {

class Camera
{
  constexpr static GLint mKeys = 1024;

 public:
   Camera();

   Camera(const glm::vec3& _position, GLfloat _pitch, GLfloat _yaw);

   void SetOrientation(const glm::vec3& _position, GLfloat _pitch, GLfloat _yaw);

   void SetViewFrustum(const GLfloat& _aspect_ratio, const GLfloat& _field_of_view = -1.0, const GLfloat& _near_plane = 0.0, const GLfloat& _far_plane = 0.0);

   void KeyControl(const StaticArray<Bool, mKeys>& _keys, const GLfloat& _delta_time);

   void MousePositionControl(const SVectorF2& _cursor_displacement);

   void MouseWheelControl(const SVectorF2& _wheel_displacement);

   void UpdateViewMatrix();

   void UpdateProjectionMatrix();

   inline const glm::vec3&
   GetPosition() const { return Position; }

   inline const glm::mat4&
   GetViewMatrix() const { return ViewMatrix; }

   inline const glm::mat4&
   GetProjectionMatrix() const { return ProjectionMatrix; }

 private:
   void BoundPitch() { Pitch = Bound(Pitch, -89.0f, 89.0f); }

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
