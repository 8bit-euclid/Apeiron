#pragma once

#include "../../../include/Global.h"
#include "../../DataContainer/include/Array.h"
#include "GLDebug.h"
#include "GLTypes.h"
#include "Model.h"

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace Apeiron {

/***************************************************************************************************************************************************************
* 2D
***************************************************************************************************************************************************************/
void CreateTriangle(Model& _model, GLfloat _length);

void CreateTriangle(Model& _model, const StaticArray<GLfloat, 3>& _v0, const StaticArray<GLfloat, 3>& _v1, const StaticArray<GLfloat, 3>& _v2);

void CreateSquare(Model& _model, GLfloat _length);

void CreateRectangle(Model& _model, GLfloat _length, GLfloat _height);

void CreateQuadrilateral(Model& _model, const StaticArray<GLfloat, 3>& _v0, const StaticArray<GLfloat, 3>& _v1, const StaticArray<GLfloat, 3>& _v2,
                         const StaticArray<GLfloat, 3>& _v3);

template <class ...t_static_vector>
void CreatePolygon(Model& _model, const t_static_vector& ..._v);

/***************************************************************************************************************************************************************
* 3D
***************************************************************************************************************************************************************/
void CreateTetrahedron(Model& _model, GLfloat _length);

void CreateTetrahedron(Model& _model, const StaticArray<GLfloat, 3>& _v0, const StaticArray<GLfloat, 3>& _v1, const StaticArray<GLfloat, 3>& _v2,
                       const StaticArray<GLfloat, 3>& _v3);

void CreateCube(Model& _model, GLfloat _length);

void CreateCuboid(Model& _model, const StaticArray<GLfloat, 3>& _lengths);

void CreateOctahedron(Model& _model, GLfloat _length);

void CreateDodecahedron(Model& _model, GLfloat _length);

void CreateIcosahedron(Model& _model, GLfloat _length);

void CreateSphere(Model& _model, GLfloat _radius);

void CreateEllipsoid(Model& _model, GLfloat _radius_x, GLfloat _radius_y, GLfloat _radius_z);

void CreateCylinder(Model& _model);

void CreateCone(Model& _model);

}
