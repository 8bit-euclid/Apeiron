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
   static Model Segment(const SVector3<GLfloat>& _v0, const SVector3<GLfloat>& _v1);

   static Model Arrow(const SVector3<GLfloat>& _v0, const SVector3<GLfloat>& _v1);

   /** 2D models
   ************************************************************************************************************************************************************/
   template<class... svectors>
   static Model SegmentChain(const svectors&... _v);

   static Model Triangle(GLfloat _length);

   static Model Triangle(GLfloat _length, GLfloat _height, GLfloat _apex_ratio);

   static Model Triangle(const SVector3<GLfloat>& _v0, const SVector3<GLfloat>& _v1, const SVector3<GLfloat>& _v2);

   static Model Square(GLfloat _length);

   static Model Rectangle(GLfloat _length, GLfloat _height);

   static Model Quadrilateral(const SVector3<GLfloat>& _v0, const SVector3<GLfloat>& _v1, const SVector3<GLfloat>& _v2, const SVector3<GLfloat>& _v3);

   template<class... svectors>
   static Model Polygon(const svectors& ..._v);

   static Model Arc(GLfloat _radius, GLfloat angle);

   static Model Sector(GLfloat _radius, GLfloat angle);

   static Model Circle(GLfloat _radius);

   static Model Ellipse(GLfloat _radius_x, GLfloat _radius_y);

   /** 3D models
   ************************************************************************************************************************************************************/
   static Model Tetrahedron(GLfloat _length);

   static Model Tetrahedron(const SVector3<GLfloat>& _v0, const SVector3<GLfloat>& _v1, const SVector3<GLfloat>& _v2, const SVector3<GLfloat>& _v3);

   static Model Cube(GLfloat _length);

   static Model Cuboid(GLfloat _length, GLfloat _width, GLfloat _height);

   static Model Octahedron(GLfloat _length);

   static Model Dodecahedron(GLfloat _length);

   static Model Icosahedron(GLfloat _length);

   static Model Sphere(GLfloat _radius);

   static Model Ellipsoid(GLfloat _radius_x, GLfloat _radius_y, GLfloat _radius_z);

   static Model Cylinder(GLfloat _radius, GLfloat _height);

   static Model Cone(GLfloat _radius, GLfloat _height);
};

}
