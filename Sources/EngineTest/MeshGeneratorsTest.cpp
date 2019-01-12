#include "SimpleTest.hpp"
#include "MeshGenerators.hpp"
#include "MeshTopology.hpp"

using namespace Modeler;

MeshTopology GetTopology (const Mesh& mesh)
{
	const MeshGeometry& geometry = mesh.GetGeometry ();
	MeshTopology result;
	MeshTopologyBuilder builder (result);
	geometry.EnumerateTriangles ([&] (const MeshTriangle& triangle) {
		builder.AddTriangle (triangle.v1, triangle.v2, triangle.v3);
	});
	return result;
}

namespace MeshGeneratorsTest
{

TEST (MeshGeneratorsTest)
{
	std::vector<Mesh> meshes {
		GenerateBox (DefaultMaterial, glm::dmat4 (1.0), 1.0, 1.0, 1.0),
		GenerateCylinder (DefaultMaterial, glm::dmat4 (1.0), 1.0, 1.0, 10, false),
		GenerateCone (DefaultMaterial, glm::dmat4 (1.0), 1.0, 2.0, 1.0, 10, false),
		GenerateCone (DefaultMaterial, glm::dmat4 (1.0), 0.0, 2.0, 1.0, 10, false),
		GenerateCone (DefaultMaterial, glm::dmat4 (1.0), 1.0, 0.0, 1.0, 10, false),
		GenerateSphere (DefaultMaterial, glm::dmat4 (1.0), 1.0, 10, false),
		GenerateTorus (DefaultMaterial, glm::dmat4 (1.0), 3.0, 0.5, 10, 10, false)
	};

	for (const Mesh& mesh : meshes) {
		MeshTopology topology = GetTopology (mesh);
		ASSERT (topology.IsValid ());
		ASSERT (!topology.IsEmpty ());
		ASSERT (topology.IsClosed ());
	}
}

}
