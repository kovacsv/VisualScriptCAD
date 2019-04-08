#include "SimpleTest.hpp"
#include "MeshGenerators.hpp"
#include "MeshTopology.hpp"
#include "Triangulation.hpp"
#include "TestUtils.hpp"

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
		GenerateBoxShell (DefaultMaterial, glm::dmat4 (1.0), 1.0, 1.0, 1.0, 0.1),
		GenerateCylinder (DefaultMaterial, glm::dmat4 (1.0), 1.0, 1.0, 10, false),
		GenerateCylinder (DefaultMaterial, glm::dmat4 (1.0), 1.0, 1.0, 10, true),
		GenerateCylinderShell (DefaultMaterial, glm::dmat4 (1.0), 1.0, 1.0, 10, false, 0.1),
		GenerateCylinderShell (DefaultMaterial, glm::dmat4 (1.0), 1.0, 1.0, 10, true, 0.1),
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

TEST (PrismShellGeneratorTest)
{
	PrismShellGenerator generator (DefaultMaterial, glm::dmat4 (1.0), 1.0, 0.1);
	generator.AddVertex (glm::dvec2 (0.0, 0.0), PolygonalGenerator::VertexType::Sharp);
	generator.AddVertex (glm::dvec2 (1.0, 0.0), PolygonalGenerator::VertexType::Sharp);
	generator.AddVertex (glm::dvec2 (2.0, 0.0), PolygonalGenerator::VertexType::Sharp);
	generator.AddVertex (glm::dvec2 (2.0, 2.0), PolygonalGenerator::VertexType::Sharp);
	generator.AddVertex (glm::dvec2 (1.0, 1.0), PolygonalGenerator::VertexType::Sharp);
	generator.AddVertex (glm::dvec2 (0.0, 2.0), PolygonalGenerator::VertexType::Sharp);
	Mesh mesh = generator.Generate ();

	MeshTopology topology = GetTopology (mesh);
	ASSERT (topology.IsValid ());
	ASSERT (!topology.IsEmpty ());
	ASSERT (topology.IsClosed ());

	Model model;
	model.AddMesh (mesh);

	ModelWriterForTest writer;
	ExportModelToObj (model, L"model", writer);
	std::wstring expected;
	expected += L"# open file model.mtl\n";
	expected += L"newmtl Material0\n";
	expected += L"Kd 0 0.5 0.7\n";
	expected += L"# close file\n";
	expected += L"# open file model.obj\n";
	expected += L"mtllib model.mtl\n";
	expected += L"g Mesh1\n";
	expected += L"v 0 0 0\n";
	expected += L"v 1 0 0\n";
	expected += L"v 2 0 0\n";
	expected += L"v 2 2 0\n";
	expected += L"v 1 1 0\n";
	expected += L"v 0 2 0\n";
	expected += L"v 0 0 1\n";
	expected += L"v 1 0 1\n";
	expected += L"v 2 0 1\n";
	expected += L"v 2 2 1\n";
	expected += L"v 1 1 1\n";
	expected += L"v 0 2 1\n";
	expected += L"v 0.1 0.1 0\n";
	expected += L"v 1 0.1 0\n";
	expected += L"v 1.9 0.1 0\n";
	expected += L"v 1.9 1.75858 0\n";
	expected += L"v 1 0.858579 0\n";
	expected += L"v 0.1 1.75858 0\n";
	expected += L"v 0.1 0.1 1\n";
	expected += L"v 1 0.1 1\n";
	expected += L"v 1.9 0.1 1\n";
	expected += L"v 1.9 1.75858 1\n";
	expected += L"v 1 0.858579 1\n";
	expected += L"v 0.1 1.75858 1\n";
	expected += L"vn 0 0 -1\n";
	expected += L"vn 0 0 1\n";
	expected += L"vn 0 -1 0\n";
	expected += L"vn 0 -1 0\n";
	expected += L"vn 1 0 0\n";
	expected += L"vn -0.707107 0.707107 0\n";
	expected += L"vn 0.707107 0.707107 0\n";
	expected += L"vn -1 0 0\n";
	expected += L"vn 0 1 0\n";
	expected += L"vn 0 1 0\n";
	expected += L"vn -1 -0 -0\n";
	expected += L"vn 0.707107 -0.707107 0\n";
	expected += L"vn -0.707107 -0.707107 -0\n";
	expected += L"vn 1 0 0\n";
	expected += L"usemtl Material0\n";
	expected += L"f 1//1 14//1 2//1\n";
	expected += L"f 1//1 13//1 14//1\n";
	expected += L"f 2//1 15//1 3//1\n";
	expected += L"f 2//1 14//1 15//1\n";
	expected += L"f 3//1 16//1 4//1\n";
	expected += L"f 3//1 15//1 16//1\n";
	expected += L"f 4//1 17//1 5//1\n";
	expected += L"f 4//1 16//1 17//1\n";
	expected += L"f 5//1 18//1 6//1\n";
	expected += L"f 5//1 17//1 18//1\n";
	expected += L"f 6//1 13//1 1//1\n";
	expected += L"f 6//1 18//1 13//1\n";
	expected += L"f 7//2 8//2 20//2\n";
	expected += L"f 7//2 20//2 19//2\n";
	expected += L"f 8//2 9//2 21//2\n";
	expected += L"f 8//2 21//2 20//2\n";
	expected += L"f 9//2 10//2 22//2\n";
	expected += L"f 9//2 22//2 21//2\n";
	expected += L"f 10//2 11//2 23//2\n";
	expected += L"f 10//2 23//2 22//2\n";
	expected += L"f 11//2 12//2 24//2\n";
	expected += L"f 11//2 24//2 23//2\n";
	expected += L"f 12//2 7//2 19//2\n";
	expected += L"f 12//2 19//2 24//2\n";
	expected += L"f 1//3 2//3 8//3\n";
	expected += L"f 1//3 8//3 7//3\n";
	expected += L"f 2//4 3//4 9//4\n";
	expected += L"f 2//4 9//4 8//4\n";
	expected += L"f 3//5 4//5 10//5\n";
	expected += L"f 3//5 10//5 9//5\n";
	expected += L"f 4//6 5//6 11//6\n";
	expected += L"f 4//6 11//6 10//6\n";
	expected += L"f 5//7 6//7 12//7\n";
	expected += L"f 5//7 12//7 11//7\n";
	expected += L"f 6//8 1//8 7//8\n";
	expected += L"f 6//8 7//8 12//8\n";
	expected += L"f 13//9 20//9 14//9\n";
	expected += L"f 13//9 19//9 20//9\n";
	expected += L"f 14//10 21//10 15//10\n";
	expected += L"f 14//10 20//10 21//10\n";
	expected += L"f 15//11 22//11 16//11\n";
	expected += L"f 15//11 21//11 22//11\n";
	expected += L"f 16//12 23//12 17//12\n";
	expected += L"f 16//12 22//12 23//12\n";
	expected += L"f 17//13 24//13 18//13\n";
	expected += L"f 17//13 23//13 24//13\n";
	expected += L"f 18//14 19//14 13//14\n";
	expected += L"f 18//14 24//14 19//14\n";
	expected += L"# close file\n";

	ASSERT (CheckString (expected, writer.result));
}

}
