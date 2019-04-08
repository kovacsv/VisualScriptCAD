#include "SimpleTest.hpp"
#include "Model.hpp"
#include "MeshGenerators.hpp"
#include "Export.hpp"
#include "TestUtils.hpp"

using namespace Modeler;

namespace ExportTest
{

TEST (Obj_OneMeshExportTest)
{
	Model model;
	model.AddMesh (GenerateBox (Material (glm::dvec3 (1.0, 0.0, 0.0)), glm::dmat4 (1.0), 1.0, 1.0, 1.0));
	
	ModelWriterForTest writer;
	ExportModelToObj (model, L"model", writer);
	std::wstring expected;
	expected += L"# open file model.mtl\n";
	expected += L"newmtl Material0\n";
	expected += L"Kd 1 0 0\n";
	expected += L"# close file\n";
	expected += L"# open file model.obj\n";
	expected += L"mtllib model.mtl\n";
	expected += L"g Mesh1\n";
	expected += L"v 0 0 0\n";
	expected += L"v 1 0 0\n";
	expected += L"v 1 1 0\n";
	expected += L"v 0 1 0\n";
	expected += L"v 0 0 1\n";
	expected += L"v 1 0 1\n";
	expected += L"v 1 1 1\n";
	expected += L"v 0 1 1\n";
	expected += L"vn 0 0 -1\n";
	expected += L"vn 0 0 1\n";
	expected += L"vn 0 -1 0\n";
	expected += L"vn 1 0 0\n";
	expected += L"vn 0 1 0\n";
	expected += L"vn -1 0 0\n";
	expected += L"usemtl Material0\n";
	expected += L"f 1//1 3//1 2//1\n";
	expected += L"f 1//1 4//1 3//1\n";
	expected += L"f 5//2 6//2 7//2\n";
	expected += L"f 5//2 7//2 8//2\n";
	expected += L"f 1//3 2//3 6//3\n";
	expected += L"f 1//3 6//3 5//3\n";
	expected += L"f 2//4 3//4 7//4\n";
	expected += L"f 2//4 7//4 6//4\n";
	expected += L"f 3//5 4//5 8//5\n";
	expected += L"f 3//5 8//5 7//5\n";
	expected += L"f 4//6 1//6 5//6\n";
	expected += L"f 4//6 5//6 8//6\n";
	expected += L"# close file\n";

	ASSERT (CheckString (expected, writer.result));
}

TEST (Obj_TwoMeshExportTest)
{
	Model model;

	model.AddMesh (GenerateBox (Material (glm::dvec3 (1.0, 0.0, 0.0)), glm::dmat4 (1.0), 1.0, 1.0, 1.0));
	model.AddMesh (GenerateBox (Material (glm::dvec3 (0.0, 1.0, 0.0)), glm::translate (glm::dmat4 (1.0), glm::dvec3 (2.0, 0.0, 0.0)), 1.0, 1.0, 1.0));

	ModelWriterForTest writer;
	ExportModelToObj (model, L"model", writer);
	std::wstring expected;
	expected += L"# open file model.mtl\n";
	expected += L"newmtl Material0\n";
	expected += L"Kd 1 0 0\n";
	expected += L"newmtl Material1\n";
	expected += L"Kd 0 1 0\n";
	expected += L"# close file\n";
	expected += L"# open file model.obj\n";
	expected += L"mtllib model.mtl\n";
	expected += L"g Mesh1\n";
	expected += L"v 0 0 0\n";
	expected += L"v 1 0 0\n";
	expected += L"v 1 1 0\n";
	expected += L"v 0 1 0\n";
	expected += L"v 0 0 1\n";
	expected += L"v 1 0 1\n";
	expected += L"v 1 1 1\n";
	expected += L"v 0 1 1\n";
	expected += L"vn 0 0 -1\n";
	expected += L"vn 0 0 1\n";
	expected += L"vn 0 -1 0\n";
	expected += L"vn 1 0 0\n";
	expected += L"vn 0 1 0\n";
	expected += L"vn -1 0 0\n";
	expected += L"usemtl Material0\n";
	expected += L"f 1//1 3//1 2//1\n";
	expected += L"f 1//1 4//1 3//1\n";
	expected += L"f 5//2 6//2 7//2\n";
	expected += L"f 5//2 7//2 8//2\n";
	expected += L"f 1//3 2//3 6//3\n";
	expected += L"f 1//3 6//3 5//3\n";
	expected += L"f 2//4 3//4 7//4\n";
	expected += L"f 2//4 7//4 6//4\n";
	expected += L"f 3//5 4//5 8//5\n";
	expected += L"f 3//5 8//5 7//5\n";
	expected += L"f 4//6 1//6 5//6\n";
	expected += L"f 4//6 5//6 8//6\n";
	expected += L"g Mesh2\n";
	expected += L"v 2 0 0\n";
	expected += L"v 3 0 0\n";
	expected += L"v 3 1 0\n";
	expected += L"v 2 1 0\n";
	expected += L"v 2 0 1\n";
	expected += L"v 3 0 1\n";
	expected += L"v 3 1 1\n";
	expected += L"v 2 1 1\n";
	expected += L"vn 0 0 -1\n";
	expected += L"vn 0 0 1\n";
	expected += L"vn 0 -1 0\n";
	expected += L"vn 1 0 0\n";
	expected += L"vn 0 1 0\n";
	expected += L"vn -1 0 0\n";
	expected += L"usemtl Material1\n";
	expected += L"f 9//7 11//7 10//7\n";
	expected += L"f 9//7 12//7 11//7\n";
	expected += L"f 13//8 14//8 15//8\n";
	expected += L"f 13//8 15//8 16//8\n";
	expected += L"f 9//9 10//9 14//9\n";
	expected += L"f 9//9 14//9 13//9\n";
	expected += L"f 10//10 11//10 15//10\n";
	expected += L"f 10//10 15//10 14//10\n";
	expected += L"f 11//11 12//11 16//11\n";
	expected += L"f 11//11 16//11 15//11\n";
	expected += L"f 12//12 9//12 13//12\n";
	expected += L"f 12//12 13//12 16//12\n";
	expected += L"# close file\n";

	ASSERT (CheckString (expected, writer.result));
}

TEST (Obj_TwoMaterialsInOneMeshExportTest)
{
	Model model;

	Mesh mesh;
	MaterialId material1 = mesh.AddMaterial (Material (glm::dvec3 (1.0, 0.0, 0.0)));
	MaterialId material2 = mesh.AddMaterial (Material (glm::dvec3 (0.0, 1.0, 0.0)));
	mesh.AddVertex (glm::dvec3 (0.0, 0.0, 0.0));
	mesh.AddVertex (glm::dvec3 (1.0, 0.0, 0.0));
	mesh.AddVertex (glm::dvec3 (1.0, 1.0, 0.0));
	mesh.AddVertex (glm::dvec3 (0.0, 0.0, 1.0));
	mesh.AddVertex (glm::dvec3 (1.0, 0.0, 1.0));
	mesh.AddVertex (glm::dvec3 (1.0, 1.0, 1.0));
	mesh.AddTriangle (0, 1, 2, material1);
	mesh.AddTriangle (3, 4, 5, material2);
	model.AddMesh (mesh);

	ModelWriterForTest writer;
	ExportModelToObj (model, L"model", writer);
	std::wstring expected;
	expected += L"# open file model.mtl\n";
	expected += L"newmtl Material0\n";
	expected += L"Kd 1 0 0\n";
	expected += L"newmtl Material1\n";
	expected += L"Kd 0 1 0\n";
	expected += L"# close file\n";
	expected += L"# open file model.obj\n";
	expected += L"mtllib model.mtl\n";
	expected += L"g Mesh1\n";
	expected += L"v 0 0 0\n";
	expected += L"v 1 0 0\n";
	expected += L"v 1 1 0\n";
	expected += L"v 0 0 1\n";
	expected += L"v 1 0 1\n";
	expected += L"v 1 1 1\n";
	expected += L"vn 0 0 1\n";
	expected += L"vn 0 0 1\n";
	expected += L"usemtl Material0\n";
	expected += L"f 1//1 2//1 3//1\n";
	expected += L"usemtl Material1\n";
	expected += L"f 4//2 5//2 6//2\n";
	expected += L"# close file\n";

	ASSERT (CheckString (expected, writer.result));
}

TEST (Stl_TwoMeshExportTest)
{
	Model model;

	model.AddMesh (GenerateBox (Material (glm::dvec3 (1.0, 0.0, 0.0)), glm::dmat4 (1.0), 1.0, 1.0, 1.0));
	model.AddMesh (GenerateBox (Material (glm::dvec3 (0.0, 1.0, 0.0)), glm::translate (glm::dmat4 (1.0), glm::dvec3 (2.0, 0.0, 0.0)), 1.0, 1.0, 1.0));

	ModelWriterForTest writer;
	ExportModelToStl (model, L"model", writer);
	std::wstring expected;
	expected += L"# open file model.stl\n";
	expected += L"solid model\n";
	expected += L"facet normal -0 0 -1\n";
	expected += L"    outer loop\n";
	expected += L"        vertex 0 0 0\n";
	expected += L"        vertex 1 1 0\n";
	expected += L"        vertex 1 0 0\n";
	expected += L"    endloop\n";
	expected += L"endfacet\n";
	expected += L"facet normal 0 -0 -1\n";
	expected += L"    outer loop\n";
	expected += L"        vertex 0 0 0\n";
	expected += L"        vertex 0 1 0\n";
	expected += L"        vertex 1 1 0\n";
	expected += L"    endloop\n";
	expected += L"endfacet\n";
	expected += L"facet normal 0 0 1\n";
	expected += L"    outer loop\n";
	expected += L"        vertex 0 0 1\n";
	expected += L"        vertex 1 0 1\n";
	expected += L"        vertex 1 1 1\n";
	expected += L"    endloop\n";
	expected += L"endfacet\n";
	expected += L"facet normal 0 0 1\n";
	expected += L"    outer loop\n";
	expected += L"        vertex 0 0 1\n";
	expected += L"        vertex 1 1 1\n";
	expected += L"        vertex 0 1 1\n";
	expected += L"    endloop\n";
	expected += L"endfacet\n";
	expected += L"facet normal -0 -1 0\n";
	expected += L"    outer loop\n";
	expected += L"        vertex 0 0 0\n";
	expected += L"        vertex 1 0 0\n";
	expected += L"        vertex 1 0 1\n";
	expected += L"    endloop\n";
	expected += L"endfacet\n";
	expected += L"facet normal 0 -1 -0\n";
	expected += L"    outer loop\n";
	expected += L"        vertex 0 0 0\n";
	expected += L"        vertex 1 0 1\n";
	expected += L"        vertex 0 0 1\n";
	expected += L"    endloop\n";
	expected += L"endfacet\n";
	expected += L"facet normal 1 0 0\n";
	expected += L"    outer loop\n";
	expected += L"        vertex 1 0 0\n";
	expected += L"        vertex 1 1 0\n";
	expected += L"        vertex 1 1 1\n";
	expected += L"    endloop\n";
	expected += L"endfacet\n";
	expected += L"facet normal 1 0 0\n";
	expected += L"    outer loop\n";
	expected += L"        vertex 1 0 0\n";
	expected += L"        vertex 1 1 1\n";
	expected += L"        vertex 1 0 1\n";
	expected += L"    endloop\n";
	expected += L"endfacet\n";
	expected += L"facet normal -0 1 0\n";
	expected += L"    outer loop\n";
	expected += L"        vertex 1 1 0\n";
	expected += L"        vertex 0 1 0\n";
	expected += L"        vertex 0 1 1\n";
	expected += L"    endloop\n";
	expected += L"endfacet\n";
	expected += L"facet normal 0 1 0\n";
	expected += L"    outer loop\n";
	expected += L"        vertex 1 1 0\n";
	expected += L"        vertex 0 1 1\n";
	expected += L"        vertex 1 1 1\n";
	expected += L"    endloop\n";
	expected += L"endfacet\n";
	expected += L"facet normal -1 0 0\n";
	expected += L"    outer loop\n";
	expected += L"        vertex 0 1 0\n";
	expected += L"        vertex 0 0 0\n";
	expected += L"        vertex 0 0 1\n";
	expected += L"    endloop\n";
	expected += L"endfacet\n";
	expected += L"facet normal -1 0 0\n";
	expected += L"    outer loop\n";
	expected += L"        vertex 0 1 0\n";
	expected += L"        vertex 0 0 1\n";
	expected += L"        vertex 0 1 1\n";
	expected += L"    endloop\n";
	expected += L"endfacet\n";
	expected += L"facet normal -0 0 -1\n";
	expected += L"    outer loop\n";
	expected += L"        vertex 2 0 0\n";
	expected += L"        vertex 3 1 0\n";
	expected += L"        vertex 3 0 0\n";
	expected += L"    endloop\n";
	expected += L"endfacet\n";
	expected += L"facet normal 0 -0 -1\n";
	expected += L"    outer loop\n";
	expected += L"        vertex 2 0 0\n";
	expected += L"        vertex 2 1 0\n";
	expected += L"        vertex 3 1 0\n";
	expected += L"    endloop\n";
	expected += L"endfacet\n";
	expected += L"facet normal 0 0 1\n";
	expected += L"    outer loop\n";
	expected += L"        vertex 2 0 1\n";
	expected += L"        vertex 3 0 1\n";
	expected += L"        vertex 3 1 1\n";
	expected += L"    endloop\n";
	expected += L"endfacet\n";
	expected += L"facet normal 0 0 1\n";
	expected += L"    outer loop\n";
	expected += L"        vertex 2 0 1\n";
	expected += L"        vertex 3 1 1\n";
	expected += L"        vertex 2 1 1\n";
	expected += L"    endloop\n";
	expected += L"endfacet\n";
	expected += L"facet normal -0 -1 0\n";
	expected += L"    outer loop\n";
	expected += L"        vertex 2 0 0\n";
	expected += L"        vertex 3 0 0\n";
	expected += L"        vertex 3 0 1\n";
	expected += L"    endloop\n";
	expected += L"endfacet\n";
	expected += L"facet normal 0 -1 -0\n";
	expected += L"    outer loop\n";
	expected += L"        vertex 2 0 0\n";
	expected += L"        vertex 3 0 1\n";
	expected += L"        vertex 2 0 1\n";
	expected += L"    endloop\n";
	expected += L"endfacet\n";
	expected += L"facet normal 1 0 0\n";
	expected += L"    outer loop\n";
	expected += L"        vertex 3 0 0\n";
	expected += L"        vertex 3 1 0\n";
	expected += L"        vertex 3 1 1\n";
	expected += L"    endloop\n";
	expected += L"endfacet\n";
	expected += L"facet normal 1 0 0\n";
	expected += L"    outer loop\n";
	expected += L"        vertex 3 0 0\n";
	expected += L"        vertex 3 1 1\n";
	expected += L"        vertex 3 0 1\n";
	expected += L"    endloop\n";
	expected += L"endfacet\n";
	expected += L"facet normal -0 1 0\n";
	expected += L"    outer loop\n";
	expected += L"        vertex 3 1 0\n";
	expected += L"        vertex 2 1 0\n";
	expected += L"        vertex 2 1 1\n";
	expected += L"    endloop\n";
	expected += L"endfacet\n";
	expected += L"facet normal 0 1 0\n";
	expected += L"    outer loop\n";
	expected += L"        vertex 3 1 0\n";
	expected += L"        vertex 2 1 1\n";
	expected += L"        vertex 3 1 1\n";
	expected += L"    endloop\n";
	expected += L"endfacet\n";
	expected += L"facet normal -1 0 0\n";
	expected += L"    outer loop\n";
	expected += L"        vertex 2 1 0\n";
	expected += L"        vertex 2 0 0\n";
	expected += L"        vertex 2 0 1\n";
	expected += L"    endloop\n";
	expected += L"endfacet\n";
	expected += L"facet normal -1 0 0\n";
	expected += L"    outer loop\n";
	expected += L"        vertex 2 1 0\n";
	expected += L"        vertex 2 0 1\n";
	expected += L"        vertex 2 1 1\n";
	expected += L"    endloop\n";
	expected += L"endfacet\n";
	expected += L"endsolid model\n";
	expected += L"# close file\n";

	ASSERT (CheckString (expected, writer.result));
}

}
