#ifndef CGAL_SUBDIVISION_HPP
#define CGAL_SUBDIVISION_HPP

#include "Shape.hpp"
#include "Mesh.hpp"

namespace CGALOperations
{

bool	MeshSubdivision (const Modeler::Mesh& mesh, int steps, Modeler::Mesh& resultMesh);

}

#endif
