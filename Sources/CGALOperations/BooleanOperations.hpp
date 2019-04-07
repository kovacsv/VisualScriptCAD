#ifndef CGAL_BOOLEANOPERATIONS_HPP
#define CGAL_BOOLEANOPERATIONS_HPP

#include "Shapes.hpp"
#include "Mesh.hpp"

#include <vector>

namespace CGALOperations
{

bool					MeshDifference (const Modeler::Mesh& aMesh, const Modeler::Mesh& bMesh, Modeler::Mesh& resultMesh);
bool					MeshIntersection (const Modeler::Mesh& aMesh, const Modeler::Mesh& bMesh, Modeler::Mesh& resultMesh);
bool					MeshUnion (const Modeler::Mesh& aMesh, const Modeler::Mesh& bMesh, Modeler::Mesh& resultMesh);
bool					MeshUnion (const std::vector<Modeler::Mesh>& meshes, Modeler::Mesh& resultMesh);

Modeler::ShapePtr		ShapeDifference (const Modeler::ShapeConstPtr& aShape, const Modeler::ShapeConstPtr& bShape);
Modeler::ShapePtr		ShapeIntersection (const Modeler::ShapeConstPtr& aShape, const Modeler::ShapeConstPtr& bShape);
Modeler::ShapePtr		ShapeUnion (const Modeler::ShapeConstPtr& aShape, const Modeler::ShapeConstPtr& bShape);
Modeler::ShapePtr		ShapeUnion (const std::vector<Modeler::ShapeConstPtr>& shapes);

}

#endif
