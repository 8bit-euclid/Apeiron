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

#pragma once

#include "../../../include/Global.h"
#include "../../DataContainer/include/Array.h"
#include "../../LinearAlgebra/include/Vector.h"
#include "../../Manifold/include/Curve.h"
#include "GLDebug.h"
#include "GLTypes.h"
#include "Model.h"

#include <functional>
#include <memory>
#include <GL/glew.h>
#include <glm/glm.hpp>

namespace aprn::vis {

class ModelFactory
{
 protected:
   ModelFactory() = default;

 public:
   /** 1D models
   ************************************************************************************************************************************************************/
   static Model Segment(const SVector3<GLfloat>& v0, const SVector3<GLfloat>& v1);

   static Model Arrow(const SVector3<GLfloat>& v0, const SVector3<GLfloat>& v1);

   /** 2D models
   ************************************************************************************************************************************************************/
   template<class... svectors>
   static Model SegmentChain(const svectors&... vs);

   static Model Triangle(GLfloat length);

   static Model Triangle(GLfloat length, GLfloat height, GLfloat _apex_ratio);

   static Model Triangle(const SVector3<GLfloat>& v0, const SVector3<GLfloat>& v1, const SVector3<GLfloat>& v2);

   static Model Square(GLfloat length);

   static Model Rectangle(GLfloat length, GLfloat height);

   static Model Quadrilateral(const SVector3<GLfloat>& v0, const SVector3<GLfloat>& v1, const SVector3<GLfloat>& v2, const SVector3<GLfloat>& v3);

   template<class... svectors>
   static Model Polygon(const svectors&... vs);

   static Model Arc(GLfloat radius, GLfloat angle);

   static Model Sector(GLfloat radius, GLfloat angle);

   static Model Circle(GLfloat radius);

   static Model Ellipse(GLfloat radius_x, GLfloat radius_y);

   /** 3D models
   ************************************************************************************************************************************************************/
   static Model Tetrahedron(GLfloat length);

   static Model Tetrahedron(const SVector3<GLfloat>& v0, const SVector3<GLfloat>& v1, const SVector3<GLfloat>& v2, const SVector3<GLfloat>& v3);

   static Model Cube(GLfloat length);

   static Model Cuboid(GLfloat length, GLfloat width, GLfloat height);

   static Model Octahedron(GLfloat length);

   static Model Dodecahedron(GLfloat length);

   static Model Icosahedron(GLfloat length);

   static Model Sphere(GLfloat radius);

   static Model Ellipsoid(GLfloat radius_x, GLfloat radius_y, GLfloat radius_z);

   static Model Cylinder(GLfloat radius, GLfloat height);

   static Model Cone(GLfloat radius, GLfloat height);
};

}
