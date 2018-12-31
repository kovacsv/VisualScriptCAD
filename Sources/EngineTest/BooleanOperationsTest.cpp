#include "SimpleTest.hpp"
#include "MeshGenerators.hpp"
#include "BooleanOperations.hpp"

using namespace Modeler;
using namespace CGALOperations;

namespace BooleanOperationsText
{
	
TEST (CubeDifferenceTest)
{
	Mesh cube1 = GenerateBox (DefaultMaterial, glm::dmat4 (1.0), 1.0, 1.0, 1.0);
	Mesh cube2 = GenerateBox (DefaultMaterial, glm::translate (glm::dmat4 (1.0), glm::dvec3 (0.5, 0.5, 0.5)), 1.0, 1.0, 1.0);
	Mesh result;
	bool opResult = MeshBooleanOperation (cube1, cube2, BooleanOperation::Difference, result);
	ASSERT (opResult == true);
}

TEST (CubeDifferenceTestWithMaterials)
{
	Mesh cube1 = GenerateBox (Material (glm::dvec3 (1.0, 0.0, 0.0)), glm::dmat4 (1.0), 1.0, 1.0, 1.0);
	Mesh cube2 = GenerateBox (Material (glm::dvec3 (0.0, 1.0, 0.0)), glm::translate (glm::dmat4 (1.0), glm::dvec3 (0.5, 0.5, 0.5)), 1.0, 1.0, 1.0);
	Mesh result;
	bool opResult = MeshBooleanOperation (cube1, cube2, BooleanOperation::Difference, result);
	ASSERT (opResult == true);

	std::unordered_set<MaterialId> foundMaterials;
	const MeshGeometry& geometry = result.GetGeometry ();
	const MeshMaterials& materials = result.GetMaterials ();
	for (unsigned int i = 0; i < geometry.TriangleCount (); i++) {
		foundMaterials.insert (materials.GetTriangleMaterial (i));
	}
	ASSERT (foundMaterials.size () == 2);
}

TEST (CubeCylinderNonManifoldDifferenceTest)
{
	Mesh cube1 = GenerateBox (DefaultMaterial, glm::dmat4 (1.0), 1.0, 1.0, 1.0);
	Mesh cube2 = GenerateCylinder (DefaultMaterial, glm::dmat4 (1.0), 0.5, 1.0, 3, true);
	Mesh result;
	bool opResult = MeshBooleanOperation (cube1, cube2, BooleanOperation::Difference, result);
	ASSERT (opResult == false);
}

}
