#include "SimpleTest.hpp"
#include "MeshGenerators.hpp"
#include "BooleanOperations.hpp"
#include "Subdivision.hpp"
#include "Export.hpp"

#ifdef _WIN32
#include <windows.h>
#endif

using namespace Modeler;
using namespace CGALOperations;

namespace CGALOperationsTest
{

/*
class DebugModelDumper : public ModelWriter
{
public:
	DebugModelDumper ()
	{

	}

	virtual void OpenFile (const std::wstring&) override
	{

	}

	virtual void CloseFile () override
	{

	}

	virtual void WriteLine (const std::wstring& text) override
	{
#ifdef _WIN32
		OutputDebugString (text.c_str ());
		OutputDebugString (L"\n");
#else
		(void) text;
#endif
	}
};
*/


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

TEST (CubeSubdivisionTest)
{
	Mesh cube = GenerateBox (DefaultMaterial, glm::dmat4 (1.0), 1.0, 1.0, 1.0);
	Mesh result;
	bool opResult = MeshSubdivision (cube, 1, result);
	ASSERT (opResult == true);
}

}
