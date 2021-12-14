#pragma once

#include "../../../include/Global.h"
#include "../../DataContainers/include/Array.h"
#include "GLDebug.h"
#include "GLTypes.h"
#include "Model.h"

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace Apeiron {

void CreateSquare(Model& _model, GLfloat _length);

void CreateRectangle(Model& _model, GLfloat _length, GLfloat _width);

template <class ...t_static_vector>
void CreatePolygon(Model& _model, const t_static_vector& ..._v);

void CreateTetrahedron(Model& _model, GLfloat _length);

void CreateTetrahedron(Model& _model, const StaticArray<GLfloat, 3>& _v0, const StaticArray<GLfloat, 3>& _v1, const StaticArray<GLfloat, 3>& _v2,
                       const StaticArray<GLfloat, 3>& _v3);

void CreateCube(Model& _model, GLfloat _length);

}
