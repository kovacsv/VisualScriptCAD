#include "SimpleTest.hpp"
#include "MeshGenerators.hpp"
#include "BooleanOperations.hpp"
#include "Subdivision.hpp"
#include "Triangulation.hpp"
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
	bool opResult = MeshDifference (cube1, cube2, result);
	ASSERT (opResult == true);
}

TEST (CubeDifferenceTestWithMaterials)
{
	Mesh cube1 = GenerateBox (Material (glm::dvec3 (1.0, 0.0, 0.0)), glm::dmat4 (1.0), 1.0, 1.0, 1.0);
	Mesh cube2 = GenerateBox (Material (glm::dvec3 (0.0, 1.0, 0.0)), glm::translate (glm::dmat4 (1.0), glm::dvec3 (0.5, 0.5, 0.5)), 1.0, 1.0, 1.0);
	Mesh result;
	bool opResult = MeshDifference (cube1, cube2, result);
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
	Mesh cube2 = GenerateCylinder (DefaultMaterial, glm::translate (glm::dmat4 (1.0), glm::dvec3 (0.5, 0.5, 0.0)), 0.5, 1.0, 3, true);
	Mesh result;
	bool opResult = MeshDifference (cube1, cube2, result);
	ASSERT (opResult == false);
}

TEST (CubeSubdivisionTest)
{
	Mesh cube = GenerateBox (DefaultMaterial, glm::dmat4 (1.0), 1.0, 1.0, 1.0);
	Mesh result;
	bool opResult = MeshSubdivision (cube, DefaultMaterial, 1, result);
	ASSERT (opResult == true);
}

TEST (TriangulationTest)
{
	{
		std::vector<glm::dvec2> points = {
			glm::dvec2 (0.0, 0.0),
			glm::dvec2 (1.0, 0.0),
			glm::dvec2 (1.0, 1.0)
		};
		std::vector<std::array<size_t, 3>> result;
		bool opResult = TriangulatePolygon (points, result);
		ASSERT (opResult == true);
		ASSERT (result.size () == 1);
		ASSERT (result[0] == (std::array<size_t, 3> { 0, 1, 2 }));
	}
	{
		std::vector<glm::dvec2> points = {
			glm::dvec2 (0.0, 0.0),
			glm::dvec2 (1.0, 0.0),
			glm::dvec2 (1.0, 1.0),
			glm::dvec2 (0.0, 1.0)
		};
		std::vector<std::array<size_t, 3>> result;
		bool opResult = TriangulatePolygon (points, result);;
		ASSERT (opResult == true);
		ASSERT (result.size () == 2);
		ASSERT (result[0] == (std::array<size_t, 3> { 3, 1, 2 }));
		ASSERT (result[1] == (std::array<size_t, 3> { 3, 0, 1 }));
	}
	{
		std::vector<glm::dvec2> points = {
			glm::dvec2 (0.0, 0.0),
			glm::dvec2 (2.0, 0.0),
			glm::dvec2 (2.0, 2.0),
			glm::dvec2 (1.0, 1.0),
			glm::dvec2 (0.0, 2.0)
		};
		std::vector<std::array<size_t, 3>> result;
		bool opResult = TriangulatePolygon (points, result);;
		ASSERT (opResult == true);
		ASSERT (result.size () == 3);
		ASSERT (result[0] == (std::array<size_t, 3> { 3, 1, 2 }));
		ASSERT (result[1] == (std::array<size_t, 3> { 0, 1, 3 }));
		ASSERT (result[2] == (std::array<size_t, 3> { 4, 0, 3 }));
	}
	{
		std::vector<glm::dvec2> points = {
			glm::dvec2 (-2.0, 0.0),
			glm::dvec2 (2.0, 0.0),
			glm::dvec2 (2.0, 2.0),
			glm::dvec2 (0.0, 2.0),
			glm::dvec2 (0.0, 1.0),
			glm::dvec2 (-2.0, 1.0)
		};
		std::vector<std::array<size_t, 3>> result;
		bool opResult = TriangulatePolygon (points, result);;
		ASSERT (opResult == true);
		ASSERT (result.size () == 4);
		ASSERT (result[0] == (std::array<size_t, 3> { 3, 4, 2 }));
		ASSERT (result[1] == (std::array<size_t, 3> { 5, 0, 4 }));
		ASSERT (result[2] == (std::array<size_t, 3> { 4, 0, 1 }));
		ASSERT (result[3] == (std::array<size_t, 3> { 2, 4, 1 }));
	}
}

}
