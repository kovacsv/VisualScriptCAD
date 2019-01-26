#include "Subdivision.hpp"
#include "TriangleUtils.hpp"
#include "IncludeGLM.hpp"

#pragma warning (push)
#pragma warning (disable : 4456)
#pragma warning (disable : 4457)
#pragma warning (disable : 4458)
#pragma warning (disable : 4702)
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/subdivision_method_3.h>
#pragma warning (pop)

typedef CGAL::Simple_cartesian<double>	CGAL_Kernel;
typedef CGAL_Kernel::Point_3			CGAL_Point;
typedef CGAL::Surface_mesh<CGAL_Point>	CGAL_Mesh;

namespace CGALOperations
{

static void ConvertMeshToCGALMesh (const Modeler::Mesh& mesh, CGAL_Mesh& cgalMesh)
{
	const Modeler::MeshGeometry& geometry = mesh.GetGeometry ();
	const glm::dmat4& transformation = mesh.GetTransformation ();
	geometry.EnumerateVertices (transformation, [&] (const glm::dvec3& vertex) {
		cgalMesh.add_vertex (CGAL_Point (vertex.x, vertex.y, vertex.z));
	});
	for (unsigned int triangleIndex = 0; triangleIndex < geometry.TriangleCount (); ++triangleIndex) {
		const Modeler::MeshTriangle& triangle = geometry.GetTriangle (triangleIndex);
		CGAL_Mesh::Face_index faceIndex = cgalMesh.add_face (
			CGAL_Mesh::Vertex_index (triangle.v1),
			CGAL_Mesh::Vertex_index (triangle.v2),
			CGAL_Mesh::Vertex_index (triangle.v3)
		);
	}
}

static void ConvertCGALMeshToMesh (const CGAL_Mesh& cgalMesh, const Modeler::Material& material, Modeler::Mesh& mesh) 
{
	Modeler::MaterialId materialId = mesh.AddMaterial (material);
	for (CGAL_Mesh::Vertex_index vertIndex : cgalMesh.vertices ()) {
		const CGAL_Point& point = cgalMesh.point (vertIndex);
		mesh.AddVertex (CGAL::to_double (point.x ()), CGAL::to_double (point.y ()), CGAL::to_double (point.z ()));
	}

	for (CGAL_Mesh::Face_index faceIndex : cgalMesh.faces ()) {
		CGAL_Mesh::Halfedge_index halfEdge = cgalMesh.halfedge (faceIndex);
		std::vector<CGAL_Mesh::Vertex_index> vertices;
		for (auto halfEdgeIndex : halfedges_around_face (halfEdge, cgalMesh)) {
			vertices.push_back (target (halfEdgeIndex, cgalMesh));
		}
		if (vertices.size () != 3) {
			throw std::logic_error ("a non-triangle face found");
		}
		mesh.AddTriangle ((unsigned int) vertices[0], (unsigned int) vertices[1], (unsigned int) vertices[2], materialId);
	}
}

bool MeshSubdivision (const Modeler::Mesh& mesh, const Modeler::Material& material, int steps, Modeler::Mesh& resultMesh)
{
	bool success = true;
	try {
		CGAL_Mesh cgalMesh;
		ConvertMeshToCGALMesh (mesh, cgalMesh);
		CGAL::Subdivision_method_3::Loop_subdivision (cgalMesh, steps);
		ConvertCGALMeshToMesh (cgalMesh, material, resultMesh);
	} catch (...) {
		success = false;
	}

	return success;
}

std::shared_ptr<Modeler::MeshShape> MeshSubdivision (const Modeler::ShapeConstPtr& shape, const Modeler::Material& material, int steps)
{
	Modeler::Mesh mesh = shape->GenerateMesh ();
	Modeler::Mesh resultMesh;
	bool opResult = MeshSubdivision (mesh, material, steps, resultMesh);
	if (!opResult) {
		return nullptr;
	}
	return std::shared_ptr<Modeler::MeshShape> (new Modeler::MeshShape (glm::dmat4 (1.0), resultMesh));
}

}
