/***************************************************************************************************************************************************************
* GPL-3.0 License
* Copyright (C) 2022 Niran A. Ilangakoon
*
* This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as
* published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
* of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program.
* If not, see <https://www.gnu.org/licenses/>.
***************************************************************************************************************************************************************/

#include "../include/Camera.h"

namespace aprn::vis {

Camera::Camera()
  : Camera(glm::vec3(0.0f, 0.0f, 0.0), 0.0f, 0.0f) {}

Camera::Camera(const glm::vec3& position, GLfloat _pitch, GLfloat _yaw)
  : Position(position), Front(glm::vec3(0.0f, 0.0f, -1.0f)), Pitch(_pitch), Yaw(_yaw), AspectRatio(0.0), FieldOfView(45.0), NearPlane(0.0),
    FarPlane(0.0)
{
  SetOrientation(Position, Pitch, Yaw);
}

void Camera::SetOrientation(const glm::vec3& position, GLfloat _pitch, GLfloat _yaw)
{
  Position = position;

  auto pitch = glm::radians(_pitch);
  auto yaw   = glm::radians(_yaw);

  Front.x = glm::cos(yaw) * glm::cos(pitch);
  Front.y = glm::sin(pitch);
  Front.z = glm::sin(yaw) * glm::cos(pitch);
  Front   = glm::normalize(Front);

  Right = glm::normalize(glm::cross(Front, WorldUp));
  Up    = glm::normalize(glm::cross(Right, Front));
}

void Camera::SetViewFrustum(const GLfloat& _aspect_ratio, const GLfloat& field_of_view, const GLfloat& near_plane, const GLfloat& far_plane)
{
   AspectRatio = _aspect_ratio;
   if(field_of_view > 0.0)
   {
      FieldOfView = field_of_view;
      NearPlane   = near_plane;
      FarPlane    = far_plane;
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
      ClipPitch();
   }
   if(_keys[GLFW_KEY_DOWN])
   {
      Pitch -= 8.0 * displacement;
      ClipPitch();
   }

   // Yaw control
   if(_keys[GLFW_KEY_LEFT])  Yaw -= 8.0 * displacement;
   if(_keys[GLFW_KEY_RIGHT]) Yaw += 8.0 * displacement;
}

// TODO - need to replace argument type with StaticVector, once it is implemented
void Camera::MousePositionControl(const SVectorF2& cursor_displacement)
{
   // Update yaw and pitch. Ensure that the pitch is in the range [-90, 90]
   Yaw   += MousePositionSensitivity * cursor_displacement[0];
   Pitch -= MousePositionSensitivity * cursor_displacement[1];
   ClipPitch();

   // Update camera orientation
   SetOrientation(Position, Pitch, Yaw);
}

void Camera::MouseWheelControl(const SVectorF2& wheel_displacement)
{
   GLfloat displacement = MouseWheelSensitivity * wheel_displacement[1];
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

