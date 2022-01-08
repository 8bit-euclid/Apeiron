#pragma once

#include "../../../include/Global.h"
#include "../../DataContainers/include/Array.h"
#include "Categories.h"

namespace Apeiron {
namespace Geometry {

/***************************************************************************************************************************************************************
* n-Polytope Abstract Base Class Definitions
***************************************************************************************************************************************************************/
template<PolytopeCategory t_category, class t_data_type = Float, std::size_t t_dimension = 3>
struct Polytope
{
  Polytope() { STATIC_ASSERT(isFloatingPoint<t_data_type>(), "Can only create polytopes with floating-point types.") }

  virtual ~Polytope() = 0;
};

template<PolytopeCategory t_category, class t_data_type = Float, std::size_t t_dimension = 3>
struct StaticPolytope : public Polytope<t_category, t_data_type, t_dimension>
{
  virtual ~StaticPolytope() = 0;

  StaticArray<StaticArray<t_data_type, t_dimension>, GetPolytopeVertexCount<t_category>()> Vertices;
  StaticArray<StaticArray<std::size_t, GetPolytopeFaceVertexCount<t_category>()>, GetPolytopeFaceCount<t_category>()> Faces;
};

template<PolytopeCategory t_category, class t_data_type = Float, std::size_t t_dimension = 3>
struct DynamicPolytope : public Polytope<t_category, t_data_type, t_dimension>
{
  virtual ~DynamicPolytope() = 0;

  DynamicArray<StaticArray<t_data_type, t_dimension>> Vertices;
  DynamicArray<DynamicArray<std::size_t>> Faces;
};

/***************************************************************************************************************************************************************
* 2-Polytope Class Definitions
***************************************************************************************************************************************************************/
/** Arbitrary Polygon *****************************************************************************************************************************************/
template<PolytopeCategory t_category, std::size_t t_n_vertices, class t_data_type = Float, std::size_t t_dimension = 3>
struct StaticPolygon : public StaticPolytope<t_category, t_data_type, t_dimension>
{
  StaticPolygon() { STATIC_ASSERT((isNPolytope<t_category, 2>()), "Can only create polytopes with floating-point types.") }

  StaticPolygon(const t_data_type _side_length);

  template<class ...t_static_vector>
  StaticPolygon(const t_static_vector& ..._vertices);
};

///** Triangle **************************************************************************************************************************************************/
//template<class t_data_type = Float, std::size_t t_dimension = 3>
//struct Triangle : public Polygon<t_data_type, t_dimension>
//{
//  Triangle(const StaticArray<t_data_type, t_dimension> &_v0, const StaticArray<t_data_type, t_dimension> &_v1, const StaticArray<t_data_type, t_dimension> &_v2);
//};
//
///** Equilateral Triangle **************************************************************************************************************************************/
//template<class t_data_type = Float, std::size_t t_dimension = 3>
//struct EquilateralTriangle : public Triangle<t_data_type, t_dimension>
//{
//  EquilateralTriangle(const t_data_type _side_length);
//};
//
///** Right Triangle ********************************************************************************************************************************************/
//template<class t_data_type = Float, std::size_t t_dimension = 3>
//struct RightTriangle : public Triangle<t_data_type, t_dimension>
//{
//  RightTriangle(const t_data_type _length, const t_data_type _height, const bool _flip_horizontally = false);
//};
//
///** Isosceles Triangle ****************************************************************************************************************************************/
//template<class t_data_type = Float, std::size_t t_dimension = 3>
//struct IsoscelesTriangle : public Triangle<t_data_type, t_dimension>
//{
//  IsoscelesTriangle(const t_data_type _length, const t_data_type _height);
//};
//
///** Arbitrary Quadrilateral ***********************************************************************************************************************************/
//template<class t_data_type = Float, std::size_t t_dimension = 3>
//struct Quadrilateral : public Polygon<t_data_type, t_dimension>
//{
//  Quadrilateral(const StaticArray<t_data_type, t_dimension> &_v0, const StaticArray<t_data_type, t_dimension> &_v1,
//                const StaticArray<t_data_type, t_dimension> &_v2, const StaticArray<t_data_type, t_dimension> &_v3);
//};
//
///** Rectangle *************************************************************************************************************************************************/
//template<class t_data_type = Float, std::size_t t_dimension = 3>
//struct Rectangle : public Quadrilateral<t_data_type, t_dimension>
//{
//  Rectangle(const t_data_type _length, const t_data_type _height);
//};
//
///** Square ****************************************************************************************************************************************************/
//template<class t_data_type = Float, std::size_t t_dimension = 3>
//struct Square : public Rectangle<t_data_type, t_dimension>
//{
//  Square(const t_data_type _side_length);
//};
//
///***************************************************************************************************************************************************************
//* 3-Polytope Class Definitions
//***************************************************************************************************************************************************************/
///** Tetrahedron ***********************************************************************************************************************************************/
//void CreateTetrahedron(Model &_model, GLfloat _side_length);
//
//void CreateTetrahedron(Model &_model, const StaticArray<GLfloat, 3> &_v0, const StaticArray<GLfloat, 3> &_v1, const StaticArray<GLfloat, 3> &_v2,
//                       const StaticArray<GLfloat, 3> &_v3);
//
///** Tetrahedron ***********************************************************************************************************************************************/
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