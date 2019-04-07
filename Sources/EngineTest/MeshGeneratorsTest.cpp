#include "SimpleTest.hpp"
#include "MeshGenerators.hpp"
#include "MeshTopology.hpp"
#include "Triangulation.hpp"

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
	// TODO: PrismShellGenerator tests for all orientations

	std::vector<Mesh> meshes {
		GenerateBox (DefaultMaterial, glm::dmat4 (1.0), 1.0, 1.0, 1.0),
		GenerateCylinder (DefaultMaterial, glm::dmat4 (1.0), 1.0, 1.0, 10, false),
		GenerateCylinder (DefaultMaterial, glm::dmat4 (1.0), 1.0, 1.0, 10, true),
		GenerateTube (DefaultMaterial, glm::dmat4 (1.0), 1.0, 1.0, 0.1, 10, false),
		GenerateTube (DefaultMaterial, glm::dmat4 (1.0), 1.0, 1.0, 0.1, 10, true),
		GenerateCone (DefaultMaterial, glm::dmat4 (1.0), 1.0, 2.0, 1.0, 10, false),
		GenerateCone (DefaultMaterial, glm::dmat4 (1.0), 1.0, 2.0, 1.0, 10, true),
		GenerateCone (DefaultMaterial, glm::dmat4 (1.0), 0.0, 2.0, 1.0, 10, false),
		GenerateCone (DefaultMaterial, glm::dmat4 (1.0), 0.0, 2.0, 1.0, 10, true),
		GenerateCone (DefaultMaterial, glm::dmat4 (1.0), 1.0, 0.0, 1.0, 10, false),
		GenerateCone (DefaultMaterial, glm::dmat4 (1.0), 1.0, 0.0, 1.0, 10, true),
		GenerateSphere (DefaultMaterial, glm::dmat4 (1.0), 1.0, 10, false),
		GenerateSphere (DefaultMaterial, glm::dmat4 (1.0), 1.0, 10, true),
		GenerateTorus (DefaultMaterial, glm::dmat4 (1.0), 3.0, 0.5, 10, 10, false),
		GenerateTorus (DefaultMaterial, glm::dmat4 (1.0), 3.0, 0.5, 10, 10, true),
		GeneratePlatonicSolid (DefaultMaterial, glm::dmat4 (1.0), PlatonicSolidType::Tetrahedron, 1.0),
		GeneratePlatonicSolid (DefaultMaterial, glm::dmat4 (1.0), PlatonicSolidType::Hexahedron, 1.0),
		GeneratePlatonicSolid (DefaultMaterial, glm::dmat4 (1.0), PlatonicSolidType::Octahedron, 1.0),
		GeneratePlatonicSolid (DefaultMaterial, glm::dmat4 (1.0), PlatonicSolidType::Dodecahedron, 1.0),
		GeneratePlatonicSolid (DefaultMaterial, glm::dmat4 (1.0), PlatonicSolidType::Icosahedron, 1.0)
	};

	for (const Mesh& mesh : meshes) {
		MeshTopology topology = GetTopology (mesh);
		ASSERT (topology.IsValid ());
		ASSERT (!topology.IsEmpty ());
		ASSERT (topology.IsClosed ());
	}
}

TEST (PrismGeneratorTest)
{
	class CGALTriangulator : public Triangulator
	{
	public:
		virtual bool TriangulatePolygon (const std::vector<glm::dvec2>& points, std::vector<std::array<size_t, 3>>& result) override
		{
			return CGALOperations::TriangulatePolygon (points, result);
		}
	};

	std::vector<glm::dvec2> basePolygon = {
		glm::dvec2 (0.0, 0.0),
		glm::dvec2 (2.0, 0.0),
		glm::dvec2 (2.0, 2.0),
		glm::dvec2 (1.0, 2.0),
		glm::dvec2 (1.0, 1.0),
		glm::dvec2 (0.0, 1.0)
	};

	CGALTriangulator triangulator;
	Mesh mesh = GeneratePrism (DefaultMaterial, glm::dmat4 (1.0), basePolygon, 1.0, triangulator);

	MeshTopology topology = GetTopology (mesh);
	ASSERT (topology.IsValid ());
	ASSERT (!topology.IsEmpty ());
	ASSERT (topology.IsClosed ());
}

}
