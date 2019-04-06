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
	expected += L"v 0.5 0.5 -0.5\n";
	expected += L"v -0.5 0.5 -0.5\n";
	expected += L"v -0.5 -0.5 0.5\n";
	expected += L"v 0.5 -0.5 0.5\n";
	expected += L"v 0.5 0.5 0.5\n";
	expected += L"v -0.5 0.5 0.5\n";
	expected += L"vn 0 0 -1\n";
	expected += L"vn 0 0 -1\n";
	expected += L"vn 0 0 1\n";
	expected += L"vn 0 0 1\n";
	expected += L"vn 0 -1 0\n";
	expected += L"vn 0 -1 0\n";
	expected += L"vn 1 0 0\n";
	expected += L"vn 1 0 0\n";
	expected += L"vn 0 1 0\n";
	expected += L"vn 0 1 0\n";
	expected += L"vn -1 0 0\n";
	expected += L"vn -1 0 0\n";
	expected += L"usemtl Material0\n";
	expected += L"f 1//1 3//1 2//1\n";
	expected += L"f 1//2 4//2 3//2\n";
	expected += L"f 5//3 6//3 7//3\n";
	expected += L"f 5//4 7//4 8//4\n";
	expected += L"f 1//5 2//5 6//5\n";
	expected += L"f 1//6 6//6 5//6\n";
	expected += L"f 2//7 3//7 7//7\n";
	expected += L"f 2//8 7//8 6//8\n";
	expected += L"f 3//9 4//9 8//9\n";
	expected += L"f 3//10 8//10 7//10\n";
	expected += L"f 4//11 1//11 5//11\n";
	expected += L"f 4//12 5//12 8//12\n";
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
	expected += L"v 0.5 0.5 -0.5\n";
	expected += L"v -0.5 0.5 -0.5\n";
	expected += L"v -0.5 -0.5 0.5\n";
	expected += L"v 0.5 -0.5 0.5\n";
	expected += L"v 0.5 0.5 0.5\n";
	expected += L"v -0.5 0.5 0.5\n";
	expected += L"vn 0 0 -1\n";
	expected += L"vn 0 0 -1\n";
	expected += L"vn 0 0 1\n";
	expected += L"vn 0 0 1\n";
	expected += L"vn 0 -1 0\n";
	expected += L"vn 0 -1 0\n";
	expected += L"vn 1 0 0\n";
	expected += L"vn 1 0 0\n";
	expected += L"vn 0 1 0\n";
	expected += L"vn 0 1 0\n";
	expected += L"vn -1 0 0\n";
	expected += L"vn -1 0 0\n";
	expected += L"usemtl Material0\n";
	expected += L"f 1//1 3//1 2//1\n";
	expected += L"f 1//2 4//2 3//2\n";
	expected += L"f 5//3 6//3 7//3\n";
	expected += L"f 5//4 7//4 8//4\n";
	expected += L"f 1//5 2//5 6//5\n";
	expected += L"f 1//6 6//6 5//6\n";
	expected += L"f 2//7 3//7 7//7\n";
	expected += L"f 2//8 7//8 6//8\n";
	expected += L"f 3//9 4//9 8//9\n";
	expected += L"f 3//10 8//10 7//10\n";
	expected += L"f 4//11 1//11 5//11\n";
	expected += L"f 4//12 5//12 8//12\n";
	expected += L"g Mesh2\n";
	expected += L"v 1.5 -0.5 -0.5\n";
	expected += L"v 2.5 -0.5 -0.5\n";
	expected += L"v 2.5 0.5 -0.5\n";
	expected += L"v 1.5 0.5 -0.5\n";
	expected += L"v 1.5 -0.5 0.5\n";
	expected += L"v 2.5 -0.5 0.5\n";
	expected += L"v 2.5 0.5 0.5\n";
	expected += L"v 1.5 0.5 0.5\n";
	expected += L"vn 0 0 -1\n";
	expected += L"vn 0 0 -1\n";
	expected += L"vn 0 0 1\n";
	expected += L"vn 0 0 1\n";
	expected += L"vn 0 -1 0\n";
	expected += L"vn 0 -1 0\n";
	expected += L"vn 1 0 0\n";
	expected += L"vn 1 0 0\n";
	expected += L"vn 0 1 0\n";
	expected += L"vn 0 1 0\n";
	expected += L"vn -1 0 0\n";
	expected += L"vn -1 0 0\n";
	expected += L"usemtl Material1\n";
	expected += L"f 9//13 11//13 10//13\n";
	expected += L"f 9//14 12//14 11//14\n";
	expected += L"f 13//15 14//15 15//15\n";
	expected += L"f 13//16 15//16 16//16\n";
	expected += L"f 9//17 10//17 14//17\n";
	expected += L"f 9//18 14//18 13//18\n";
	expected += L"f 10//19 11//19 15//19\n";
	expected += L"f 10//20 15//20 14//20\n";
	expected += L"f 11//21 12//21 16//21\n";
	expected += L"f 11//22 16//22 15//22\n";
	expected += L"f 12//23 9//23 13//23\n";
	expected += L"f 12//24 13//24 16//24\n";
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
	expected += L"facet normal -0 0 -1\n";
	expected += L"    outer loop\n";
	expected += L"        vertex -0.5 -0.5 -0.5\n";
	expected += L"        vertex 0.5 0.5 -0.5\n";
	expected += L"        vertex 0.5 -0.5 -0.5\n";
	expected += L"    endloop\n";
	expected += L"endfacet\n";
	expected += L"facet normal 0 -0 -1\n";
	expected += L"    outer loop\n";
	expected += L"        vertex -0.5 -0.5 -0.5\n";
	expected += L"        vertex -0.5 0.5 -0.5\n";
	expected += L"        vertex 0.5 0.5 -0.5\n";
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
	expected += L"facet normal -0 0 -1\n";
	expected += L"    outer loop\n";
	expected += L"        vertex 1.5 -0.5 -0.5\n";
	expected += L"        vertex 2.5 0.5 -0.5\n";
	expected += L"        vertex 2.5 -0.5 -0.5\n";
	expected += L"    endloop\n";
	expected += L"endfacet\n";
	expected += L"facet normal 0 -0 -1\n";
	expected += L"    outer loop\n";
	expected += L"        vertex 1.5 -0.5 -0.5\n";
	expected += L"        vertex 1.5 0.5 -0.5\n";
	expected += L"        vertex 2.5 0.5 -0.5\n";
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
	expected += L"endsolid model\n";
	expected += L"# close file\n";

	ASSERT (CheckResult (expected, writer.result));
}

}
