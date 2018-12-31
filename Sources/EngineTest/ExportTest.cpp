#include "SimpleTest.hpp"
#include "Model.hpp"
#include "MeshGenerators.hpp"
#include "Export.hpp"

using namespace Modeler;

class ModelWriterForTest : public ModelWriter
{
public:
	virtual void OpenFile (const std::string& fileName) override
	{
		result += "# open file " + fileName + "\n";
	}

	virtual void CloseFile () override
	{
		result += "# close file\n";
	}

	virtual void WriteLine (const std::string& text) override
	{
		result += text + "\n";
	}

	std::string result;
};

namespace ExportTest
{

static bool CheckResult (const std::string& expected, const std::string& result)
{
	if (result == expected) {
		return true;
	} else {
		std::cout << std::endl << expected;
		std::cout << std::endl << result << std::endl;
		return false;
	}
}

TEST (Obj_OneMeshExportTest)
{
	Model model;
	model.AddMesh (GenerateBox (Material (glm::dvec3 (1.0, 0.0, 0.0)), glm::dmat4 (1.0), 1.0, 1.0, 1.0));
	
	ModelWriterForTest writer;
	ExportModelToObj (model, "model", writer);
	std::string expected;
	expected += "# open file model.mtl\n";
	expected += "newmtl Material0\n";
	expected += "Kd 1 0 0\n";
	expected += "# close file\n";
	expected += "# open file model.obj\n";
	expected += "mtllib model.mtl\n";
	expected += "g Mesh1\n";
	expected += "v -0.5 -0.5 -0.5\n";
	expected += "v 0.5 -0.5 -0.5\n";
	expected += "v 0.5 -0.5 0.5\n";
	expected += "v -0.5 -0.5 0.5\n";
	expected += "v -0.5 0.5 -0.5\n";
	expected += "v 0.5 0.5 -0.5\n";
	expected += "v 0.5 0.5 0.5\n";
	expected += "v -0.5 0.5 0.5\n";
	expected += "vn 0 -1 0\n";
	expected += "vn 0 -1 0\n";
	expected += "vn 1 0 0\n";
	expected += "vn 1 0 0\n";
	expected += "vn 0 1 0\n";
	expected += "vn 0 1 0\n";
	expected += "vn -1 0 0\n";
	expected += "vn -1 0 0\n";
	expected += "vn 0 0 -1\n";
	expected += "vn 0 0 -1\n";
	expected += "vn 0 0 1\n";
	expected += "vn 0 0 1\n";
	expected += "usemtl Material0\n";
	expected += "f 1//1 2//1 3//1\n";
	expected += "f 1//2 3//2 4//2\n";
	expected += "f 2//3 6//3 7//3\n";
	expected += "f 2//4 7//4 3//4\n";
	expected += "f 6//5 5//5 8//5\n";
	expected += "f 6//6 8//6 7//6\n";
	expected += "f 5//7 1//7 4//7\n";
	expected += "f 5//8 4//8 8//8\n";
	expected += "f 1//9 5//9 6//9\n";
	expected += "f 1//10 6//10 2//10\n";
	expected += "f 4//11 3//11 7//11\n";
	expected += "f 4//12 7//12 8//12\n";
	expected += "# close file\n";

	ASSERT (CheckResult (expected, writer.result));
}

TEST (Obj_TwoMeshExportTest)
{
	Model model;

	model.AddMesh (GenerateBox (Material (glm::dvec3 (1.0, 0.0, 0.0)), glm::dmat4 (1.0), 1.0, 1.0, 1.0));
	model.AddMesh (GenerateBox (Material (glm::dvec3 (0.0, 1.0, 0.0)), glm::translate (glm::dmat4 (1.0), glm::dvec3 (2.0, 0.0, 0.0)), 1.0, 1.0, 1.0));

	ModelWriterForTest writer;
	ExportModelToObj (model, "model", writer);
	std::string expected;
	expected += "# open file model.mtl\n";
	expected += "newmtl Material0\n";
	expected += "Kd 1 0 0\n";
	expected += "newmtl Material1\n";
	expected += "Kd 0 1 0\n";
	expected += "# close file\n";
	expected += "# open file model.obj\n";
	expected += "mtllib model.mtl\n";
	expected += "g Mesh1\n";
	expected += "v -0.5 -0.5 -0.5\n";
	expected += "v 0.5 -0.5 -0.5\n";
	expected += "v 0.5 -0.5 0.5\n";
	expected += "v -0.5 -0.5 0.5\n";
	expected += "v -0.5 0.5 -0.5\n";
	expected += "v 0.5 0.5 -0.5\n";
	expected += "v 0.5 0.5 0.5\n";
	expected += "v -0.5 0.5 0.5\n";
	expected += "vn 0 -1 0\n";
	expected += "vn 0 -1 0\n";
	expected += "vn 1 0 0\n";
	expected += "vn 1 0 0\n";
	expected += "vn 0 1 0\n";
	expected += "vn 0 1 0\n";
	expected += "vn -1 0 0\n";
	expected += "vn -1 0 0\n";
	expected += "vn 0 0 -1\n";
	expected += "vn 0 0 -1\n";
	expected += "vn 0 0 1\n";
	expected += "vn 0 0 1\n";
	expected += "usemtl Material0\n";
	expected += "f 1//1 2//1 3//1\n";
	expected += "f 1//2 3//2 4//2\n";
	expected += "f 2//3 6//3 7//3\n";
	expected += "f 2//4 7//4 3//4\n";
	expected += "f 6//5 5//5 8//5\n";
	expected += "f 6//6 8//6 7//6\n";
	expected += "f 5//7 1//7 4//7\n";
	expected += "f 5//8 4//8 8//8\n";
	expected += "f 1//9 5//9 6//9\n";
	expected += "f 1//10 6//10 2//10\n";
	expected += "f 4//11 3//11 7//11\n";
	expected += "f 4//12 7//12 8//12\n";
	expected += "g Mesh2\n";
	expected += "v 1.5 -0.5 -0.5\n";
	expected += "v 2.5 -0.5 -0.5\n";
	expected += "v 2.5 -0.5 0.5\n";
	expected += "v 1.5 -0.5 0.5\n";
	expected += "v 1.5 0.5 -0.5\n";
	expected += "v 2.5 0.5 -0.5\n";
	expected += "v 2.5 0.5 0.5\n";
	expected += "v 1.5 0.5 0.5\n";
	expected += "vn 0 -1 0\n";
	expected += "vn 0 -1 0\n";
	expected += "vn 1 0 0\n";
	expected += "vn 1 0 0\n";
	expected += "vn 0 1 0\n";
	expected += "vn 0 1 0\n";
	expected += "vn -1 0 0\n";
	expected += "vn -1 0 0\n";
	expected += "vn 0 0 -1\n";
	expected += "vn 0 0 -1\n";
	expected += "vn 0 0 1\n";
	expected += "vn 0 0 1\n";
	expected += "usemtl Material1\n";
	expected += "f 9//13 10//13 11//13\n";
	expected += "f 9//14 11//14 12//14\n";
	expected += "f 10//15 14//15 15//15\n";
	expected += "f 10//16 15//16 11//16\n";
	expected += "f 14//17 13//17 16//17\n";
	expected += "f 14//18 16//18 15//18\n";
	expected += "f 13//19 9//19 12//19\n";
	expected += "f 13//20 12//20 16//20\n";
	expected += "f 9//21 13//21 14//21\n";
	expected += "f 9//22 14//22 10//22\n";
	expected += "f 12//23 11//23 15//23\n";
	expected += "f 12//24 15//24 16//24\n";
	expected += "# close file\n";

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
	ExportModelToObj (model, "model", writer);
	std::string expected;
	expected += "# open file model.mtl\n";
	expected += "newmtl Material0\n";
	expected += "Kd 1 0 0\n";
	expected += "newmtl Material1\n";
	expected += "Kd 0 1 0\n";
	expected += "# close file\n";
	expected += "# open file model.obj\n";
	expected += "mtllib model.mtl\n";
	expected += "g Mesh1\n";
	expected += "v 0 0 0\n";
	expected += "v 1 0 0\n";
	expected += "v 1 1 0\n";
	expected += "v 0 0 1\n";
	expected += "v 1 0 1\n";
	expected += "v 1 1 1\n";
	expected += "vn 0 0 1\n";
	expected += "vn 0 0 1\n";
	expected += "usemtl Material0\n";
	expected += "f 1//1 2//1 3//1\n";
	expected += "usemtl Material1\n";
	expected += "f 4//2 5//2 6//2\n";
	expected += "# close file\n";

	ASSERT (CheckResult (expected, writer.result));
}

TEST (Stl_TwoMeshExportTest)
{
	Model model;

	model.AddMesh (GenerateBox (Material (glm::dvec3 (1.0, 0.0, 0.0)), glm::dmat4 (1.0), 1.0, 1.0, 1.0));
	model.AddMesh (GenerateBox (Material (glm::dvec3 (0.0, 1.0, 0.0)), glm::translate (glm::dmat4 (1.0), glm::dvec3 (2.0, 0.0, 0.0)), 1.0, 1.0, 1.0));

	ModelWriterForTest writer;
	ExportModelToStl (model, "model", writer);
	std::string expected;
	expected += "# open file model.stl\n";
	expected += "solid model\n";
	expected += "facet normal -0 -1 0\n";
	expected += "    outer loop\n";
	expected += "        vertex -0.5 -0.5 -0.5\n";
	expected += "        vertex 0.5 -0.5 -0.5\n";
	expected += "        vertex 0.5 -0.5 0.5\n";
	expected += "    endloop\n";
	expected += "endfacet\n";
	expected += "facet normal 0 -1 -0\n";
	expected += "    outer loop\n";
	expected += "        vertex -0.5 -0.5 -0.5\n";
	expected += "        vertex 0.5 -0.5 0.5\n";
	expected += "        vertex -0.5 -0.5 0.5\n";
	expected += "    endloop\n";
	expected += "endfacet\n";
	expected += "facet normal 1 0 0\n";
	expected += "    outer loop\n";
	expected += "        vertex 0.5 -0.5 -0.5\n";
	expected += "        vertex 0.5 0.5 -0.5\n";
	expected += "        vertex 0.5 0.5 0.5\n";
	expected += "    endloop\n";
	expected += "endfacet\n";
	expected += "facet normal 1 0 0\n";
	expected += "    outer loop\n";
	expected += "        vertex 0.5 -0.5 -0.5\n";
	expected += "        vertex 0.5 0.5 0.5\n";
	expected += "        vertex 0.5 -0.5 0.5\n";
	expected += "    endloop\n";
	expected += "endfacet\n";
	expected += "facet normal -0 1 0\n";
	expected += "    outer loop\n";
	expected += "        vertex 0.5 0.5 -0.5\n";
	expected += "        vertex -0.5 0.5 -0.5\n";
	expected += "        vertex -0.5 0.5 0.5\n";
	expected += "    endloop\n";
	expected += "endfacet\n";
	expected += "facet normal 0 1 0\n";
	expected += "    outer loop\n";
	expected += "        vertex 0.5 0.5 -0.5\n";
	expected += "        vertex -0.5 0.5 0.5\n";
	expected += "        vertex 0.5 0.5 0.5\n";
	expected += "    endloop\n";
	expected += "endfacet\n";
	expected += "facet normal -1 0 0\n";
	expected += "    outer loop\n";
	expected += "        vertex -0.5 0.5 -0.5\n";
	expected += "        vertex -0.5 -0.5 -0.5\n";
	expected += "        vertex -0.5 -0.5 0.5\n";
	expected += "    endloop\n";
	expected += "endfacet\n";
	expected += "facet normal -1 0 0\n";
	expected += "    outer loop\n";
	expected += "        vertex -0.5 0.5 -0.5\n";
	expected += "        vertex -0.5 -0.5 0.5\n";
	expected += "        vertex -0.5 0.5 0.5\n";
	expected += "    endloop\n";
	expected += "endfacet\n";
	expected += "facet normal 0 -0 -1\n";
	expected += "    outer loop\n";
	expected += "        vertex -0.5 -0.5 -0.5\n";
	expected += "        vertex -0.5 0.5 -0.5\n";
	expected += "        vertex 0.5 0.5 -0.5\n";
	expected += "    endloop\n";
	expected += "endfacet\n";
	expected += "facet normal -0 0 -1\n";
	expected += "    outer loop\n";
	expected += "        vertex -0.5 -0.5 -0.5\n";
	expected += "        vertex 0.5 0.5 -0.5\n";
	expected += "        vertex 0.5 -0.5 -0.5\n";
	expected += "    endloop\n";
	expected += "endfacet\n";
	expected += "facet normal 0 0 1\n";
	expected += "    outer loop\n";
	expected += "        vertex -0.5 -0.5 0.5\n";
	expected += "        vertex 0.5 -0.5 0.5\n";
	expected += "        vertex 0.5 0.5 0.5\n";
	expected += "    endloop\n";
	expected += "endfacet\n";
	expected += "facet normal 0 0 1\n";
	expected += "    outer loop\n";
	expected += "        vertex -0.5 -0.5 0.5\n";
	expected += "        vertex 0.5 0.5 0.5\n";
	expected += "        vertex -0.5 0.5 0.5\n";
	expected += "    endloop\n";
	expected += "endfacet\n";
	expected += "facet normal -0 -1 0\n";
	expected += "    outer loop\n";
	expected += "        vertex 1.5 -0.5 -0.5\n";
	expected += "        vertex 2.5 -0.5 -0.5\n";
	expected += "        vertex 2.5 -0.5 0.5\n";
	expected += "    endloop\n";
	expected += "endfacet\n";
	expected += "facet normal 0 -1 -0\n";
	expected += "    outer loop\n";
	expected += "        vertex 1.5 -0.5 -0.5\n";
	expected += "        vertex 2.5 -0.5 0.5\n";
	expected += "        vertex 1.5 -0.5 0.5\n";
	expected += "    endloop\n";
	expected += "endfacet\n";
	expected += "facet normal 1 0 0\n";
	expected += "    outer loop\n";
	expected += "        vertex 2.5 -0.5 -0.5\n";
	expected += "        vertex 2.5 0.5 -0.5\n";
	expected += "        vertex 2.5 0.5 0.5\n";
	expected += "    endloop\n";
	expected += "endfacet\n";
	expected += "facet normal 1 0 0\n";
	expected += "    outer loop\n";
	expected += "        vertex 2.5 -0.5 -0.5\n";
	expected += "        vertex 2.5 0.5 0.5\n";
	expected += "        vertex 2.5 -0.5 0.5\n";
	expected += "    endloop\n";
	expected += "endfacet\n";
	expected += "facet normal -0 1 0\n";
	expected += "    outer loop\n";
	expected += "        vertex 2.5 0.5 -0.5\n";
	expected += "        vertex 1.5 0.5 -0.5\n";
	expected += "        vertex 1.5 0.5 0.5\n";
	expected += "    endloop\n";
	expected += "endfacet\n";
	expected += "facet normal 0 1 0\n";
	expected += "    outer loop\n";
	expected += "        vertex 2.5 0.5 -0.5\n";
	expected += "        vertex 1.5 0.5 0.5\n";
	expected += "        vertex 2.5 0.5 0.5\n";
	expected += "    endloop\n";
	expected += "endfacet\n";
	expected += "facet normal -1 0 0\n";
	expected += "    outer loop\n";
	expected += "        vertex 1.5 0.5 -0.5\n";
	expected += "        vertex 1.5 -0.5 -0.5\n";
	expected += "        vertex 1.5 -0.5 0.5\n";
	expected += "    endloop\n";
	expected += "endfacet\n";
	expected += "facet normal -1 0 0\n";
	expected += "    outer loop\n";
	expected += "        vertex 1.5 0.5 -0.5\n";
	expected += "        vertex 1.5 -0.5 0.5\n";
	expected += "        vertex 1.5 0.5 0.5\n";
	expected += "    endloop\n";
	expected += "endfacet\n";
	expected += "facet normal 0 -0 -1\n";
	expected += "    outer loop\n";
	expected += "        vertex 1.5 -0.5 -0.5\n";
	expected += "        vertex 1.5 0.5 -0.5\n";
	expected += "        vertex 2.5 0.5 -0.5\n";
	expected += "    endloop\n";
	expected += "endfacet\n";
	expected += "facet normal -0 0 -1\n";
	expected += "    outer loop\n";
	expected += "        vertex 1.5 -0.5 -0.5\n";
	expected += "        vertex 2.5 0.5 -0.5\n";
	expected += "        vertex 2.5 -0.5 -0.5\n";
	expected += "    endloop\n";
	expected += "endfacet\n";
	expected += "facet normal 0 0 1\n";
	expected += "    outer loop\n";
	expected += "        vertex 1.5 -0.5 0.5\n";
	expected += "        vertex 2.5 -0.5 0.5\n";
	expected += "        vertex 2.5 0.5 0.5\n";
	expected += "    endloop\n";
	expected += "endfacet\n";
	expected += "facet normal 0 0 1\n";
	expected += "    outer loop\n";
	expected += "        vertex 1.5 -0.5 0.5\n";
	expected += "        vertex 2.5 0.5 0.5\n";
	expected += "        vertex 1.5 0.5 0.5\n";
	expected += "    endloop\n";
	expected += "endfacet\n";
	expected += "endsolid model\n";
	expected += "# close file\n";

	ASSERT (CheckResult (expected, writer.result));
}

}
