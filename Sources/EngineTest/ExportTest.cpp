#include "SimpleTest.hpp"
#include "Model.hpp"
#include "MeshGenerators.hpp"
#include "Export.hpp"

using namespace Modeler;

class ModelWriterForTest : public ModelWriter
{
public:
	virtual void OpenFile (const std::wstring& fileName) override
	{
		result += L"# open file " + fileName + L"\n";
	}

	virtual void CloseFile () override
	{
		result += L"# close file\n";
	}

	virtual void WriteLine (const std::wstring& text) override
	{
		result += text + L"\n";
	}

	std::wstring result;
};

namespace ExportTest
{

static bool CheckResult (const std::wstring& expected, const std::wstring& result)
{
	if (result == expected) {
		return true;
	} else {
		std::wcout << std::endl << expected;
		std::wcout << std::endl << result << std::endl;
		return false;
	}
}

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
	expected += L"v -0.5 -0.5 -0.5\n";
	expected += L"v 0.5 -0.5 -0.5\n";
	expected += L"v 0.5 -0.5 0.5\n";
	expected += L"v -0.5 -0.5 0.5\n";
	expected += L"v -0.5 0.5 -0.5\n";
	expected += L"v 0.5 0.5 -0.5\n";
	expected += L"v 0.5 0.5 0.5\n";
	expected += L"v -0.5 0.5 0.5\n";
	expected += L"vn 0 -1 0\n";
	expected += L"vn 0 -1 0\n";
	expected += L"vn 1 0 0\n";
	expected += L"vn 1 0 0\n";
	expected += L"vn 0 1 0\n";
	expected += L"vn 0 1 0\n";
	expected += L"vn -1 0 0\n";
	expected += L"vn -1 0 0\n";
	expected += L"vn 0 0 -1\n";
	expected += L"vn 0 0 -1\n";
	expected += L"vn 0 0 1\n";
	expected += L"vn 0 0 1\n";
	expected += L"usemtl Material0\n";
	expected += L"f 1//1 2//1 3//1\n";
	expected += L"f 1//2 3//2 4//2\n";
	expected += L"f 2//3 6//3 7//3\n";
	expected += L"f 2//4 7//4 3//4\n";
	expected += L"f 6//5 5//5 8//5\n";
	expected += L"f 6//6 8//6 7//6\n";
	expected += L"f 5//7 1//7 4//7\n";
	expected += L"f 5//8 4//8 8//8\n";
	expected += L"f 1//9 5//9 6//9\n";
	expected += L"f 1//10 6//10 2//10\n";
	expected += L"f 4//11 3//11 7//11\n";
	expected += L"f 4//12 7//12 8//12\n";
	expected += L"# close file\n";

	ASSERT (CheckResult (expected, writer.result));
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
	expected += L"v -0.5 -0.5 -0.5\n";
	expected += L"v 0.5 -0.5 -0.5\n";
	expected += L"v 0.5 -0.5 0.5\n";
	expected += L"v -0.5 -0.5 0.5\n";
	expected += L"v -0.5 0.5 -0.5\n";
	expected += L"v 0.5 0.5 -0.5\n";
	expected += L"v 0.5 0.5 0.5\n";
	expected += L"v -0.5 0.5 0.5\n";
	expected += L"vn 0 -1 0\n";
	expected += L"vn 0 -1 0\n";
	expected += L"vn 1 0 0\n";
	expected += L"vn 1 0 0\n";
	expected += L"vn 0 1 0\n";
	expected += L"vn 0 1 0\n";
	expected += L"vn -1 0 0\n";
	expected += L"vn -1 0 0\n";
	expected += L"vn 0 0 -1\n";
	expected += L"vn 0 0 -1\n";
	expected += L"vn 0 0 1\n";
	expected += L"vn 0 0 1\n";
	expected += L"usemtl Material0\n";
	expected += L"f 1//1 2//1 3//1\n";
	expected += L"f 1//2 3//2 4//2\n";
	expected += L"f 2//3 6//3 7//3\n";
	expected += L"f 2//4 7//4 3//4\n";
	expected += L"f 6//5 5//5 8//5\n";
	expected += L"f 6//6 8//6 7//6\n";
	expected += L"f 5//7 1//7 4//7\n";
	expected += L"f 5//8 4//8 8//8\n";
	expected += L"f 1//9 5//9 6//9\n";
	expected += L"f 1//10 6//10 2//10\n";
	expected += L"f 4//11 3//11 7//11\n";
	expected += L"f 4//12 7//12 8//12\n";
	expected += L"g Mesh2\n";
	expected += L"v 1.5 -0.5 -0.5\n";
	expected += L"v 2.5 -0.5 -0.5\n";
	expected += L"v 2.5 -0.5 0.5\n";
	expected += L"v 1.5 -0.5 0.5\n";
	expected += L"v 1.5 0.5 -0.5\n";
	expected += L"v 2.5 0.5 -0.5\n";
	expected += L"v 2.5 0.5 0.5\n";
	expected += L"v 1.5 0.5 0.5\n";
	expected += L"vn 0 -1 0\n";
	expected += L"vn 0 -1 0\n";
	expected += L"vn 1 0 0\n";
	expected += L"vn 1 0 0\n";
	expected += L"vn 0 1 0\n";
	expected += L"vn 0 1 0\n";
	expected += L"vn -1 0 0\n";
	expected += L"vn -1 0 0\n";
	expected += L"vn 0 0 -1\n";
	expected += L"vn 0 0 -1\n";
	expected += L"vn 0 0 1\n";
	expected += L"vn 0 0 1\n";
	expected += L"usemtl Material1\n";
	expected += L"f 9//13 10//13 11//13\n";
	expected += L"f 9//14 11//14 12//14\n";
	expected += L"f 10//15 14//15 15//15\n";
	expected += L"f 10//16 15//16 11//16\n";
	expected += L"f 14//17 13//17 16//17\n";
	expected += L"f 14//18 16//18 15//18\n";
	expected += L"f 13//19 9//19 12//19\n";
	expected += L"f 13//20 12//20 16//20\n";
	expected += L"f 9//21 13//21 14//21\n";
	expected += L"f 9//22 14//22 10//22\n";
	expected += L"f 12//23 11//23 15//23\n";
	expected += L"f 12//24 15//24 16//24\n";
	expected += L"# close file\n";

