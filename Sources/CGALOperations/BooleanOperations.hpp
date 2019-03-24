#ifndef CGAL_BOOLEANOPERATIONS_HPP
#define CGAL_BOOLEANOPERATIONS_HPP

#include "Shape.hpp"
#include "Mesh.hpp"

namespace CGALOperations
{

bool									MeshDifference (const Modeler::Mesh& aMesh, const Modeler::Mesh& bMesh, Modeler::Mesh& resultMesh);
bool									MeshIntersection (const Modeler::Mesh& aMesh, const Modeler::Mesh& bMesh, Modeler::Mesh& resultMesh);
bool									MeshUnion (const Modeler::Mesh& aMesh, const Modeler::Mesh& bMesh, Modeler::Mesh& resultMesh);

std::shared_ptr<Modeler::MeshShape>		ShapeDifference (const Modeler::ShapeConstPtr& aShape, const Modeler::ShapeConstPtr& bShape);
std::shared_ptr<Modeler::MeshShape>		ShapeIntersection (const Modeler::ShapeConstPtr& aShape, const Modeler::ShapeConstPtr& bShape);
std::shared_ptr<Modeler::MeshShape>		ShapeUnion (const Modeler::ShapeConstPtr& aShape, const Modeler::ShapeConstPtr& bShape);

}

#endif
