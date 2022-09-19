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
#include "ModelGroup.h"

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
   static ModelGroup Segment(const SVector3<GLfloat>& v0, const SVector3<GLfloat>& v1);

   template<class... svectors>
   static ModelGroup SegmentChain(const svectors&... vs);

   static ModelGroup Arrow(const SVector3<GLfloat>& start, const SVector3<GLfloat>& end);

   /** 2D models
   ************************************************************************************************************************************************************/
   static ModelGroup Triangle(GLfloat length);

   static ModelGroup Triangle(GLfloat length, GLfloat height, GLfloat _apex_ratio);

   static ModelGroup Triangle(const SVector3<GLfloat>& v0, const SVector3<GLfloat>& v1, const SVector3<GLfloat>& v2);

   static ModelGroup Square(GLfloat length);

   static ModelGroup Rectangle(GLfloat length, GLfloat height);

   static ModelGroup ScreenQuad();

   static ModelGroup Quadrilateral(const SVector3<GLfloat>& v0, const SVector3<GLfloat>& v1, const SVector3<GLfloat>& v2, const SVector3<GLfloat>& v3);

   template<class... svectors>
   static ModelGroup Polygon(const svectors&... vs);

   static ModelGroup Arc(GLfloat radius, GLfloat angle);

   static ModelGroup Sector(GLfloat radius, GLfloat angle);

   static ModelGroup Circle(GLfloat radius);

   static ModelGroup Ellipse(GLfloat radius_x, GLfloat radius_y);

   /** 3D models
   ************************************************************************************************************************************************************/
   static ModelGroup Tetrahedron(GLfloat length);

   static ModelGroup Tetrahedron(const SVector3<GLfloat>& v0, const SVector3<GLfloat>& v1, const SVector3<GLfloat>& v2, const SVector3<GLfloat>& v3);

   static ModelGroup Cube(GLfloat length);

   static ModelGroup Cuboid(GLfloat length, GLfloat width, GLfloat height);

   static ModelGroup Octahedron(GLfloat length);

   static ModelGroup Dodecahedron(GLfloat length);

   static ModelGroup Icosahedron(GLfloat length);

   static ModelGroup Sphere(GLfloat radius);

   static ModelGroup Ellipsoid(GLfloat radius_x, GLfloat radius_y, GLfloat radius_z);

   static ModelGroup Cylinder(GLfloat radius, GLfloat height);

   static ModelGroup Cone(GLfloat radius, GLfloat height);
};

}
