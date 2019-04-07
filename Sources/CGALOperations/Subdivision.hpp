#ifndef CGAL_SUBDIVISION_HPP
#define CGAL_SUBDIVISION_HPP

#include "Shapes.hpp"
#include "Mesh.hpp"

namespace CGALOperations
{

bool									MeshSubdivision (const Modeler::Mesh& mesh, const Modeler::Material& material, int steps, Modeler::Mesh& resultMesh);
std::shared_ptr<Modeler::MeshShape>		MeshSubdivision (const Modeler::ShapeConstPtr& shape, const Modeler::Material& material, int steps);

}

#endif
