#include "SimpleTest.hpp"
#include "Geometry.hpp"
#include "MeshGenerators.hpp"
#include "Mesh.hpp"
#include "BSPTree.hpp"

using namespace Geometry;
using namespace Modeler;

namespace BSPTreeTest
{

class TriangleClipCounter : public BSPTriangleClipper
{
public:
	TriangleClipCounter () :
		frontCount (0),
		backCount (0),
		planeCount (0)
	{
	}

	virtual void FrontTrianglesFound (const std::vector<Triangle>& triangles) override
	{
		frontCount += triangles.size ();
	}

	virtual void BackTrianglesFound (const std::vector<Triangle>& triangles) override
	{
		backCount += triangles.size ();
	}

	virtual void PlaneTrianglesFound (const std::vector<Triangle>& triangles) override
	{
		planeCount += triangles.size ();
	}

	size_t frontCount;
	size_t backCount;
	size_t planeCount;
};

TEST (BSPTreeClipTest)
{
	Mesh cube = GenerateBox (DefaultMaterial, glm::translate (glm::dmat4 (1.0), glm::dvec3 (-0.5, -0.5, -0.5)), 1.0, 1.0, 1.0);

	BSPTree tree;

	const glm::dmat4& transformation = cube.GetTransformation ();
	cube.GetGeometry ().EnumerateTriangles ([&] (const MeshTriangle& triangle) {
		tree.AddTriangle (Triangle (
			glm::dvec3 (transformation * glm::dvec4 (cube.GetGeometry ().GetVertex (triangle.v1), 1.0)),
			glm::dvec3 (transformation * glm::dvec4 (cube.GetGeometry ().GetVertex (triangle.v2), 1.0)),
			glm::dvec3 (transformation * glm::dvec4 (cube.GetGeometry ().GetVertex (triangle.v3), 1.0))
		));
	});

	{
		Triangle triangle (glm::dvec3 (2.0, 0.0, 0.0), glm::dvec3 (4.0, 0.0, 0.0), glm::dvec3 (3.0, 1.0, 0.0));
		TriangleClipCounter counter;
		tree.ClipTriangle (triangle, counter);
		ASSERT (counter.frontCount == 1 && counter.backCount == 0 && counter.planeCount == 0);
	}

	{
		Triangle triangle (glm::dvec3 (0.0, 0.0, 0.0), glm::dvec3 (0.2, 0.0, 0.0), glm::dvec3 (0.1, 0.3, 0.0));
		TriangleClipCounter counter;
		tree.ClipTriangle (triangle, counter);
		ASSERT (counter.frontCount == 0 && counter.backCount == 1 && counter.planeCount == 0);
	}

	{
		Triangle triangle (glm::dvec3 (0.0, 0.0, 0.0), glm::dvec3 (2.0, 0.0, 0.0), glm::dvec3 (2.0, 1.0, 0.0));
		TriangleClipCounter counter;
		tree.ClipTriangle (triangle, counter);
		ASSERT (counter.frontCount == 2 && counter.backCount == 1 && counter.planeCount == 0);
	}

	{
		Triangle triangle (glm::dvec3 (0.5, 0.0, 0.0), glm::dvec3 (0.5, -0.5, 0.0), glm::dvec3 (0.5, 0.5, 0.5));
		TriangleClipCounter counter;
		tree.ClipTriangle (triangle, counter);
		ASSERT (counter.frontCount == 0 && counter.backCount == 0 && counter.planeCount == 1);
	}
}

}
