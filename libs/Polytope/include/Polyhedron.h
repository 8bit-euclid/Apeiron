#pragma once

#include "../../../include/Global.h"
#include "../../DataContainer/include/Array.h"
#include "Categories.h"
#include "Polytope.h"

namespace Apeiron {
namespace Shapes {

/***************************************************************************************************************************************************************
* Static/Dynamic Polyhedra
***************************************************************************************************************************************************************/
template<PolytopeCategory t_category>
struct Polyhedron : public StaticPolytope<t_category, 3>
{
  Polyhedron() { STATIC_ASSERT((isNPolytope<t_category, 3>()), "A polyhedron must be a 3-polytope.") }

  Polyhedron(const Float _side_length);

  template<class ...t_static_vector>
  Polyhedron(const t_static_vector& ..._vertices);
};

template<>
struct Polyhedron<PolytopeCategory::Arbitrary3D> : public DynamicPolytope<PolytopeCategory::Arbitrary3D, 3>
{
  Polyhedron();
};

/***************************************************************************************************************************************************************
* Tetrahedra
***************************************************************************************************************************************************************/
struct Tetrahedron : public Polyhedron<PolytopeCategory::Tetrahedral>
{
  Tetrahedron(const StaticArray<Float, 3>& _v0, const StaticArray<Float, 3>& _v1, const StaticArray<Float, 3>& _v2, const StaticArray<Float, 3>& _v3);
};

struct RegularTetrahedron : public Tetrahedron
{
  RegularTetrahedron(const Float _side_length);
};

struct TrirectangularTetrahedron : public Tetrahedron
{
  TrirectangularTetrahedron(const Float _length, const Float _height, const Float _width, const bool _flip_horizontally = false);
};

/***************************************************************************************************************************************************************
* Static/Dynamic Polygons
***************************************************************************************************************************************************************/
//void CreateCube(Model &_model, GLfloat _side_length);
//
//void CreateCuboid(Model &_model, const StaticArray<GLfloat, 3> &_lengths);
//
//void CreateOctahedron(Model &_model, GLfloat _side_length);
//
//void CreateDodecahedron(Model &_model, GLfloat _side_length);
//
//void CreateIcosahedron(Model &_model, GLfloat _side_length);
//
//void CreateSphere(Model &_model, GLfloat _radius);
//
//void CreateEllipsoid(Model &_model, GLfloat _radius_x, GLfloat _radius_y, GLfloat _radius_z);
//
//void CreateCylinder(Model &_model);
//
//void CreateCone(Model &_model);

}
}