	ASSERT (CheckResult (expected, writer.result));
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

	ASSERT (CheckResult (expected, writer.result));
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
	expected += L"facet normal -0 -1 0\n";
	expected += L"    outer loop\n";
	expected += L"        vertex -0.5 -0.5 -0.5\n";
	expected += L"        vertex 0.5 -0.5 -0.5\n";
	expected += L"        vertex 0.5 -0.5 0.5\n";
	expected += L"    endloop\n";
	expected += L"endfacet\n";
	expected += L"facet normal 0 -1 -0\n";
	expected += L"    outer loop\n";
	expected += L"        vertex -0.5 -0.5 -0.5\n";
	expected += L"        vertex 0.5 -0.5 0.5\n";
	expected += L"        vertex -0.5 -0.5 0.5\n";
	expected += L"    endloop\n";
	expected += L"endfacet\n";
	expected += L"facet normal 1 0 0\n";
	expected += L"    outer loop\n";
	expected += L"        vertex 0.5 -0.5 -0.5\n";
	expected += L"        vertex 0.5 0.5 -0.5\n";
	expected += L"        vertex 0.5 0.5 0.5\n";
	expected += L"    endloop\n";
	expected += L"endfacet\n";
	expected += L"facet normal 1 0 0\n";
	expected += L"    outer loop\n";
	expected += L"        vertex 0.5 -0.5 -0.5\n";
	expected += L"        vertex 0.5 0.5 0.5\n";
	expected += L"        vertex 0.5 -0.5 0.5\n";
	expected += L"    endloop\n";
	expected += L"endfacet\n";
	expected += L"facet normal -0 1 0\n";
	expected += L"    outer loop\n";
	expected += L"        vertex 0.5 0.5 -0.5\n";
	expected += L"        vertex -0.5 0.5 -0.5\n";
	expected += L"        vertex -0.5 0.5 0.5\n";
	expected += L"    endloop\n";
	expected += L"endfacet\n";
	expected += L"facet normal 0 1 0\n";
	expected += L"    outer loop\n";
	expected += L"        vertex 0.5 0.5 -0.5\n";
	expected += L"        vertex -0.5 0.5 0.5\n";
	expected += L"        vertex 0.5 0.5 0.5\n";
	expected += L"    endloop\n";
	expected += L"endfacet\n";
	expected += L"facet normal -1 0 0\n";
	expected += L"    outer loop\n";
	expected += L"        vertex -0.5 0.5 -0.5\n";
	expected += L"        vertex -0.5 -0.5 -0.5\n";
	expected += L"        vertex -0.5 -0.5 0.5\n";
	expected += L"    endloop\n";
	expected += L"endfacet\n";
	expected += L"facet normal -1 0 0\n";
	expected += L"    outer loop\n";
	expected += L"        vertex -0.5 0.5 -0.5\n";
	expected += L"        vertex -0.5 -0.5 0.5\n";
	expected += L"        vertex -0.5 0.5 0.5\n";
	expected += L"    endloop\n";
	expected += L"endfacet\n";
	expected += L"facet normal 0 -0 -1\n";
	expected += L"    outer loop\n";
	expected += L"        vertex -0.5 -0.5 -0.5\n";
	expected += L"        vertex -0.5 0.5 -0.5\n";
	expected += L"        vertex 0.5 0.5 -0.5\n";
	expected += L"    endloop\n";
	expected += L"endfacet\n";
	expected += L"facet normal -0 0 -1\n";
	expected += L"    outer loop\n";
	expected += L"        vertex -0.5 -0.5 -0.5\n";
	expected += L"        vertex 0.5 0.5 -0.5\n";
	expected += L"        vertex 0.5 -0.5 -0.5\n";
	expected += L"    endloop\n";
	expected += L"endfacet\n";
	expected += L"facet normal 0 0 1\n";
	expected += L"    outer loop\n";
	expected += L"        vertex -0.5 -0.5 0.5\n";
	expected += L"        vertex 0.5 -0.5 0.5\n";
	expected += L"        vertex 0.5 0.5 0.5\n";
	expected += L"    endloop\n";
	expected += L"endfacet\n";
	expected += L"facet normal 0 0 1\n";
	expected += L"    outer loop\n";
	expected += L"        vertex -0.5 -0.5 0.5\n";
	expected += L"        vertex 0.5 0.5 0.5\n";
	expected += L"        vertex -0.5 0.5 0.5\n";
	expected += L"    endloop\n";
	expected += L"endfacet\n";
	expected += L"facet normal -0 -1 0\n";
	expected += L"    outer loop\n";
	expected += L"        vertex 1.5 -0.5 -0.5\n";
	expected += L"        vertex 2.5 -0.5 -0.5\n";
	expected += L"        vertex 2.5 -0.5 0.5\n";
	expected += L"    endloop\n";
	expected += L"endfacet\n";
	expected += L"facet normal 0 -1 -0\n";
	expected += L"    outer loop\n";
	expected += L"        vertex 1.5 -0.5 -0.5\n";
	expected += L"        vertex 2.5 -0.5 0.5\n";
	expected += L"        vertex 1.5 -0.5 0.5\n";
	expected += L"    endloop\n";
	expected += L"endfacet\n";
	expected += L"facet normal 1 0 0\n";
	expected += L"    outer loop\n";
	expected += L"        vertex 2.5 -0.5 -0.5\n";
	expected += L"        vertex 2.5 0.5 -0.5\n";
	expected += L"        vertex 2.5 0.5 0.5\n";
	expected += L"    endloop\n";
	expected += L"endfacet\n";
	expected += L"facet normal 1 0 0\n";
	expected += L"    outer loop\n";
	expected += L"        vertex 2.5 -0.5 -0.5\n";
	expected += L"        vertex 2.5 0.5 0.5\n";
	expected += L"        vertex 2.5 -0.5 0.5\n";
	expected += L"    endloop\n";
	expected += L"endfacet\n";
	expected += L"facet normal -0 1 0\n";
	expected += L"    outer loop\n";
	expected += L"        vertex 2.5 0.5 -0.5\n";
	expected += L"        vertex 1.5 0.5 -0.5\n";
	expected += L"        vertex 1.5 0.5 0.5\n";
	expected += L"    endloop\n";
	expected += L"endfacet\n";
	expected += L"facet normal 0 1 0\n";
	expected += L"    outer loop\n";
	expected += L"        vertex 2.5 0.5 -0.5\n";
	expected += L"        vertex 1.5 0.5 0.5\n";
	expected += L"        vertex 2.5 0.5 0.5\n";
	expected += L"    endloop\n";
	expected += L"endfacet\n";
	expected += L"facet normal -1 0 0\n";
	expected += L"    outer loop\n";
	expected += L"        vertex 1.5 0.5 -0.5\n";
	expected += L"        vertex 1.5 -0.5 -0.5\n";
	expected += L"        vertex 1.5 -0.5 0.5\n";
	expected += L"    endloop\n";
	expected += L"endfacet\n";
	expected += L"facet normal -1 0 0\n";
	expected += L"    outer loop\n";
	expected += L"        vertex 1.5 0.5 -0.5\n";
	expected += L"        vertex 1.5 -0.5 0.5\n";
	expected += L"        vertex 1.5 0.5 0.5\n";
	expected += L"    endloop\n";
	expected += L"endfacet\n";
	expected += L"facet normal 0 -0 -1\n";
	expected += L"    outer loop\n";
	expected += L"        vertex 1.5 -0.5 -0.5\n";
	expected += L"        vertex 1.5 0.5 -0.5\n";
	expected += L"        vertex 2.5 0.5 -0.5\n";
	expected += L"    endloop\n";
	expected += L"endfacet\n";
	expected += L"facet normal -0 0 -1\n";
	expected += L"    outer loop\n";
	expected += L"        vertex 1.5 -0.5 -0.5\n";
	expected += L"        vertex 2.5 0.5 -0.5\n";
	expected += L"        vertex 2.5 -0.5 -0.5\n";
	expected += L"    endloop\n";
	expected += L"endfacet\n";
	expected += L"facet normal 0 0 1\n";
	expected += L"    outer loop\n";
	expected += L"        vertex 1.5 -0.5 0.5\n";
	expected += L"        vertex 2.5 -0.5 0.5\n";
	expected += L"        vertex 2.5 0.5 0.5\n";
	expected += L"    endloop\n";
	expected += L"endfacet\n";
	expected += L"facet normal 0 0 1\n";
	expected += L"    outer loop\n";
	expected += L"        vertex 1.5 -0.5 0.5\n";
	expected += L"        vertex 2.5 0.5 0.5\n";
	expected += L"        vertex 1.5 0.5 0.5\n";
	expected += L"    endloop\n";
	expected += L"endfacet\n";
	expected += L"endsolid model\n";
	expected += L"# close file\n";

	ASSERT (CheckResult (expected, writer.result));
}

}
