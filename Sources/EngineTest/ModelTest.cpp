#include "SimpleTest.hpp"
#include "Model.hpp"
#include "MeshGenerators.hpp"

using namespace Modeler;

namespace ModelTest
{

TEST (ModelTest)
{
	Model model;

	Mesh mesh1 = GenerateBox (Material (glm::dvec3 (1.0, 0.0, 0.0)), glm::dmat4 (1.0), 1.0, 1.0, 1.0);
	Mesh mesh2 = GenerateBox (Material (glm::dvec3 (1.0, 0.0, 0.0)), glm::translate (glm::dmat4 (1.0), glm::dvec3 (2.0, 0.0, 0.0)), 1.0, 1.0, 1.0);
	Mesh mesh3 = GenerateBox (Material (glm::dvec3 (1.0, 0.0, 0.0)), glm::translate (glm::dmat4 (1.0), glm::dvec3 (4.0, 0.0, 0.0)), 1.0, 1.0, 2.0);
	Mesh mesh4 = GenerateBox (Material (glm::dvec3 (0.0, 1.0, 0.0)), glm::translate (glm::dmat4 (1.0), glm::dvec3 (4.0, 0.0, 0.0)), 1.0, 1.0, 1.0);

	model.AddMesh (mesh1);
	model.AddMesh (mesh2);
	model.AddMesh (mesh3);
	model.AddMesh (mesh4);

	ModelInfo modelInfo = model.GetModelInfo ();
	ASSERT (modelInfo.meshGeometryCount == 2);
	ASSERT (modelInfo.meshMaterialsCount == 2);
	ASSERT (modelInfo.meshCount == 4);
	ASSERT (modelInfo.vertexCount == 4 * 8);
	ASSERT (modelInfo.triangleCount == 4 * 12);
}

TEST (MaterialTest)
{
	Model model;

	Mesh mesh1;
	MaterialId material1 = mesh1.AddMaterial (Material (glm::dvec3 (1.0, 0.0, 0.0)));
	MaterialId material2 = mesh1.AddMaterial (Material (glm::dvec3 (0.0, 1.0, 0.0)));
	mesh1.AddVertex (glm::dvec3 (0.0, 0.0, 0.0));
	mesh1.AddVertex (glm::dvec3 (1.0, 0.0, 0.0));
	mesh1.AddVertex (glm::dvec3 (1.0, 1.0, 0.0));
	mesh1.AddVertex (glm::dvec3 (0.0, 0.0, 1.0));
	mesh1.AddVertex (glm::dvec3 (1.0, 0.0, 1.0));
	mesh1.AddVertex (glm::dvec3 (1.0, 1.0, 1.0));
	mesh1.AddTriangle (0, 1, 2, material1);
	mesh1.AddTriangle (3, 4, 5, material2);
	MeshId meshId1 = model.AddMesh (mesh1);

	Mesh mesh2 = mesh1;
	MeshId meshId2 = model.AddMesh (mesh2);

	{
		ModelInfo modelInfo = model.GetModelInfo ();
		ASSERT (modelInfo.meshGeometryCount == 1);
		ASSERT (modelInfo.meshMaterialsCount == 1);
		ASSERT (modelInfo.meshCount == 2);
	}

	model.RemoveMesh (meshId2);

	{
		ModelInfo modelInfo = model.GetModelInfo ();
		ASSERT (modelInfo.meshGeometryCount == 1);
		ASSERT (modelInfo.meshMaterialsCount == 1);
		ASSERT (modelInfo.meshCount == 1);
	}

	model.RemoveMesh (meshId1);

	{
		ModelInfo modelInfo = model.GetModelInfo ();
		ASSERT (modelInfo.meshGeometryCount == 0);
		ASSERT (modelInfo.meshMaterialsCount == 0);
		ASSERT (modelInfo.meshCount == 0);
	}
}

}
