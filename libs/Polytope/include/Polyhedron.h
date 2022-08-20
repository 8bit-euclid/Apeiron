#pragma once

#include "../../../include/Global.h"
#include "../../DataContainer/include/Array.h"
#include "Categories.h"
#include "Polytope.h"

namespace aprn::ptope {

/***************************************************************************************************************************************************************
* Static/Dynamic Polyhedra
***************************************************************************************************************************************************************/
template<PolytopeCategory cat>
struct Polyhedron : public StaticPolytope<cat, 3>
{
  Polyhedron() { STATIC_ASSERT((isNPolytope<cat, 3>()), "A polyhedron must be a 3-polytope.") }

  Polyhedron(const Float _side_length);

  template<class... t_static_vector>
  Polyhedron(const t_static_vector&... _vertices);
};

template<>
struct Polyhedron<PolytopeCategory::Arbitrary3D> : public DynamicPolytope<PolytopeCategory::Arbitrary3D, 3>
{
  Polyhedron();
};

/***************************************************************************************************************************************************************
* Tetrahedra
***************************************************************************************************************************************************************/
struct Tetrahedron : public Polyhedron<PolytopeCategory::Tetrahedron>
{
  Tetrahedron(const SVectorF3& _v0, const SVectorF3& _v1, const SVectorF3& _v2, const SVectorF3& _v3);
};

struct RegularTetrahedron : public Tetrahedron
{
  RegularTetrahedron(const Float _side_length);
};

struct TrirectangularTetrahedron : public Tetrahedron
{
  TrirectangularTetrahedron(const Float _length, const Float height, const Float width, const bool _flip_horizontally = false);
};

/***************************************************************************************************************************************************************
* Static/Dynamic Polygons
***************************************************************************************************************************************************************/
//void CreateCube(Model &model, GLfloat _side_length);
//
//void CreateCuboid(Model &model, const SVectorF3& _lengths);
//
//void CreateOctahedron(Model &model, GLfloat _side_length);
//
//void CreateDodecahedron(Model &model, GLfloat _side_length);
//
//void CreateIcosahedron(Model &model, GLfloat _side_length);
//
//void CreateSphere(Model &model, GLfloat _radius);
//
//void CreateEllipsoid(Model &model, GLfloat _radius_x, GLfloat _radius_y, GLfloat _radius_z);
//
//void CreateCylinder(Model &model);
//
//void CreateCone(Model &model);

}
