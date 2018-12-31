#ifndef CGAL_BOOLEANOPERATIONS_HPP
#define CGAL_BOOLEANOPERATIONS_HPP

#include "Shape.hpp"
#include "Mesh.hpp"

namespace CGALOperations
{

enum class BooleanOperation
{
	Difference,
	Intersection,
	Union
};

bool									MeshBooleanOperation (const Modeler::Mesh& aMesh, const Modeler::Mesh& bMesh, BooleanOperation operation, Modeler::Mesh& resultMesh);
std::shared_ptr<Modeler::MeshShape>		MeshBooleanOperation (const Modeler::ShapeConstPtr& aShape, const Modeler::ShapeConstPtr& bShape, BooleanOperation operation);

}

#endif
