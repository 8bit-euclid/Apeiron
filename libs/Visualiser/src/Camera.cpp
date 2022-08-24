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

Camera::Camera(const glm::vec3& position, GLfloat pitch, GLfloat yaw)
  : _Position(position), _Front(glm::vec3(0.0f, 0.0f, -1.0f)), _Pitch(pitch), _Yaw(yaw), _AspectRatio(0.0), _FieldOfView(45.0), _NearPlane(0.0),
    _FarPlane(0.0)
{
  SetOrientation(_Position, _Pitch, _Yaw);
}

void Camera::SetOrientation(const glm::vec3& position, GLfloat pitch, GLfloat yaw)
{
   _Position = position;

   pitch = glm::radians(pitch);
   yaw   = glm::radians(yaw);

   _Front.x = glm::cos(yaw) * glm::cos(pitch);
   _Front.y = glm::sin(pitch);
   _Front.z = glm::sin(yaw) * glm::cos(pitch);
   _Front   = glm::normalize(_Front);

   _Right = glm::normalize(glm::cross(_Front, _WorldUp));
   _Up    = glm::normalize(glm::cross(_Right, _Front));
}

void Camera::SetViewFrustum(const GLfloat& aspect_ratio, const GLfloat& field_of_view, const GLfloat& near_plane, const GLfloat& far_plane)
{
   _AspectRatio = aspect_ratio;
   if(field_of_view > 0.0)
   {
      _FieldOfView = field_of_view;
      _NearPlane   = near_plane;
      _FarPlane    = far_plane;
   }
}

void Camera::KeyControl(const StaticArray<Bool, KeyCount>& keys, const GLfloat& delta_time)
{
   GLfloat displacement = _MotionSensitivity * delta_time;

   // Position control
   if(keys[GLFW_KEY_W]) _Position += displacement * _Front;
   if(keys[GLFW_KEY_S]) _Position -= displacement * _Front;
   if(keys[GLFW_KEY_A]) _Position -= displacement * _Right;
   if(keys[GLFW_KEY_D]) _Position += displacement * _Right;
   if(keys[GLFW_KEY_Q]) _Position += displacement * _WorldUp;
   if(keys[GLFW_KEY_E]) _Position -= displacement * _WorldUp;

   // Pitch control
   if(keys[GLFW_KEY_UP])
   {
      _Pitch += _RotationSensitivity * displacement;
      ClipPitch();
   }
   if(keys[GLFW_KEY_DOWN])
   {
      _Pitch -= _RotationSensitivity * displacement;
      ClipPitch();
   }

   // Yaw control
   if(keys[GLFW_KEY_LEFT]) _Yaw -= _RotationSensitivity * displacement;
   if(keys[GLFW_KEY_RIGHT]) _Yaw += _RotationSensitivity * displacement;
}

void Camera::CursorControl(const SVectorF2& cursor_displacement)
{
   // Update yaw and pitch. Ensure that the pitch is in the range [-90, 90]
   _Yaw   += _MouseCursorSensitivity * cursor_displacement[0];
   _Pitch -= _MouseCursorSensitivity * cursor_displacement[1];
   ClipPitch();

   // Update camera orientation
   SetOrientation(_Position, _Pitch, _Yaw);
}

void Camera::WheelControl(const SVectorF2& wheel_displacement)
{
   GLfloat displacement = _MouseWheelSensitivity * wheel_displacement[1];
   _Position += displacement * _Front;
}

void Camera::UpdateViewMatrix()
{
//  ViewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), WorldUp);
   _ViewMatrix = glm::lookAt(_Position, _Position + _Front, _WorldUp);
}

void Camera::UpdateProjMatrix()
{
   _ProjMatrix = glm::perspective(glm::radians(_FieldOfView), _AspectRatio, _NearPlane, _FarPlane);
//  ProjectionMatrix = glm::ortho(-AspectRatio, AspectRatio, -1.0f, 1.0f, -10.0f, 10.0f);
}

}

